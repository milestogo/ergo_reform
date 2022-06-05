/*
  MNT Reform 2.0 Keyboard Firmware
  See keyboard.c for Copyright
  SPDX-License-Identifier: MIT
*/

// OLED (SSD1306) rendering code. The OLED is interfaced via I2C.

#include "oled.h"
#include "i2c.h"
#include <stdio.h>
#include <string.h>
#include <avr/pgmspace.h>
#include "gfx/font.c"

int oledbrt = 0;

// Write command sequence.
// Returns true on success.
static inline bool _send_cmd1(uint8_t cmd) {
  bool res = false;

  if (i2c_start_write(SSD1306_ADDRESS)) {
    // failed to start write
    goto done;
  }

  if (i2c_master_write(0x0 /* command byte follows */)) {
    // failed to write control byte
    goto done;
  }

  if (i2c_master_write(cmd)) {
    //  failed to write command
    goto done;
  }
  res = true;
done:
  i2c_master_stop();
  return res;
}

// Write 2-byte command sequence.
// Returns true on success
static inline bool _send_cmd2(uint8_t cmd, uint8_t opr) {
  if (!_send_cmd1(cmd)) {
    //return false;
  }
  return _send_cmd1(opr);
}

// Write 3-byte command sequence.
// Returns true on success
static inline bool _send_cmd3(uint8_t cmd, uint8_t opr1, uint8_t opr2) {
  if (!_send_cmd1(cmd)) {
    //return false;
  }
  if (!_send_cmd1(opr1)) {
    //return false;
  }
  return _send_cmd1(opr2);
}

#define send_cmd1(c) if (!_send_cmd1(c)) {goto done;}
#define send_cmd2(c,o) if (!_send_cmd2(c,o)) {goto done;}
#define send_cmd3(c,o1,o2) if (!_send_cmd3(c,o1,o2)) {goto done;}

static void clear_display(void) {
  matrix_clear(&display);

  // Clear all of the display bits (there can be random noise
  // in the RAM on startup)
  send_cmd3(PageAddr, 0, (DisplayHeight / 8) - 1);
  send_cmd3(ColumnAddr, 0, DisplayWidth - 1);

  if (i2c_start_write(SSD1306_ADDRESS)) {
    goto done;
  }
  if (i2c_master_write(0x40)) {
    // Data mode
    goto done;
  }
  for (uint8_t row = 0; row < MatrixRows; ++row) {
    for (uint8_t col = 0; col < DisplayWidth; ++col) {
      i2c_master_write(0);
    }
  }

  display.dirty = false;

done:
  i2c_master_stop();
}

bool gfx_init(bool rotate) {
  bool success = false;
	rotate = false; // FIXME

  i2c_master_init();
  send_cmd1(DisplayOff);
  send_cmd2(SetDisplayClockDiv, 0x80);
  send_cmd2(SetMultiPlex, DisplayHeight - 1);

  send_cmd2(SetDisplayOffset, 0);

  send_cmd1(SetStartLine | 0x0);
  send_cmd2(SetChargePump, 0x14 /* Enable */);
  send_cmd2(SetMemoryMode, 0 /* horizontal addressing */);

  if (rotate) {
    // the following Flip the display orientation 180 degrees
    send_cmd1(SegRemap);
    send_cmd1(ComScanInc);
  } else {
    // Flips the display orientation 0 degrees
    send_cmd1(SegRemap | 0x1);
    send_cmd1(ComScanDec);
  }

  send_cmd2(SetComPins, 0x2);
  send_cmd2(SetContrast, 0x8f);
  send_cmd2(SetPreCharge, 0xf1);
  send_cmd2(SetVComDetect, 0x40);
  send_cmd1(DisplayAllOnResume);
  send_cmd1(NormalDisplay);
  send_cmd1(DeActivateScroll);
  send_cmd1(DisplayOn);

  send_cmd2(SetContrast, 0); // Dim

  clear_display();

  success = true;

  gfx_flush();

done:
  return success;
}

bool gfx_off(void) {
  bool success = false;

  //send_cmd1(InvertDisplay);
  send_cmd1(DisplayOff);
  success = true;

done:
  return success;
}

bool gfx_on(void) {
  bool success = false;

  send_cmd1(NormalDisplay);
  send_cmd1(DisplayOn);
  success = true;

done:
  return success;
}

void gfx_clear(void) {
  for (int y=0; y<4; y++) {
    for (int x=0; x<21; x++) {
      gfx_poke(x,y,' ');
    }
  }
  gfx_clear_invert();
}

void gfx_contrast(int c) {
  send_cmd2(SetContrast, c);
done:
  return;
}

void matrix_write_char_inner(struct CharacterMatrix *matrix, uint8_t c) {
  *matrix->cursor = c;
  ++matrix->cursor;

  if (matrix->cursor - &matrix->display[0][0] == sizeof(matrix->display)) {
    // We went off the end; scroll the display upwards by one line
    memmove(&matrix->display[0], &matrix->display[1],
            MatrixCols * (MatrixRows - 1));
    matrix->cursor = &matrix->display[MatrixRows - 1][0];
    memset(matrix->cursor, ' ', MatrixCols);
  }
}

void matrix_write_char(struct CharacterMatrix *matrix, uint8_t c) {
  matrix->dirty = true;

  if (c == '\n') {
    // Clear to end of line from the cursor and then move to the
    // start of the next line
    uint8_t cursor_col = (matrix->cursor - &matrix->display[0][0]) % MatrixCols;

    while (cursor_col++ < MatrixCols) {
      matrix_write_char_inner(matrix, ' ');
    }
    return;
  }

  matrix_write_char_inner(matrix, c);
}

void gfx_poke(uint8_t x, uint8_t y, uint8_t c) {
	display.display[y][x] = c;
}

void gfx_poke_str(uint8_t x, uint8_t y, char* str) {
  int len = strlen(str);
  if (len>21) len = 21;
  // clip
  if (y<0 || y>3) return;

  for (int xx=x; xx<x+len && xx<21; xx++) {
    if (xx>=0 && xx<21) {
      display.display[y][xx] = (uint8_t)str[xx-x];
    }
  }
}

void gfx_write_char(uint8_t c) {
  matrix_write_char(&display, c);
}

void matrix_write(struct CharacterMatrix *matrix, const char *data) {
  const char *end = data + strlen(data);
  while (data < end) {
    matrix_write_char(matrix, *data);
    ++data;
  }
}

void matrix_write_ln(struct CharacterMatrix *matrix, const char *data) {
  char data_ln[strlen(data)+2];
  snprintf(data_ln, sizeof(data_ln), "%s\n", data);
  matrix_write(matrix, data_ln);
}

void gfx_write(const char *data) {
  matrix_write(&display, data);
}

void matrix_write_P(struct CharacterMatrix *matrix, const char *data) {
  while (true) {
    uint8_t c = pgm_read_byte(data);
    if (c == 0) {
      return;
    }
    matrix_write_char(matrix, c);
    ++data;
  }
}

void gfx_write_P(const char *data) {
  matrix_write_P(&display, data);
}

void matrix_clear(struct CharacterMatrix *matrix) {
  memset(matrix->display, ' ', sizeof(matrix->display));
  matrix->cursor = &matrix->display[0][0];
  matrix->dirty = true;
}

void gfx_clear_screen(void) {
  matrix_clear(&display);
}

void gfx_clear_invert(void) {
  for (int y=0;y<4;y++) {
    for (int x=0;x<21;x++) {
      display.invert[y][x] = 0;
    }
  }
}

void gfx_invert_row(uint8_t y) {
  if (y<0 || y>3) return;
  for (int x=0;x<21;x++) {
    display.invert[y][x] = 1;
  }
}

void matrix_render(struct CharacterMatrix *matrix) {
  gfx_on();

  // Move to the home position
  send_cmd3(PageAddr, 0, MatrixRows - 1);
  send_cmd3(ColumnAddr, 0, (MatrixCols * FontWidth) - 1);

  if (i2c_start_write(SSD1306_ADDRESS)) {
    //goto done;
  }
  if (i2c_master_write(0x40)) {
    // Data mode
    //goto done;
  }

  for (uint8_t row = 0; row < MatrixRows; ++row) {
    for (uint8_t col = 0; col < MatrixCols; ++col) {
      const uint8_t *glyph = font + (matrix->display[row][col] * FontWidth);
      const uint8_t invert = matrix->invert[row][col];

      for (uint8_t glyphCol = 0; glyphCol < FontWidth; ++glyphCol) {
        uint8_t colBits = pgm_read_byte(glyph + glyphCol);
        if (invert) colBits = ~colBits;
        i2c_master_write(colBits);
      }
    }
  }

  matrix->dirty = false;

done:
  i2c_master_stop();
}

void matrix_render_direct(uint8_t* bitmap) {
  gfx_on();

  // Move to the home position
  send_cmd3(PageAddr, 0, MatrixRows - 1);
  send_cmd3(ColumnAddr, 0, (MatrixCols * FontWidth) - 1);

  i2c_start_write(SSD1306_ADDRESS);
  i2c_master_write(0x40);

  int c = 0;
  for (uint16_t y=0; y<4; y++) {
    for (uint16_t x=0; x<126; x++) {
      i2c_master_write(bitmap[c++]);
    }
  }

done:
  i2c_master_stop();
}

void gfx_flush(void) {
  matrix_render(&display);
}

void oled_brightness_inc(void) {
  oledbrt+=10;
  if (oledbrt>=0xff) oledbrt = 0xff;
  gfx_contrast(oledbrt);
}

void oled_brightness_dec(void) {
  oledbrt-=10;
  if (oledbrt<0) oledbrt = 0;
  gfx_contrast(oledbrt);
}
