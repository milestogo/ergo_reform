

dfu-programmer atmega32u4 erase --suppress-bootloader-mem 

dfu-programmer atmega32u4 flash ./Keyboard.hex --suppress-bootloader-mem 

dfu-programmer atmega32u4 start 
