#!/bin/bash

../smartthings edge:drivers:package Arduino/temperature/driver
../smartthings edge:channels:create
../smartthings edge:channels:assign
../smartthings edge:channels:enroll
../smartthings edge:drivers:install
