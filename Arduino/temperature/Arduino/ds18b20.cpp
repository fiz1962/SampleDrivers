#include <OneWire.h>
#include <DallasTemperature.h>

const int SensorDataPin = D4;     
OneWire oneWire(SensorDataPin);
DallasTemperature sensors(&oneWire);

void tempSetup() {
    sensors.begin();
    pinMode(SensorDataPin, INPUT_PULLUP);
}

float getTemp() {
  sensors.requestTemperatures(); 
  return(sensors.getTempFByIndex(0));
}