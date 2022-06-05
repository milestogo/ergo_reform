/*
  MNT Reform 2.0 Keyboard Firmware
  See keyboard.c for Copyright
  SPDX-License-Identifier: MIT
*/

#include <avr/io.h>
#include <stdint.h>
#include "backlight.h"

int16_t pwmval = 8;

void kbd_brightness_init(void) {
  // initial brightness
  OCR0A = pwmval;

  // clear/set, WGM1:0 set (Phase correct PWM)
  TCCR0A = (1 << 7) | (0 << 6) | (0<<1) | 1;

  // 3=WGM02, (cs02 2:0 -> clock/256 = 100)
  TCCR0B = /*(1 << 3) |*/ (1 << 0) | (0 << 1) | 1;
}

void kbd_brightness_inc(void) {
  pwmval+=2;
  if (pwmval>=10) pwmval = 10;
  OCR0A = pwmval;
}

void kbd_brightness_dec(void) {
  pwmval-=2;
  if (pwmval<0) pwmval = 0;
  OCR0A = pwmval;
}

void kbd_brightness_set(int brite) {
  pwmval = brite;
  if (pwmval<0) pwmval = 0;
  if (pwmval>=10) pwmval = 10;
  OCR0A = pwmval;
}
