/*
  MNT Reform 2.0 Keyboard Firmware
  See keyboard.c for Copyright
  SPDX-License-Identifier: MIT

  ERGO mods Copyright milestogo and under MIT license. 
  Added https://source.mnt.re/reform/reform/-/merge_requests/26 

*/

#include <avr/io.h>
#include <avr/sleep.h>
#include "powersave.h"
#include "keyboard.h"
#include "oled.h"

/* Setup the AVR to enter the Power-Down state to greatly save power.
 * Configures all outputs to be in the low state if possible, and disables
 * services like USB and Serial.
 *
 * Will leave the ports setup so that the Circle key row is being scanned
 * so when the watchdog wakes up it can quickly check and go back to sleep if not
 * Added by Chartreuse - 2021/08/14
 *
 */
void keyboard_power_off(void)
{
  USB_Disable(); // Stop USB stack so it doesn't wake us up

  // turn off backlight, but don't overwrite setting
  OCR0A = 0;

  // Turn off OLED to save power
  gfx_clear_screen();
  gfx_off();
  // Disable ADC to save even more power
  ADCSRA=0;

  cli();    // No interrupts

  // Set all ports not floating if possible, leaving pullups alone
  #ifdef KBD_VARIANT_ERGO
  // Want row 6 (PC7) low, Col 1(PB3) high
  PORTB=0xF6; // Leave pull-up on all Cols  2-5 on PB1,2,3, drive rows 2-4 high (B5-B7), PB0-low, Col0/B3floats for circle key
  PORTC=0x00; // also drop PWM pin on C6
  PORTD=0xD0; // Keep pullup on Col5,6 like setup did, drive rows 5 high
  PORTE=0x40; // Pullup on PE6
  PORTF=0xFF; // Pullups on PF (columns)
  // ROW6 is the only row driven low and left low, thus is always ready to be read out
  // We just need to check COL0 (PB3) if it is low (pressed) or high
 
 #else
  PORTB=0x3F; // Leave pull-up on all the columns on PB0-3, drive rows 2-3 high, 1-low
  PORTC=0xC0;
  PORTD=0xF0; // Keep pullup on PD5 like setup did, drive rows 4,5,6 high
  PORTE=0x40; // Pullup on PE6
  PORTF=0xFF; // Pullups on PF (columns)
  // ROW1 is the only row driven low and left low, thus is always ready to be read out
  // We just need to check COL14 (PC6) if it is low (pressed) or high

  // Unfortunately the circle key is on COL14(PC6) which doesn't have pin change interrupt
  // capabilities, so we need to wake up every so often to check if it is pressed, and
  // if so bring us out of power-off
  // We can use the Watchdog timer to do this.
#endif 
  do {
    // Setting WDT parameters must be done within 4 cycles of setting WDCE, so interrupts
    // must be disabled.
    cli();

    wdt_reset();
    // Enable writes to watchdog, then set interrupt-only mode, 1s timeout.
    // (Interrupt-only mode is preferred to interrupt-and-reset mode, because
    // the latter has the risk of WDT-induced bootloops if the bootloader doesn't
    // correctly handle WDT resets. Whereas we have a physical reset button, if
    // a hard reset is actually needed.)
    WDTCSR = (1<<WDCE) | (1<<WDE);
    WDTCSR = (1<<WDIE) | (0<<WDE) | (0<<WDP3) | (1<<WDP2) | (1<<WDP1) | (0<<WDP0);
 

    // Enter Power-save mode
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_enable();
    sei();              // Enable interrupts so we can actually wake
    sleep_cpu();        // Actually go to sleep
    // Zzzzzz
    sleep_disable();    // We've woken up
    wdt_disable();
    sei();
    // Check if circle key has been pressed (active-low)
    // If not reset the watchdog and try again
#ifdef KBD_VARIANT_ERGO
 }  while( PINB&(1<<3) ); // Col1
#else
  } while(PINC&(1<<6)); // Col 14
#endif 
  // Resume and reinitialize hardware
  setup_hardware();
}
