#Incomplete work

This firwmare does not yet work in the mnt reform.
The keymap and OLED graphics should work. 
The serial connection has not been tested. 
The problem is that QMK expects USB power at boot. Looking at similar examples in keyboards/bioi/main.c , we probably need to create a custom main.c
The order would be 
1) init everything. 
2) open serial, find the LPC. 
3) Wait for power on, talk to LPC, exit power saving. 
4) Wait for USB VBUS
5) Start USB, 
6) standard QMK boot cycle. 

To use, copy or symlink this directory into the $QMK/keyboards/ directory
