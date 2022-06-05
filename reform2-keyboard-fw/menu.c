/*
  MNT Reform 2.0 Keyboard Firmware
  See keyboard.c for Copyright
  SPDX-License-Identifier: MIT
*/

#include "backlight.h"
#include "constants.h"
#include "keyboard.h"
#include "menu.h"
#include "oled.h"
#include "powersave.h"
#include "remote.h"
#include "scancodes.h"

int current_menu_y = 0;
int current_scroll_y = 0;

#ifdef KBD_VARIANT_STANDALONE
#define MENU_NUM_ITEMS 5
const MenuItem menu_items[] = {
  { "Exit Menu         ESC", KEY_ESCAPE },
  { "Key Backlight-     F1", KEY_F1 },
  { "Key Backlight+     F2", KEY_F2 },
  { "System Status       s", KEY_S },
  { "USB Flashing Mode   x", KEY_X },
};
#else
#ifdef KBD_VARIANT_ERGO
#define MENU_NUM_ITEMS 7
const MenuItem menu_items[] = {
  { "Exit Menu         ESC", KEY_ESCAPE },
  { "Power On            1", KEY_1 },
  { "Power Off           0", KEY_0 },
  { "Reset               r", KEY_R },
  { "Battery Status      b", KEY_B },
  { "Wake              SPC", KEY_SPACE },
  { "System Status       s", KEY_S },
  // Only needed for debugging.
  // The keyboard will go to sleep when turning off
  // main system power.
  { "KBD Power-Off       p", KEY_P },
};
#else
#define MENU_NUM_ITEMS 9
const MenuItem menu_items[] = {
  { "Exit Menu         ESC", KEY_ESCAPE },
  { "Power On            1", KEY_1 },
  { "Power Off           0", KEY_0 },
  { "Reset               r", KEY_R },
  { "Battery Status      b", KEY_B },
  { "Key Backlight-     F1", KEY_F1 },
  { "Key Backlight+     F2", KEY_F2 },
  { "Wake              SPC", KEY_SPACE },
  { "System Status       s", KEY_S },

  // Only needed for debugging.
  // The keyboard will go to sleep when turning off
  // main system power.
  { "KBD Power-Off       p", KEY_P },
};
#endif //ergo
#endif // standalone


void reset_and_render_menu() {
  current_scroll_y = 0;
  current_menu_y = 0;
  render_menu(current_scroll_y);
}

void render_menu(int y) {
  gfx_clear();
  gfx_invert_row(current_menu_y-y);
  for (int i=0; i<MENU_NUM_ITEMS; i++) {
    gfx_poke_str(0,i-y,menu_items[i].title);
  }
  gfx_on();
  gfx_flush();
}

int execute_menu_function(int y) {
  if (y>=0 && y<MENU_NUM_ITEMS) {
    return execute_meta_function(menu_items[y].keycode);
  }
  return execute_meta_function(KEY_ESCAPE);
}

#define BOOTLOADER_START_ADDRESS ((0x8000-0x1000) >> 1)
void jump_to_bootloader(void) {
  ((void (*)(void))BOOTLOADER_START_ADDRESS)();
}

// returns 1 for navigation function (stay in meta mode), 0 for terminal function
int execute_meta_function(int keycode) {
  if (keycode == KEY_0) {
    // TODO: are you sure?
    anim_goodbye();
    remote_turn_off_som();
    keyboard_power_off();
    reset_keyboard_state();
    // Directly enter menu again
    return 2;
  }
  else if (keycode == KEY_1) {
    if (remote_turn_on_som()) {
      anim_hello();
    }
    kbd_brightness_init();
    return 0;
  }
  else if (keycode == KEY_R) {
    // TODO: are you sure?
    remote_reset_som();
  }
  else if (keycode == KEY_SPACE) {
    remote_wake_som();
  }
  /*else if (keycode == KEY_V) {
    remote_turn_off_aux();
  }*/
  else if (keycode == KEY_B) {
    remote_get_voltages();
    return 0;
  }
  else if (keycode == KEY_S) {
    remote_get_status();
    return 0;
  }
  else if (keycode == KEY_F1) {
    kbd_brightness_dec();
    return 1;
  }
  else if (keycode == KEY_F2) {
    kbd_brightness_inc();
    return 1;
  }
  else if (keycode == HID_KEYBOARD_SC_UP_ARROW) {
    current_menu_y--;
    if (current_menu_y<0) current_menu_y = 0;
    if (current_menu_y<=current_scroll_y) current_scroll_y--;
    if (current_scroll_y<0) current_scroll_y = 0;
    render_menu(current_scroll_y);
    return 1;
  }
  else if (keycode == HID_KEYBOARD_SC_DOWN_ARROW) {
    current_menu_y++;
    if (current_menu_y>=MENU_NUM_ITEMS) current_menu_y = MENU_NUM_ITEMS-1;
    if (current_menu_y>=current_scroll_y+3) current_scroll_y++;
    render_menu(current_scroll_y);
    return 1;
  }
  else if (keycode == KEY_ENTER) {
    return execute_menu_function(current_menu_y);
  }
  else if (keycode == KEY_ESCAPE) {
    gfx_clear();
    gfx_flush();
  }
  else if (keycode == KEY_X) {
    jump_to_bootloader();
  }

  gfx_clear();
  gfx_flush();

  return 0;
}

void anim_hello(void) {
  gfx_clear();
  gfx_on();
  for (int y=0; y<3; y++) {
    for (int x=0; x<12; x++) {
      gfx_poke(x+4,y+1,(5+y)*32+x);
      gfx_flush();
    }
  }
  for (int y=0; y<0xff; y++) {
    gfx_contrast(y);
    Delay_MS(2);
  }
  for (int y=0; y<0xff; y++) {
    gfx_contrast(0xff-y);
    Delay_MS(2);
  }
}

void anim_goodbye(void) {
  gfx_clear();
  gfx_on();
  for (int y=0; y<3; y++) {
    for (int x=0; x<12; x++) {
      gfx_poke(x+4,y+1,(5+y)*32+x);
    }
  }
  for (int y=0; y<3; y++) {
    for (int x=0; x<12; x++) {
      gfx_poke(x+4,y+1,' ');
      gfx_flush();
    }
  }
  gfx_off();
}
