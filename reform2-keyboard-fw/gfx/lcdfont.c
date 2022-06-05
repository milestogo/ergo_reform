#include <stdio.h>
#include <stdlib.h>

#include "input.xbm"

int main(int argc, char** argv) {
  FILE* f = 0;

  if (argc>1) f = fopen(argv[1],"w");

  int pitch = 192;

  if (f) {
    // expand bits to bytes
    unsigned char* expanded = malloc(sizeof(input_bits)*8);
    for (int i=0; i<sizeof(input_bits); i++) {
      if (i%24==0) printf("\n");
      
      for (int j=0; j<8; j++) {
        expanded[i*8+j] = (input_bits[i]&(1<<j)) ? 1 : 0;
        if (expanded[i*8+j]) {
          printf("#");
        } else {
          printf(".");
        }
      }
    }
    printf("\n");
    
    fprintf(f, "#include <avr/pgmspace.h>\n\n");
    fprintf(f, "const unsigned char font[] PROGMEM = {\n");
    
    for (int c=0; c<0x100; c++) {
      int y = c/32;
      int x = c%32;

      for (int j=0; j<6; j++) {
        unsigned char bt = 0;
        for (int i=0; i<8; i++) {
          bt |= expanded[(y*8+i)*pitch + x*6 + j]<<(i);
        }
        fprintf(f, "0x%02x, ", bt);
      }
      fprintf(f, "\n");
    }
    fprintf(f, "\n};\n");
    fclose(f);
    free(expanded);
  } else {
    printf("error: couldn't open output file.\n");
    return 1;
  }

  return 0;
}
