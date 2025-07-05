
#include <Arduino.h>

const int SensorDataPin = D4;
String lastWater;

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
