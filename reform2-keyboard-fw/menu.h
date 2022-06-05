/*
  MNT Reform 2.0 Keyboard Firmware
  See keyboard.c for Copyright
  SPDX-License-Identifier: MIT
*/

#ifndef _MENU_H_
#define _MENU_H_

typedef struct MenuItem {
  char* title;
  int keycode;
} MenuItem;

void reset_and_render_menu(void);
void render_menu(int y);
int execute_menu_function(int y);
int execute_meta_function(int keycode);
void anim_hello(void);
void anim_goodbye(void);

#endif
