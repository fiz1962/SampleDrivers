#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>

void startDevice();
void loopDevice();
void loopOTA();
//void loopFS();
void startOTA();
void setServer();
void startFSBrowser();
void loopServer();
void startNTP();

void setup() {
const char* ssid = "***";
const char* password = "***";
IPAddress local_IP(192, 168, 1, 100);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

    Serial.begin(115200);
    Serial.println();

    if (!WiFi.config(local_IP, gateway, subnet)) {
      Serial.println("STA Failed to configure");
    }

    dns_setserver(0, IPAddress(8,8,8,8));
    dns_setserver(1, IPAddress(8,8,4,4));

    Serial.printf("Connecting to %s ", ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
    delay(500);
    Serial.print(".");
    }
    Serial.println("connected");
    Serial.println(WiFi.localIP());

  if (!MDNS.begin("ESP")) {
    //Serial.println("Error setting up MDNS responder!");
    //while (1) { delay(1000); }
  }

    //startOTA();
    startDevice();
    //setServer();
    startFSBrowser();
    startOTA();
    startNTP();
}

void loop() {
  loopOTA();
  loopDevice();
  loopServer();
  MDNS.update();
}
