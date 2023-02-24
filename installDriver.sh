#!/bin/bash

../smartthings edge:drivers:package "Arduino/switch/driver"
../smartthings edge:channels:create
../smartthings edge:channels:assign
../smartthings edge:channels:enroll
../smartthings edge:drivers:install
