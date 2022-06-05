/*
  MNT Reform 2.0 Keyboard Firmware
  See keyboard.c for Copyright
  SPDX-License-Identifier: MIT
*/

#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/power.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <string.h>

#include "Config/LUFAConfig.h"
#include "descriptors.h"
#include <LUFA/Drivers/USB/USB.h>
#include <LUFA/Platform/Platform.h>

// some GPIO macros
#define output_low(port,pin) port &= ~(1<<pin)
#define output_high(port,pin) port |= (1<<pin)
#define set_input(portdir,pin) portdir &= ~(1<<pin)
#define set_output(portdir,pin) portdir |= (1<<pin)

// Top row, left to right
#define MATRIX_DEFAULT_ROW_1 \
  KEY_ESCAPE,\
  KEY_F1,\
  KEY_F2,\
  KEY_F3,\
  KEY_F4,\
  KEY_F5,\
  KEY_F6,\
  KEY_F7,\
  KEY_F8,\
  KEY_F9,\
  KEY_F10,\
  KEY_F11,\
  KEY_F12,\
  KEY_CIRCLE

// Second row
#define MATRIX_DEFAULT_ROW_2 \
  KEY_GRAVE_ACCENT_AND_TILDE,\
  KEY_1,\
  KEY_2,\
  KEY_3,\
  KEY_4,\
  KEY_5,\
  KEY_6,\
  KEY_7,\
  KEY_8,\
  KEY_9,\
  KEY_0,\
  KEY_MINUS_AND_UNDERSCORE,\
  KEY_EQUAL_AND_PLUS,\
  KEY_BACKSPACE

// Third row
#define MATRIX_DEFAULT_ROW_3 \
  KEY_TAB,\
  KEY_Q,\
  KEY_W,\
  KEY_E,\
  KEY_R,\
  KEY_T,\
  KEY_Y,\
  KEY_U,\
  KEY_I,\
  KEY_O,\
  KEY_P,\
  KEY_OPENING_BRACKET_AND_OPENING_BRACE,\
  KEY_CLOSING_BRACKET_AND_CLOSING_BRACE,\
  KEY_BACKSLASH_AND_PIPE

// Fourth row
#define MATRIX_DEFAULT_ROW_4 \
  HID_KEYBOARD_SC_LEFT_CONTROL,\
  HID_KEYBOARD_SC_APPLICATION,\
  KEY_A,\
  KEY_S,\
  KEY_D,\
  KEY_F,\
  KEY_G,\
  KEY_H,\
  KEY_J,\
  KEY_K,\
  KEY_L,\
  KEY_SEMICOLON_AND_COLON,\
  KEY_APOSTROPHE_AND_QUOTE,\
  KEY_ENTER

// Fifth row
#define MATRIX_DEFAULT_ROW_5 \
  HID_KEYBOARD_SC_LEFT_SHIFT,\
  HID_KEYBOARD_SC_NON_US_BACKSLASH_AND_PIPE,\
  KEY_Z,\
  KEY_X,\
  KEY_C,\
  KEY_V,\
  KEY_B,\
  KEY_N,\
  KEY_M,\
  HID_KEYBOARD_SC_COMMA_AND_LESS_THAN_SIGN,\
  HID_KEYBOARD_SC_DOT_AND_GREATER_THAN_SIGN,\
  KEY_SLASH_AND_QUESTION_MARK,\
  HID_KEYBOARD_SC_UP_ARROW,\
  HID_KEYBOARD_SC_RIGHT_SHIFT

// Sixth row
#define MATRIX_DEFAULT_ROW_6 \
  HID_KEYBOARD_SC_EXECUTE,\
  HID_KEYBOARD_SC_LEFT_GUI,\
  HID_KEYBOARD_SC_RIGHT_CONTROL,\
  KEY_SPACE,\
  HID_KEYBOARD_SC_LEFT_ALT,\
  HID_KEYBOARD_SC_RIGHT_ALT,\
  KEY_SPACE,\
  HID_KEYBOARD_SC_PAGE_UP,\
  HID_KEYBOARD_SC_PAGE_DOWN,\
  HID_KEYBOARD_SC_LEFT_ARROW,\
  HID_KEYBOARD_SC_DOWN_ARROW,\
  HID_KEYBOARD_SC_RIGHT_ARROW

// MediaController USB Report
typedef struct
{
  unsigned Play           : 1;
  unsigned Pause          : 1;
  unsigned FForward       : 1;
  unsigned Rewind         : 1;
  unsigned NextTrack      : 1;
  unsigned PreviousTrack  : 1;
  unsigned Stop           : 1;
  unsigned PlayPause      : 1;
  unsigned Mute           : 1;
  unsigned VolumeUp       : 1;
  unsigned VolumeDown     : 1;
  unsigned RESERVED       : 5;
} ATTR_PACKED USB_MediaReport_Data_t;

void setup_hardware(void);
void reset_keyboard_state(void);

// LUFA USB handlers
void EVENT_USB_Device_Connect(void);
void EVENT_USB_Device_Disconnect(void);
void EVENT_USB_Device_ConfigurationChanged(void);
void EVENT_USB_Device_ControlRequest(void);
void EVENT_USB_Device_StartOfFrame(void);

bool CALLBACK_HID_Device_CreateHIDReport(USB_ClassInfo_HID_Device_t* const HIDInterfaceInfo,
                                         uint8_t* const ReportID,
                                         const uint8_t ReportType,
                                         void* ReportData,
                                         uint16_t* const ReportSize);

void CALLBACK_HID_Device_ProcessHIDReport(USB_ClassInfo_HID_Device_t* const HIDInterfaceInfo,
                                          const uint8_t ReportID,
                                          const uint8_t ReportType,
                                          const void* ReportData,
                                          const uint16_t ReportSize);

#endif
