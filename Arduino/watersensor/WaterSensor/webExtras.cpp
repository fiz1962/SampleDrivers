//#include "headers.h" 
#include <LittleFS.h>
#include <ESP8266WebServer.h>

ESP8266WebServer* getServer();
bool authServer();
String getDateTime();
String getWater();

void startWebExtras() {

}