//#include <OneWire.h>
//#include <DallasTemperature.h>
#include <Arduino.h>

const int SensorDataPin = D4;
String lastWater;

//OneWire oneWire(SensorDataPin);
//DallasTemperature sensors(&oneWire);
void makePlot(double newT);

void waterSetup() {
  lastWater = "dry";
}

String getWater() {
  if( lastWater == "dry" )
      lastWater = "wet";
  else
      lastWater = "dry";

  return(lastWater);
}