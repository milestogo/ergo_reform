/*
NewSoftSerial.cpp 
Multi-instance software serial library for Arduino/Wiring
-- Interrupt-driven receive and other improvements by ladyada
   (http://ladyada.net)
-- Tuning, circular buffer, derivation from class Print/Stream,
   multi-instance support, porting to 8MHz processors,
   various optimizations, PROGMEM delay tables, inverse logic and 
   direct port writing by Mikal Hart (http://www.arduiniana.org)
-- Pin change interrupt macros by Paul Stoffregen (http://www.pjrc.com)
-- 20MHz processor support by Garrett Mace (http://www.macetech.com)
-- ATmega1280/2560 support by Brett Hagman (http://www.roguerobotics.com/)

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

The latest version of this library can always be found at
http://arduiniana.org.
*/

#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/io.h>
#include <stdlib.h>
#include <stdbool.h>

//#include "WConstants.h"
//#include "pins_arduino.h"
#include "serial.h"
//
// Lookup table
//
typedef struct _DELAY_TABLE
{
  long baud;
  unsigned short rx_delay_centering;
  unsigned short rx_delay_intrabit;
  unsigned short rx_delay_stopbit;
  unsigned short tx_delay;
} DELAY_TABLE;

#if F_CPU == 16000000

static const DELAY_TABLE PROGMEM table[] = 
{
  //  baud    rxcenter   rxintra    rxstop    tx
  { 115200,   1,         17,        17,       12,    },
  { 57600,    10,        37,        37,       33,    },
  { 38400,    25,        57,        57,       54,    },
  { 31250,    31,        70,        70,       68,    },
  { 28800,    34,        77,        77,       74,    },
  { 19200,    54,        117,       117,      114,   },
  { 14400,    74,        156,       156,      153,   },
  { 9600,     114,       236,       236,      233,   },
  { 4800,     233,       474,       474,      471,   },
  { 2400,     471,       950,       950,      947,   },
  { 1200,     947,       1902,      1902,     1899,  },
  { 300,      3804,      7617,      7617,     7614,  },
};

const int XMIT_START_ADJUSTMENT = 5;

#elif F_CPU == 8000000

static const DELAY_TABLE table[] PROGMEM = 
{
  //  baud    rxcenter    rxintra    rxstop  tx
  { 115200,   1,          5,         5,      3,      },
  { 57600,    1,          15,        15,     13,     },
  { 38400,    2,          25,        26,     23,     },
  { 31250,    7,          32,        33,     29,     },
  { 28800,    11,         35,        35,     32,     },
  { 19200,    20,         55,        55,     52,     },
  { 14400,    30,         75,        75,     72,     },
  { 9600,     50,         114,       114,    112,    },
  { 4800,     110,        233,       233,    230,    },
  { 2400,     229,        472,       472,    469,    },
  { 1200,     467,        948,       948,    945,    },
  { 300,      1895,       3805,      3805,   3802,   },
};

const int XMIT_START_ADJUSTMENT = 4;

#error This version of NewSoftSerial supports only 16 and 8MHz processors

#endif

#define HIGH 1
#define LOW 0

// per object data
uint8_t _receivePin;
uint8_t _receiveBitMask;
volatile uint8_t *_receivePortRegister;
uint8_t _transmitBitMask;
volatile uint8_t *_transmitPortRegister;

uint16_t _rx_delay_centering;
uint16_t _rx_delay_intrabit;
uint16_t _rx_delay_stopbit;
uint16_t _tx_delay;

uint16_t _buffer_overflow=1;
uint16_t _inverse_logic=1;

// static data
static char _receive_buffer[_SS_MAX_RX_BUFF]; 
static volatile uint8_t _receive_buffer_tail;
static volatile uint8_t _receive_buffer_head;

// public only for easy access by interrupt handlers
static inline void ser_handle_interrupt(void);

//
// Private methods
//

/* static */ 
inline void ser_tunedDelay(uint16_t delay) { 
  uint8_t tmp=0;
  register uint16_t dly asm("r24") = delay;

  asm volatile("sbiw    %0, 0x01 \n\t"
    "ldi %1, 0xFF \n\t"
    "cpi %A0, 0xFF \n\t"
    "cpc %B0, %1 \n\t"
    "brne .-10 \n\t"
    : "+r" (dly), "+a" (tmp)
    : "0" (dly)
    );
}

bool _listening = false;

bool ser_listen()
{
  _buffer_overflow = false;
  uint8_t oldSREG = SREG;
  cli();
  _receive_buffer_head = _receive_buffer_tail = 0;
  SREG = oldSREG;

  _listening = true;
  
  return true;
}

//
// The receive routine called by the interrupt handler
//
void ser_recv()
{
  uint8_t d = 0;

  // If RX line is high, then we don't see any start bit
  // so interrupt is probably not for us
  if (_inverse_logic ? ser_rx_pin_read() : !ser_rx_pin_read())
  {
    // Wait approximately 1/2 of a bit width to "center" the sample
    ser_tunedDelay(_rx_delay_centering);

    // Read each of the 8 bits
    for (uint8_t i=0x1; i; i <<= 1)
    {
      ser_tunedDelay(_rx_delay_intrabit);
      uint8_t noti = ~i;
      if (ser_rx_pin_read())
        d |= i;
      else // else clause added to ensure function timing is ~balanced
        d &= noti;
    }

    // skip the stop bit
    ser_tunedDelay(_rx_delay_stopbit);

    if (_inverse_logic)
      d = ~d;

    // if buffer full, set the overflow flag and return
    if ((_receive_buffer_tail + 1) % _SS_MAX_RX_BUFF != _receive_buffer_head) 
    {
      // save new data in buffer: tail points to where byte goes
      _receive_buffer[_receive_buffer_tail] = d; // save new byte
      _receive_buffer_tail = (_receive_buffer_tail + 1) % _SS_MAX_RX_BUFF;
    } 
    else 
    {
      _buffer_overflow = true;
    }
  }
}

void ser_tx_pin_write(uint8_t pin_state)
{
  if (pin_state == LOW) {
    //*_transmitPortRegister &= ~_transmitBitMask;
    PORTB &= ~(1<<7);
  } else {
    PORTB |= (1<<7);
    //*_transmitPortRegister |= _transmitBitMask;
  }
}

uint8_t ser_rx_pin_read()
{
  return (PINE&(1<<6));
  //return *_receivePortRegister & _receiveBitMask;
}

//
// Interrupt handling
//

static inline void ser_handle_interrupt()
{
  ser_recv();
}

ISR(INT6_vect)
{
  ser_handle_interrupt();
}

//
// Constructor
//
void ser_init(volatile uint8_t* rx_port, uint8_t rx_pin, volatile uint8_t* tx_port, uint8_t tx_pin, bool inverse_logic)
{
  _rx_delay_centering = 0;
  _rx_delay_intrabit = 0;
  _rx_delay_stopbit = 0;
  _tx_delay = 0;
  _buffer_overflow = false;
  _inverse_logic = inverse_logic;
  ser_set_tx(rx_port, rx_pin);
  ser_set_rx(rx_port, rx_pin);
}

bool ser_is_listening() {
  return _listening;
}

bool ser_overflow() {
  bool ret = _buffer_overflow;
  _buffer_overflow = false;
  return ret;
}

#define output_low(port,pin) *port &= ~(1<<pin)
#define output_high(port,pin) *port |= (1<<pin)

void ser_set_tx(volatile uint8_t* port, uint8_t pin)
{
  output_high(port, pin);
  //digitalWrite(tx, HIGH);
  _transmitBitMask = (1<<pin);
  _transmitPortRegister = port;
}

void ser_set_rx(volatile uint8_t* port, uint8_t pin)
{
  //if (!_inverse_logic)
    //  digitalWrite(rx, HIGH);  // pullup for normal logic!
  //_receivePin = rx;
  _receiveBitMask = (1<<pin);
  _receivePortRegister = port;
}

//
// Public methods
//

void ser_begin(long speed)
{
  _rx_delay_centering = _rx_delay_intrabit = _rx_delay_stopbit = _tx_delay = 0;

  for (unsigned i=0; i<sizeof(table)/sizeof(table[0]); ++i)
  {
    long baud = pgm_read_dword(&table[i].baud);
    if (baud == speed)
    {
      _rx_delay_centering = pgm_read_word(&table[i].rx_delay_centering);
      _rx_delay_intrabit = pgm_read_word(&table[i].rx_delay_intrabit);
      _rx_delay_stopbit = pgm_read_word(&table[i].rx_delay_stopbit);
      _tx_delay = pgm_read_word(&table[i].tx_delay);
      break;
    }
  }

  // Set up RX interrupts, but only if we have a valid RX baud rate
  if (_rx_delay_stopbit)
  {
    //PCICR |= 1; // enable pin change interrupt

    EICRB = (1<<4); // ISC60: Any logical change on INT6 generates an interrupt request
    EIMSK |= (1<<6); // INT6 unmask
    
    ser_tunedDelay(_tx_delay); // if we were low this establishes the end
  }

  ser_listen();
}

void ser_end()
{
  // TODO disable interrupts
  EICRB = 0;
}


// Read data from buffer
int ser_read()
{
  if (!ser_is_listening())
    return -1;

  // Empty buffer?
  if (_receive_buffer_head == _receive_buffer_tail)
    return -1;

  // Read from "head"
  uint8_t d = _receive_buffer[_receive_buffer_head]; // grab next byte
  _receive_buffer_head = (_receive_buffer_head + 1) % _SS_MAX_RX_BUFF;
  return d;
}

int ser_available()
{
  if (!ser_is_listening())
    return 0;

  return (_receive_buffer_tail + _SS_MAX_RX_BUFF - _receive_buffer_head) % _SS_MAX_RX_BUFF;
}

void ser_write(uint8_t b)
{
  if (_tx_delay == 0)
    return;

  uint8_t oldSREG = SREG;
  cli();  // turn off interrupts for a clean txmit

  // Write the start bit
  ser_tx_pin_write(_inverse_logic ? HIGH : LOW);
  ser_tunedDelay(_tx_delay + XMIT_START_ADJUSTMENT);

  // Write each of the 8 bits
  if (_inverse_logic)
  {
    for (uint8_t mask = 0x01; mask; mask <<= 1)
    {
      if (b & mask) // choose bit
        ser_tx_pin_write(LOW); // send 1
      else
        ser_tx_pin_write(HIGH); // send 0
    
      ser_tunedDelay(_tx_delay);
    }

    ser_tx_pin_write(LOW); // restore pin to natural state
  }
  else
  {
    for (uint8_t mask = 0x01; mask; mask <<= 1)
    {
      if (b & mask) // choose bit
        ser_tx_pin_write(HIGH); // send 1
      else
        ser_tx_pin_write(LOW); // send 0
    
      ser_tunedDelay(_tx_delay);
    }

    ser_tx_pin_write(HIGH); // restore pin to natural state
  }

  SREG = oldSREG; // turn interrupts back on
  ser_tunedDelay(_tx_delay);
}

#if !defined(cbi)
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

void ser_enable_timer0(bool enable) 
{
  if (enable)
#if defined(__AVR_ATmega8__)
    sbi(TIMSK, TOIE0);
#else
    sbi(TIMSK0, TOIE0);
#endif
  else 
#if defined(__AVR_ATmega8__)
    cbi(TIMSK, TOIE0);
#else
    cbi(TIMSK0, TOIE0);
#endif
}

void ser_flush()
{
  uint8_t oldSREG = SREG;
  cli();
  _receive_buffer_head = _receive_buffer_tail = 0;
  SREG = oldSREG;
}

int ser_peek()
{
  // Empty buffer?
  if (_receive_buffer_head == _receive_buffer_tail)
    return -1;

  // Read from "head"
  return _receive_buffer[_receive_buffer_head];
}
