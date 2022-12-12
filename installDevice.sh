#!/bin/bash

./esptool-master/esptool.py --port /dev/ttyUSB0 erase_flash
./esptool-master/esptool.py --port /dev/ttyUSB0 write_flash 0x00000 nodemcu-release-9-modules-2022-12-11-23-09-24-float.bin
nodemcu-tool reset
sleep 10
nodemcu-tool upload lightbulb-lan-esp8266/app/src/*
