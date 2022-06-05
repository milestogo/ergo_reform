#!/bin/bash

printf " " > /tmp/oled

# clip
i=0
while IFS= read -r line
do
	i=$((i+1))
	if [ $i -lt 5 ]
	then
	printf "%21.21s" "$line" >> /tmp/oled
	fi
done 

# fold -w21 >> /tmp/oled

printf "%63.63s" " " >> /tmp/oled

cat /tmp/oled

