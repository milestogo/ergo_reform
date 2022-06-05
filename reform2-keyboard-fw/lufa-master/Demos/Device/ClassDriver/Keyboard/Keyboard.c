/*
             LUFA Library
     Copyright (C) Dean Camera, 2018.

  dean [at] fourwalledcubicle [dot] com
           www.lufa-lib.org
*/
/*
  Copyright 2018  Dean Camera (dean [at] fourwalledcubicle [dot] com)

  Permission to use, copy, modify, distribute, and sell this
  software and its documentation for any purpose is hereby granted
  without fee, provided that the above copyright notice appear in
  all copies and that both that the copyright notice and this
  permission notice and warranty disclaimer appear in supporting
  documentation, and that the name of the author not be used in
  advertising or publicity pertaining to distribution of the
  software without specific, written prior permission.

  The author disclaims all warranties with regard to this
  software, including all implied warranties of merchantability
  and fitness.  In no event shall the author be liable for any
  special, indirect or consequential damages or any damages
  whatsoever resulting from loss of use, data or profits, whether
  in an action of contract, negligence or other tortious action,
  arising out of or in connection with the use or performance of
  this software.
*/

#include "Keyboard.h"

/** Buffer to hold the previously generated Keyboard HID report, for comparison purposes inside the HID class driver. */
static uint8_t PrevKeyboardHIDReportBuffer[sizeof(USB_KeyboardReport_Data_t)];

/** LUFA HID Class driver interface configuration and state information. This structure is
 *  passed to all HID Class driver functions, so that multiple instances of the same class
 *  within a device can be differentiated from one another.
 */
USB_ClassInfo_HID_Device_t Keyboard_HID_Interface =
	{
		.Config =
			{
				.InterfaceNumber              = INTERFACE_ID_Keyboard,
				.ReportINEndpoint             =
					{
						.Address              = KEYBOARD_EPADDR,
						.Size                 = KEYBOARD_EPSIZE,
						.Banks                = 1,
					},
				.PrevReportINBuffer           = PrevKeyboardHIDReportBuffer,
				.PrevReportINBufferSize       = sizeof(PrevKeyboardHIDReportBuffer),
			},
	};


#define output_low(port,pin) port &= ~(1<<pin)
#define output_high(port,pin) port |= (1<<pin)
#define set_input(portdir,pin) portdir &= ~(1<<pin)
#define set_output(portdir,pin) portdir |= (1<<pin)
#define LED PE6

//char kbd_state[15*6];
int kdb_modifier = 0;

int main(void)
{
	SetupHardware();
	GlobalInterruptEnable();

	for (;;)
	{
		HID_Device_USBTask(&Keyboard_HID_Interface);
		USB_USBTask();
	}
}
/** Configures the board hardware and chip peripherals for the demo's functionality. */
void SetupHardware()
{
	/* Disable watchdog if enabled by bootloader/fuses */
	MCUSR &= ~(1 << WDRF);
	wdt_disable();

	/* Disable clock division */
	clock_prescale_set(clock_div_1);
	/* Hardware Initialization */
	USB_Init();

  DDRB  = 0b00111111;
  DDRD  = 0b00000000;
  DDRF  = 0b10000000;
  
  PORTB = 0b11000000;
  PORTD = 0b11111111;
  PORTF = 0b01111111;
  
  DDRE = 0b01000000;
  
  // no jtag plox
  MCUCR |=(1<<JTD);
  MCUCR |=(1<<JTD);
  
  output_low(PORTE, LED);
}

/** Event handler for the library USB Connection event. */
void EVENT_USB_Device_Connect(void)
{
}

/** Event handler for the library USB Disconnection event. */
void EVENT_USB_Device_Disconnect(void)
{
}

/** Event handler for the library USB Configuration Changed event. */
void EVENT_USB_Device_ConfigurationChanged(void)
{
	bool ConfigSuccess = true;

	ConfigSuccess &= HID_Device_ConfigureEndpoints(&Keyboard_HID_Interface);

	USB_Device_EnableSOFEvents();
}

/** Event handler for the library USB Control Request reception event. */
void EVENT_USB_Device_ControlRequest(void)
{
	HID_Device_ProcessControlRequest(&Keyboard_HID_Interface);
}

/** Event handler for the USB device Start Of Frame event. */
void EVENT_USB_Device_StartOfFrame(void)
{
	HID_Device_MillisecondElapsed(&Keyboard_HID_Interface);
}

#define 	KEY_A   0x04
#define 	KEY_B   0x05
#define 	KEY_C   0x06
#define 	KEY_D   0x07
#define 	KEY_E   0x08
#define 	KEY_F   0x09
#define 	KEY_G   0x0A
#define 	KEY_H   0x0B
#define 	KEY_I   0x0C
#define 	KEY_J   0x0D
#define 	KEY_K   0x0E
#define 	KEY_L   0x0F
#define 	KEY_M   0x10
#define 	KEY_N   0x11
#define 	KEY_O   0x12
#define 	KEY_P   0x13
#define 	KEY_Q   0x14
#define 	KEY_R   0x15
#define 	KEY_S   0x16
#define 	KEY_T   0x17
#define 	KEY_U   0x18
#define 	KEY_V   0x19
#define 	KEY_W   0x1A
#define 	KEY_X   0x1B
#define 	KEY_Y   0x1C
#define 	KEY_Z   0x1D
#define 	KEY_1   0x1E
#define 	KEY_2   0x1F
#define 	KEY_3   0x20
#define 	KEY_4   0x21
#define 	KEY_5   0x22
#define 	KEY_6   0x23
#define 	KEY_7   0x24
#define 	KEY_8   0x25
#define 	KEY_9   0x26
#define 	KEY_0   0x27
#define 	KEY_ENTER   0x28
#define 	KEY_ESCAPE   0x29
#define 	KEY_BACKSPACE   0x2A
#define 	KEY_TAB   0x2B
#define 	KEY_SPACE   0x2C
#define 	KEY_MINUS_AND_UNDERSCORE   0x2D
#define 	KEY_EQUAL_AND_PLUS   0x2E
#define 	KEY_OPENING_BRACKET_AND_OPENING_BRACE   0x2F
#define 	KEY_CLOSING_BRACKET_AND_CLOSING_BRACE   0x30
#define 	KEY_BACKSLASH_AND_PIPE   0x31
#define 	KEY_NON_US_HASHMARK_AND_TILDE   0x32
#define 	KEY_SEMICOLON_AND_COLON   0x33
#define 	KEY_APOSTROPHE_AND_QUOTE   0x34
#define 	KEY_GRAVE_ACCENT_AND_TILDE   0x35
#define 	KEY_COMMA_AND_LESS_THAN_SIGN   0x36
#define 	KEY_DOT_AND_GREATER_THAN_SIGN   0x37
#define 	KEY_SLASH_AND_QUESTION_MARK   0x38
#define 	KEY_CAPS_LOCK   0x39
#define 	KEY_F1   0x3A
#define 	KEY_F2   0x3B
#define 	KEY_F3   0x3C
#define 	KEY_F4   0x3D
#define 	KEY_F5   0x3E
#define 	KEY_F6   0x3F
#define 	KEY_F7   0x40
#define 	KEY_F8   0x41
#define 	KEY_F9   0x42
#define 	KEY_F10   0x43
#define 	KEY_F11   0x44
#define 	KEY_F12   0x45
#define 	KEY_PRINT_SCREEN   0x46
#define 	KEY_SCROLL_LOCK   0x47
#define 	KEY_PAUSE   0x48
#define 	KEY_INSERT   0x49
#define 	KEY_HOME   0x4A
#define 	KEY_PAGE_UP   0x4B
#define 	KEY_DELETE   0x4C
#define 	KEY_END   0x4D
#define 	KEY_PAGE_DOWN   0x4E
#define 	KEY_RIGHT_ARROW   0x4F
#define 	KEY_LEFT_ARROW   0x50
#define 	KEY_DOWN_ARROW   0x51
#define 	KEY_UP_ARROW   0x52
#define 	KEY_NUM_LOCK   0x53
#define 	KEY_KEYPAD_SLASH   0x54
#define 	KEY_KEYPAD_ASTERISK   0x55
#define 	KEY_KEYPAD_MINUS   0x56
#define 	KEY_KEYPAD_PLUS   0x57
#define 	KEY_KEYPAD_ENTER   0x58
#define 	KEY_KEYPAD_1_AND_END   0x59
#define 	KEY_KEYPAD_2_AND_DOWN_ARROW   0x5A
#define 	KEY_KEYPAD_3_AND_PAGE_DOWN   0x5B
#define 	KEY_KEYPAD_4_AND_LEFT_ARROW   0x5C
#define 	KEY_KEYPAD_5   0x5D
#define 	KEY_KEYPAD_6_AND_RIGHT_ARROW   0x5E
#define 	KEY_KEYPAD_7_AND_HOME   0x5F
#define 	KEY_KEYPAD_8_AND_UP_ARROW   0x60
#define 	KEY_KEYPAD_9_AND_PAGE_UP   0x61
#define 	KEY_KEYPAD_0_AND_INSERT   0x62
#define 	KEY_KEYPAD_DOT_AND_DELETE   0x63
#define 	KEY_NON_US_BACKSLASH_AND_PIPE   0x64
#define 	KEY_APPLICATION   0x65
#define 	KEY_POWER   0x66
#define 	KEY_KEYPAD_EQUAL_SIGN   0x67
#define 	KEY_F13   0x68
#define 	KEY_F14   0x69
#define 	KEY_F15   0x6A
#define 	KEY_F16   0x6B
#define 	KEY_F17   0x6C
#define 	KEY_F18   0x6D
#define 	KEY_F19   0x6E
#define 	KEY_F20   0x6F
#define 	KEY_F21   0x70
#define 	KEY_F22   0x71
#define 	KEY_F23   0x72
#define 	KEY_F24   0x73

#define 	KEY_MUTE   0x7F
#define 	KEY_VOLUME_UP   0x80
#define 	KEY_VOLUME_DOWN   0x81

const int matrix[15*6] = {
  KEY_ESCAPE, KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6, KEY_F7, KEY_F8, KEY_F9, KEY_F10, KEY_VOLUME_DOWN, KEY_VOLUME_UP, KEY_DELETE, KEY_APPLICATION,
  KEY_GRAVE_ACCENT_AND_TILDE, KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7, KEY_8, KEY_9, KEY_0, KEY_MINUS_AND_UNDERSCORE, KEY_EQUAL_AND_PLUS, KEY_BACKSPACE, 0,
  KEY_TAB, KEY_Q, KEY_W, KEY_E, KEY_R, KEY_T, KEY_Y, KEY_U, KEY_I, KEY_O, KEY_P, KEY_OPENING_BRACKET_AND_OPENING_BRACE, KEY_CLOSING_BRACKET_AND_CLOSING_BRACE, KEY_ENTER, 0,
  HID_KEYBOARD_SC_LEFT_CONTROL, KEY_A, KEY_S, KEY_D, KEY_F, KEY_G, KEY_H, KEY_J, KEY_K, KEY_L, KEY_SEMICOLON_AND_COLON, KEY_APOSTROPHE_AND_QUOTE, KEY_BACKSLASH_AND_PIPE,0,0,
  HID_KEYBOARD_SC_LEFT_SHIFT, KEY_Z, KEY_X, KEY_C, KEY_V, KEY_B, KEY_N, KEY_M, HID_KEYBOARD_SC_COMMA_AND_LESS_THAN_SIGN, HID_KEYBOARD_SC_DOT_AND_GREATER_THAN_SIGN, HID_KEYBOARD_SC_UP_ARROW, KEY_SLASH_AND_QUESTION_MARK, HID_KEYBOARD_SC_RIGHT_SHIFT, 0,0,
  KEY_APPLICATION, HID_KEYBOARD_SC_LEFT_ALT, HID_KEYBOARD_SC_LEFT_ALT, KEY_SPACE, HID_KEYBOARD_SC_LEFT_ARROW, HID_KEYBOARD_SC_DOWN_ARROW, HID_KEYBOARD_SC_RIGHT_ARROW, HID_KEYBOARD_SC_RIGHT_ALT, KEY_SPACE,KEY_SPACE,0,0,0,0,0
};

/** HID class driver callback function for the creation of HID reports to the host.
 *
 *  \param[in]     HIDInterfaceInfo  Pointer to the HID class interface configuration structure being referenced
 *  \param[in,out] ReportID    Report ID requested by the host if non-zero, otherwise callback should set to the generated report ID
 *  \param[in]     ReportType  Type of the report to create, either HID_REPORT_ITEM_In or HID_REPORT_ITEM_Feature
 *  \param[out]    ReportData  Pointer to a buffer where the created report should be stored
 *  \param[out]    ReportSize  Number of bytes written in the report (or zero if no report is to be sent)
 *
 *  \return Boolean \c true to force the sending of the report, \c false to let the library determine if it needs to be sent
 */
bool CALLBACK_HID_Device_CreateHIDReport(USB_ClassInfo_HID_Device_t* const HIDInterfaceInfo,
                                         uint8_t* const ReportID,
                                         const uint8_t ReportType,
                                         void* ReportData,
                                         uint16_t* const ReportSize)
{
	USB_KeyboardReport_Data_t* KeyboardReport = (USB_KeyboardReport_Data_t*)ReportData;

	uint8_t UsedKeyCodes = 0;

  output_low(PORTE, LED);

  for (int y=0; y<6; y++) {
    output_low(PORTB, y);
    for (int x=0; x<15; x++) {
      if (x<=1) {
        int pidx = x+6;
        if (!(PINB&(1<<pidx))) {
          output_high(PORTE, LED);
          KeyboardReport->KeyCode[UsedKeyCodes++] = matrix[y*15+x];
        }
      } else if (x>=2 && x<=9) {
        int pidx = x-2;
        if (!(PIND&(1<<pidx))) {
          output_high(PORTE, LED);
          KeyboardReport->KeyCode[UsedKeyCodes++] = matrix[y*15+x];
        }
      } else if (x>=10 && x<=11) {
        int pidx = x-10;
        if (!(PINF&(1<<pidx))) {
          output_high(PORTE, LED);
          KeyboardReport->KeyCode[UsedKeyCodes++] = matrix[y*15+x];
        }
      } else {
        int pidx = 4+x-12;
        if (!(PINF&(1<<pidx))) {
          output_high(PORTE, LED);
          KeyboardReport->KeyCode[UsedKeyCodes++] = matrix[y*15+x];
        }
      }
    }
    output_high(PORTB, y);
  }
  
	/*if (JoyStatus_LCL & JOY_UP)
	  KeyboardReport->KeyCode[UsedKeyCodes++] = HID_KEYBOARD_SC_A;
	else if (JoyStatus_LCL & JOY_DOWN)
	  KeyboardReport->KeyCode[UsedKeyCodes++] = HID_KEYBOARD_SC_B;

	if (JoyStatus_LCL & JOY_LEFT)
	  KeyboardReport->KeyCode[UsedKeyCodes++] = HID_KEYBOARD_SC_C;
	else if (JoyStatus_LCL & JOY_RIGHT)
	  KeyboardReport->KeyCode[UsedKeyCodes++] = HID_KEYBOARD_SC_D;

	if (JoyStatus_LCL & JOY_PRESS)
	  KeyboardReport->KeyCode[UsedKeyCodes++] = HID_KEYBOARD_SC_E;

	if (ButtonStatus_LCL & BUTTONS_BUTTON1)
	  KeyboardReport->KeyCode[UsedKeyCodes++] = HID_KEYBOARD_SC_F;

	if (UsedKeyCodes)
  KeyboardReport->Modifier = HID_KEYBOARD_MODIFIER_LEFTSHIFT;*/

	*ReportSize = sizeof(USB_KeyboardReport_Data_t);
	return false;
}

/** HID class driver callback function for the processing of HID reports from the host.
 *
 *  \param[in] HIDInterfaceInfo  Pointer to the HID class interface configuration structure being referenced
 *  \param[in] ReportID    Report ID of the received report from the host
 *  \param[in] ReportType  The type of report that the host has sent, either HID_REPORT_ITEM_Out or HID_REPORT_ITEM_Feature
 *  \param[in] ReportData  Pointer to a buffer where the received report has been stored
 *  \param[in] ReportSize  Size in bytes of the received HID report
 */
void CALLBACK_HID_Device_ProcessHIDReport(USB_ClassInfo_HID_Device_t* const HIDInterfaceInfo,
                                          const uint8_t ReportID,
                                          const uint8_t ReportType,
                                          const void* ReportData,
                                          const uint16_t ReportSize)
{
	//uint8_t  LEDMask   = LEDS_NO_LEDS;
	//uint8_t* LEDReport = (uint8_t*)ReportData;

	/*if (*LEDReport & HID_KEYBOARD_LED_NUMLOCK)
	  LEDMask |= LEDS_LED1;

	if (*LEDReport & HID_KEYBOARD_LED_CAPSLOCK)
	  LEDMask |= LEDS_LED3;

	if (*LEDReport & HID_KEYBOARD_LED_SCROLLLOCK)
    LEDMask |= LEDS_LED4;*/

	//LEDs_SetAllLEDs(LEDMask);
}

