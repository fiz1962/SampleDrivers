#!/bin/bash

./smartthings edge:drivers:package lightbulb-lan-esp8266/driver 
./smartthings edge:channels:create
./smartthings edge:channels:assign
./smartthings edge:channels:enroll
./smartthings edge:drivers:install
