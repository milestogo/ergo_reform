
#
## MCU name
MCU = atmega32u4
F_CPU=8000000
ARCH=avr8
LTO_ENABLE=yes

SPACE_CADET_ENABLE=no
GRAVE_ESC_ENABLE=no
OLED_ENABLE=yes

# Bootloader selection
BOOTLOADER = caterina

SRC += reform_serial.c


QUANTUM_LIB_SRC += uart.c

OLED_ENABLE= yes
#OLED_DRIVER = SSD1306
