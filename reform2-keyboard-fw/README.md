# MNT Reform 2.0 Keyboard Firmware

## Code Structure

- `constants.h`: Define which keyboard variant you want to build for
- `keyboard.c`: Main entrypoint that includes the chosen matrix file (keyboard layout)
- `matrix.h`: Keyboard layout definition (default)
- `matrix_*.h`: Alternative layouts
- `backlight.c`: Keyboard backlight control
- `menu.c`: OLED Menu handling
- `oled.c`: OLED graphics control
- `remote.c`: Communication with MNT Reform motherboard LPC
- `hid_report.c`: USB HID raw report handler (commands sent by OS)
- `powersave.c`: Low power/sleep mode
- `descriptors.c`: USB HID descriptors
- `i2c.c`: Soft I2C master implementation (for OLED)
- `serial.c`: Soft UART implementation
- `font.c`: Bitmap data for OLED font and icons

## Dependencies

### Debian/Ubuntu

`apt install gcc-avr avr-libc dfu-programmer`

### Mac

*TODO: is this correct?*

```
brew tap osx-cross/avr
brew install avr-gcc
brew install dfu-programmer
```

## Hacking

To change the keyboard layout, adjust the `matrix` arrays in `keyboard.c`.

## Building

*Important*: Adjust the variant settings in `constants.h` do match your keyboard or laptop model.
I.e., if you are targeting the Standalone Keyboard, uncomment the `#define KBD_VARIANT_STANDALONE`.

To build, type:
`make`

To flash, put your keyboard into [flashing mode](https://mntre.com/reform2/handbook/parts.html#keyboard-firmware) and run:
`sudo ./flash.sh`
