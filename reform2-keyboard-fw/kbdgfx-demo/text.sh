#!/bin/bash

# Usage: ./text.sh /dev/hidraw3 18 "hello world"

convert -size 126x32 -background black -font Inter-Bold.ttf -pointsize $2 -fill white -gravity center caption:"$3" -depth 8 -flatten gray:temp.bin
sudo ./kbdgfx $1 ./temp.bin

