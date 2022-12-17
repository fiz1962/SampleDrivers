#include <ESP8266WiFi.h>


const char* ssid = "-----";
const char* password = "-----";

void startDevice();
void loopDevice();

void setup() {
    Serial.begin(115200);
    Serial.println();

    Serial.printf("Connecting to %s ", ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
    delay(500);
    Serial.print(".");
    }
    Serial.println("connected");
    Serial.println(WiFi.localIP());

    startDevice();
}

void loop() {
  loopDevice();
}
