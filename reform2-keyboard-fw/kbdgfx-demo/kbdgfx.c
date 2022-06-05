/*
  kbdgfx.c -- Demo for drawing realtime graphics to the MNT Reform Keyboard
  Copyright 2022 MNT Research GmbH (https://mntre.com)
  License: MIT
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <unistd.h>

#define ROWS 4
#define COLS 126
#define BUFSZ (5+COLS*ROWS)
#define FBUFSZ COLS*ROWS*8

// our unpacked, wasteful framebuffer (one byte per pixel, 126x32)
uint8_t fb[FBUFSZ];

// the buffer we're sending to the keyboard (bit packed and column byte order)
uint8_t buf[BUFSZ];

void oled_blit(uint8_t* src, uint8_t* dst) {
  for (int y = 0; y < ROWS; y++) {
    // right-to-left
    for (int x = 0; x < COLS; x++) {
      uint8_t column = 0;

      for (int z = 0; z < 8; z++) {
        // look up the pixel and threshold it to black and white at 127
        uint8_t bit = src[((y * 8 + 7 - z)*COLS + x)] > 127;

        // bitshift the column byte to the left to make room for new pixel
        column <<= 1;
        // OR the bit (the pixel) to the column byte
        column |= bit;
      }

      // store in the destination buffer
      dst[y*COLS + x] = column;
    }
  }
}

// unused
void fill_pattern(uint8_t bitpattern, uint8_t* dst) {
  int i = 0;
  for (int y = 0; y < 32; y++) {
    for (int x = 0; x < COLS; x++) {
      uint8_t pos = x % 8;
      uint8_t b = bitpattern & (1<<pos);

      dst[i++] = b ? 1 : 0;
    }
  }
}

void draw_sine(float t, uint8_t* dst) {
  for (int x=0; x<126; x++) {
    int y = 16 + sin(t + ((float)x/126.0 * 3.141))*12;
    if (y < 0) y = 0;
    if (y > 31) y = 31;

    dst[y*COLS + x] = 0xff;
  }
}

int main(int argc, char** argv) {
  // just a counter
  uint32_t t = 0;

  if (argc < 2) {
    printf("Usage: sudo kbdgfx /dev/hidraw0 [bitmap.raw]\n");
    exit(1);
  }

  // loop forever
  while (1) {
    FILE* f = fopen(argv[1],"w");

    if (!f) {
      printf("Couldn't open %s. Try sudo.\n", argv[1]);
      exit(1);
    }

    // start with the command
    buf[0] = 'x';
    buf[1] = 'W';
    buf[2] = 'B';
    buf[3] = 'I';
    buf[4] = 'T';

    // clear
    memset(fb, 0, FBUFSZ);

    if (argc == 3) {
      // read bitmap from file
      FILE* bmf = fopen(argv[2],"r");
      if (!bmf) {
        printf("Couldn't open bitmap %s!\n", argv[2]);
        fclose(f);
        exit(2);
      }

      int res = fread(fb, FBUFSZ, 1, bmf);
      fclose(bmf);
      
      if (res<1) {
        printf("Couldn't read bitmap or wrong size.\n", argv[2]);
        fclose(f);
        exit(3);
      }
    } else {
      // graphics demo
    
      // paint
      draw_sine((float)t*0.03, fb);
      draw_sine((float)t*0.05, fb);
    }
    
    // convert to weird OLED buffer format
    oled_blit(fb, buf+5);

    // send our buffer to the keyboard
    fwrite(buf, BUFSZ, 1, f);

    fclose(f);

    // if we're in bitmap file mode, exit now
    if (argc == 3) exit(0);

    // ~50 FPS
    usleep(1000*20);
    // ~2 FPS
    //usleep(1000*500);
    t++;
  }
}
