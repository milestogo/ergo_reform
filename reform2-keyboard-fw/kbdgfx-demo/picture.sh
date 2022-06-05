#!/bin/bash

# Usage: ./picture.sh /dev/hidraw3 picture.png

convert $2 -resize 126 -gravity center -extent 126x32 -depth 8 gray:temp.bin
sudo ./kbdgfx $1 ./temp.bin

