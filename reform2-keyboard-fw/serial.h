/*
NewSoftSerial.h
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

#ifndef NewSoftSerial_h
#define NewSoftSerial_h

/******************************************************************************
* Definitions
******************************************************************************/

#define _SS_MAX_RX_BUFF 64 // RX buffer size
#define _SS_VERSION 12 // software version of this library
#ifndef GCC_VERSION
#define GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#endif


// private methods
void ser_recv(void);
uint8_t ser_rx_pin_read(void);
void ser_tx_pin_write(uint8_t pin_state);
void ser_set_tx(volatile uint8_t* port, uint8_t pin);
void ser_set_rx(volatile uint8_t* port, uint8_t pin);

// public methods
void ser_init(volatile uint8_t* rx_port, uint8_t rx_pin, volatile uint8_t* tx_port, uint8_t tx_pin, bool inverse_logic);
void ser_begin(long speed);
bool ser_listen(void);
void ser_end(void);
bool ser_is_listening(void);
bool ser_overflow(void);
void ser_enable_timer0(bool enable);
int ser_peek(void);

void ser_write(uint8_t byte);
int ser_read(void);
int ser_available(void);
void ser_flush(void);

#endif
