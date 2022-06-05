/*
  MNT Reform 2.0 Keyboard Firmware
  See keyboard.c for Copyright
  SPDX-License-Identifier: MIT
*/

#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_

#define KBD_FW_REV "R1 20220221"
//#define KBD_VARIANT_QWERTY_US
//#define KBD_VARIANT_STANDALONE
//#define KBD_VARIANT_NEO2
//#define KBD_VARIANT_V

#define KBD_VARIANT_ERGO

#ifdef KBD_VARIANT_ERGO
#undef KBD_FW_REV
#define KBD_FW_REV "ERGO 20220221"
#define KBD_COLS 12
#define KBD_ROWS 6

#else // factory keyboard
#define KBD_COLS 14
#define KBD_ROWS 6
#endif

#define KBD_MATRIX_SZ KBD_COLS * KBD_ROWS + 4
#define KBD_EDITOR_MARKER 0xfe,0xed,0xca,0xfe

#endif
