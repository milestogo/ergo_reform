/*
  MNT Reform 2.0 Keyboard Firmware
  See keyboard.c for Copyright
  SPDX-License-Identifier: MIT
*/

#include <stdint.h>
#include "backlight.h"
#include "hid_report.h"
#include "keyboard.h"
#include "menu.h"
#include "oled.h"
#include "powersave.h"
#include "remote.h"

// hid commands are all 4-letter, so they fit in a 32 bit integer
#define cmd(_s) (*(uint32_t *)(_s))
#define CMD_TEXT_FRAME      cmd("OLED")     // fill the screen with a single wall of text
#define CMD_ROW_INVERT      cmd("OINV")     // invert a line of text
#define CMD_REPORT_POWER    cmd("RPRT")     // ask for power stats report over UART
#define CMD_OLED_CLEAR      cmd("WCLR")     // clear the oled display
#define CMD_OLED_BITMAP     cmd("WBIT")     // (u16 offset, u8 bytes...) write raw bytes into the oled framebuffer
#define CMD_POWER_OFF       cmd("PWR0")     // turn off power rails
#define CMD_BACKLIGHT       cmd("LITE")     // keyboard backlight level
#define CMD_UART_ON         cmd("UAR1")     // uart reporting on
#define CMD_UART_OFF        cmd("UAR0")     // uart reporting off

void hid_report_cmd(uint8_t* data) {
  const uint32_t command = *(uint32_t *)data;
  
  if (command == CMD_TEXT_FRAME) {
    gfx_on();
    for (int y=0; y<4; y++) {
      for (int x=0; x<21; x++) {
        gfx_poke(x,y,data[4+y*21+x]);
      }
    }
    gfx_flush();
  }
  else if (command == CMD_ROW_INVERT) {
    gfx_clear_invert();
    gfx_invert_row(data[4]-'0');
  }
  else if (command == CMD_BACKLIGHT) {
    char brite = data[4]-'0';
    brite++;
    if (brite<=1) brite=0;
    if (brite>9) brite=9;
    kbd_brightness_set(brite);
  }
  else if (command == CMD_POWER_OFF) {
    anim_goodbye();
    remote_turn_off_som();
    keyboard_power_off();
    reset_keyboard_state();
  }
  else if (command == CMD_UART_ON) {
    remote_enable_som_uart();
  }
  else if (command == CMD_UART_OFF) {
    remote_disable_som_uart();
  }
  else if (command == CMD_REPORT_POWER) {
    remote_report_voltages();
  }
  else if (command == CMD_OLED_BITMAP) {
    matrix_render_direct(data+4);
  }
  else if (command == CMD_OLED_BITMAP) {
    gfx_clear();
    gfx_flush();
  }
}
