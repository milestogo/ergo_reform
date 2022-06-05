#
#             LUFA Library
#     Copyright (C) Dean Camera, 2018.
#
#  dean [at] fourwalledcubicle [dot] com
#           www.lufa-lib.org
#
# --------------------------------------
#         LUFA Project Makefile.
# --------------------------------------

# Run "make help" for target help.

MCU          = atmega32u4
ARCH         = AVR8
BOARD        = USBKEY

# default
## ifdef KBD_VARIANT_ERGO
F_CPU=8000000
#else factory default
#F_CPU        = 16000000

F_USB        = $(F_CPU)

OPTIMIZATION = s
TARGET       = keyboard
SRC          = $(TARGET).c descriptors.c i2c.c oled.c remote.c powersave.c backlight.c menu.c hid_report.c $(LUFA_SRC_USB) $(LUFA_SRC_USBCLASS)
LUFA_PATH    = ./lufa-master/LUFA
CC_FLAGS     = -DUSE_LUFA_CONFIG_HEADER $(REFORM_KBD_OPTIONS) -IConfig/
LD_FLAGS     = -Wl,-u,vfprintf -lprintf_flt

# Default target
all:

# Include LUFA-specific DMBS extension modules
DMBS_LUFA_PATH ?= $(LUFA_PATH)/Build/LUFA
include $(DMBS_LUFA_PATH)/lufa-sources.mk
include $(DMBS_LUFA_PATH)/lufa-gcc.mk

# Include common DMBS build system modules
DMBS_PATH      ?= $(LUFA_PATH)/Build/DMBS/DMBS
include $(DMBS_PATH)/core.mk
include $(DMBS_PATH)/cppcheck.mk
include $(DMBS_PATH)/doxygen.mk
include $(DMBS_PATH)/dfu.mk
include $(DMBS_PATH)/gcc.mk
include $(DMBS_PATH)/hid.mk
include $(DMBS_PATH)/avrdude.mk
include $(DMBS_PATH)/atprogram.mk
