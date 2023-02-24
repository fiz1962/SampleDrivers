# Sample Edge Driver for ESP8266 Switch

Install driver.  See installDriver.sh for example.

To test with python device: Run both device.py and discovery.py on some server.  Then add device using mobile app.  Discovery.py should recognize the scan, respond and exit leaving device.py to run as the device.

The Arduino (ESP8266) device can be written using the Arduino IDE.  Then go to http://{esp8266_IP}:8191/pair and add add device using the mobile app.