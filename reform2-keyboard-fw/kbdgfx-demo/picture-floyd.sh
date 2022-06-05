#!/bin/bash

# Usage: ./picture-floyd.sh /dev/hidraw3 picture.jpg

#convert $2 -resize 126 -crop 126x32+0+0 -depth 8 -dither FloydSteinberg -remap pattern:gray50 gray:temp.bin

convert $2 -resize 126 -gravity center -extent 126x32 -depth 8 -dither FloydSteinberg -remap pattern:gray50 gray:temp.bin
sudo ./kbdgfx $1 ./temp.bin

