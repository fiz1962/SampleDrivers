#include <ESP8266WiFi.h>
#include "FSBrowser.h"

const char* ssid = "*****";
const char* password = "*****";
IPAddress local_IP(192, 168, 1, 100);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(8, 8, 8, 8);

void startDevice();
void loopDevice();
void loopOTA();

void startOTA();
void setServer();

void setup() {
    Serial.begin(115200);
    Serial.println();
  
    if (!WiFi.config(local_IP, gateway, subnet, primaryDNS)) {
      Serial.println("STA Failed to configure");
    }

    Serial.printf("Connecting to %s ", ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
    delay(500);
    Serial.print(".");
    }
    Serial.println("connected");
    Serial.println(WiFi.localIP());


  startOTA();
  startDevice();
  setServer();

  LogIt("Started");
}

void loop() {
  loopOTA();
  loopDevice();
}
