#include <Arduino.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

#define pin D2

void setSwitch(String OnOff);
String getSwitch();

String STServer;
String STPort;
String STUUID;
String lastState;

void setST(String thisServer, String thisPort, String thisUUID) {
  STServer = thisServer;
  STPort = thisPort;
  STUUID = thisUUID;

  String message = "";
  message += "Server:"+STServer;
  message += "\nPort:"+STPort;
  message += "\nUUID:"+STUUID;
  Serial.println(message);
}

void switchSetup() {
  STServer = "";
  STPort = "";
  String STUUID = "";
  lastState = "";

  pinMode(pin, OUTPUT);
}

String getSwitch() {
  if( digitalRead(pin) == HIGH )
    return "on";

  return "off";
}

void setSwitch(String OnOff) {
  if( OnOff == "on" )
    digitalWrite(pin, HIGH);
  else
    digitalWrite(pin, LOW);
}