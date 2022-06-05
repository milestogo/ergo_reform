/*
  MNT Reform 2.0 Keyboard Firmware
  See keyboard.c for Copyright
  SPDX-License-Identifier: MIT
*/

#ifndef _BACKLIGHT_H_
#define _BACKLIGHT_H_

void kbd_brightness_init(void);
void kbd_brightness_inc(void);
void kbd_brightness_dec(void);
void kbd_brightness_set(int brite);

#endif
