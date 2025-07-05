
#include <Arduino.h>

const int SensorDataPin = A0;
String lastWater;

void waterSetup() {
  pinMode(SensorDataPin, INPUT_PULLUP);
  lastWater = "dry";
}

String getWater() {
  int sensor = analogRead(SensorDataPin);
  //Serial.println(sensor);
  //set lastWater "wet" or "dry"
  lastWater = "dry";
  if( sensor > 256 )
    lastWater = "wet";
  return(lastWater);
}
