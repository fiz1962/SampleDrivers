#!/bin/bash

./esptool-master/esptool.py --port /dev/ttyUSB0 erase_flash
./esptool-master/esptool.py --port /dev/ttyUSB0 write_flash 0x00000 nodemcu-release-7-modules-2022-12-03-19-09-06-float.bin 
nodemcu-tool reset
sleep 10
nodemcu-tool upload app/src/*
