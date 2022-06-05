/*
  MNT Reform 2.0 Keyboard Firmware
  See keyboard.c for Copyright
  SPDX-License-Identifier: MIT
*/

/** \file
 *
 *  Header file for Descriptors.c.
 */

#ifndef _DESCRIPTORS_H_
#define _DESCRIPTORS_H_

	/* Includes: */
		#include <avr/pgmspace.h>

		#include <LUFA/Drivers/USB/USB.h>

	/* Type Defines: */
		/** Type define for the device configuration descriptor structure. This must be defined in the
		 *  application code, as the configuration descriptor contains several sub-descriptors which
		 *  vary between devices, and which describe the device's usage to the host.
		 */
		typedef struct
		{
			USB_Descriptor_Configuration_Header_t Config;

			// Keyboard HID Interface
			USB_Descriptor_Interface_t            HID1_KeyboardInterface;
			USB_HID_Descriptor_HID_t              HID1_KeyboardHID;
			USB_Descriptor_Endpoint_t             HID1_ReportINEndpoint;
      // Media Control HID Interface
			USB_Descriptor_Interface_t            HID2_MediaControlInterface;
			USB_HID_Descriptor_HID_t              HID2_MediaControlHID;
			USB_Descriptor_Endpoint_t             HID2_ReportINEndpoint;
		} USB_Descriptor_Configuration_t;

		/** Enum for the device interface descriptor IDs within the device. Each interface descriptor
		 *  should have a unique ID index associated with it, which can be used to refer to the
		 *  interface from other descriptors.
		 */
		enum InterfaceDescriptors_t
		{
			INTERFACE_ID_Keyboard = 0, /**< Keyboard interface descriptor ID */
			INTERFACE_ID_MediaControl = 1, /**< Media Control interface descriptor ID */
		};

		/** Enum for the device string descriptor IDs within the device. Each string descriptor should
		 *  have a unique ID index associated with it, which can be used to refer to the string from
		 *  other descriptors.
		 */
		enum StringDescriptors_t
		{
			STRING_ID_Language     = 0, /**< Supported Languages string descriptor ID (must be zero) */
			STRING_ID_Manufacturer = 1, /**< Manufacturer string ID */
			STRING_ID_Product      = 2, /**< Product string ID */
		};

	/* Macros: */
		/** Endpoint address of the Keyboard HID reporting IN endpoint. */
		#define KEYBOARD_EPADDR              (ENDPOINT_DIR_IN | 1)

		/** Endpoint address of the Media Control HID reporting IN endpoint. */
		#define MEDIACONTROL_EPADDR          (ENDPOINT_DIR_IN | 3)

		/** Size in bytes of HID reporting IN endpoints. */
		#define HID_EPSIZE          8

	/* Function Prototypes: */
//uint16_t CALLBACK_USB_GetDescriptor(const uint16_t wValue,
//	                                    const uint16_t wIndex,
//	                                    const void** const DescriptorAddress)
//	                                    ATTR_WARN_UNUSED_RESULT ATTR_NON_NULL_PTR_ARG(3);

#endif
