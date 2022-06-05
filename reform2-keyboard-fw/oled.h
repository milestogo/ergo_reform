/*
  MNT Reform 2.0 Keyboard Firmware
  See keyboard.c for Copyright
  SPDX-License-Identifier: MIT
*/

#ifndef _OLED_H_
#define _OLED_H_

#include <stdbool.h>
#include <stdint.h>

enum ssd1306_cmds {
  DisplayOff = 0xAE,
  DisplayOn = 0xAF,

  SetContrast = 0x81,
  DisplayAllOnResume = 0xA4,

  DisplayAllOn = 0xA5,
  NormalDisplay = 0xA6,
  InvertDisplay = 0xA7,
  SetDisplayOffset = 0xD3,
  SetComPins = 0xda,
  SetVComDetect = 0xdb,
  SetDisplayClockDiv = 0xD5,
  SetPreCharge = 0xd9,
  SetMultiPlex = 0xa8,
  SetLowColumn = 0x00,
  SetHighColumn = 0x10,
  SetStartLine = 0x40,

  SetMemoryMode = 0x20,
  ColumnAddr = 0x21,
  PageAddr = 0x22,

  ComScanInc = 0xc0,
  ComScanDec = 0xc8,
  SegRemap = 0xa0,
  SetChargePump = 0x8d,
  ExternalVcc = 0x01,
  SwitchCapVcc = 0x02,

  ActivateScroll = 0x2f,
  DeActivateScroll = 0x2e,
  SetVerticalScrollArea = 0xa3,
  RightHorizontalScroll = 0x26,
  LeftHorizontalScroll = 0x27,
  VerticalAndRightHorizontalScroll = 0x29,
  VerticalAndLeftHorizontalScroll = 0x2a,
};

#define SSD1306_ADDRESS 0x3C

#define DisplayHeight 32
#define DisplayWidth 128

#define FontHeight 8
#define FontWidth 6

#define MatrixRows (DisplayHeight / FontHeight)
#define MatrixCols (DisplayWidth / FontWidth)

struct CharacterMatrix {
  uint8_t display[MatrixRows][MatrixCols];
  uint8_t invert[MatrixRows][MatrixCols];
  uint8_t *cursor;
  bool dirty;
};

struct CharacterMatrix display;

void gfx_poke(uint8_t x, uint8_t y, uint8_t c);
void gfx_poke_str(uint8_t x, uint8_t y, char* str);
void gfx_clear_invert(void);
void gfx_invert_row(uint8_t y);
bool gfx_init(bool rotate);
void gfx_task(void);
bool gfx_off(void);
bool gfx_on(void);
void gfx_flush(void);
void gfx_clear(void);
void gfx_write_char(uint8_t c);
void gfx_write(const char *data);
void gfx_write_P(const char *data);
void gfx_clear_screen(void);
void gfx_contrast(int c);

void matrix_clear(struct CharacterMatrix *matrix);
void matrix_write_char_inner(struct CharacterMatrix *matrix, uint8_t c);
void matrix_write_char(struct CharacterMatrix *matrix, uint8_t c);
void matrix_write(struct CharacterMatrix *matrix, const char *data);
void matrix_write_ln(struct CharacterMatrix *matrix, const char *data);
void matrix_write_P(struct CharacterMatrix *matrix, const char *data);
void matrix_render(struct CharacterMatrix *matrix);
void matrix_render_direct(uint8_t* bitmap);

void oled_brightness_inc(void);
void oled_brightness_dec(void);

#endif
