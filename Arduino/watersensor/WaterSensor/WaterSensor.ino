#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESP8266WiFiMulti.h>

ESP8266WiFiMulti wifiMulti;

void startDevice();
void loopDevice();
void loopOTA();
//void loopFS();
void startOTA();
void setServer();
void startFSBrowser();
void loopServer();
void startNTP();
void LogIt(String msg);
void startWebExtras();

void setup() {
    WiFi.persistent(false);
    IPAddress local_IP(192, 168, 1, 228);
    IPAddress gateway(192, 168, 1, 1);
    IPAddress subnet(255, 255, 255, 0);

    wifiMulti.addAP("****", "****");
    wifiMulti.addAP("****", "****");

    Serial.begin(115200);
    Serial.println();

    if (!WiFi.config(local_IP, gateway, subnet)) {
      Serial.println("STA Failed to configure");
    }

    dns_setserver(0, IPAddress(8,8,8,8));
    dns_setserver(1, IPAddress(8,8,4,4));

    Serial.println("Connecting");
    //WiFi.begin(ssid, password);
    while (wifiMulti.run(5000) != WL_CONNECTED)
    {
    delay(500);
    Serial.print(".");
    }

    Serial.println("connected");
    Serial.println(WiFi.localIP());

  if (!MDNS.begin("WetOMatic")) {
    //Serial.println("Error setting up MDNS responder!");
    //while (1) { delay(1000); }
  }

    //startOTA();
    startDevice();
    //setServer();
    startFSBrowser();
    startWebExtras();
    startOTA();
    startNTP();
    LogIt("Connected to "+WiFi.SSID());
}

void loop() {
  if( wifiMulti.run(5000) == WL_CONNECTED ) {
  //  LogIt("Wifi reconnected to " + WiFi.SSID());
  }
  loopOTA();
  loopDevice();
  loopServer();
  MDNS.update();
}
