#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include "config.h"

WiFiUDP UDP;
char buf[2048];
char resp[2048];
void LogIt(String msg, bool filter=true);

void startPairing() {
  sprintf(resp, "HTTP/1.1 200 OK\r\nCache-Control: max-age=100\r\n" \
                "EXT: SERVER: NodeMCU/Lua5.1.4 UPnP/1.1\r\n%s\r\n" \
                "ST: upnp:rootdevice\r\nUSN: %s\r\nLocation: http://%s:%d/%s \r\n\r\n", \
                devName, "1535", WiFi.localIP().toString().c_str(), devPort, devProfile);

    LogIt(resp);

    UDP.begin(multicastPort);
    UDP.beginMulticast(WiFi.localIP(), multicastIP, multicastPort);
}

void stopParing() {
    UDP.stop();
    Serial.println("Discovery stopped");
}

void loopPair() {
    int packetSize = UDP.parsePacket();
    if (packetSize) {
      int len = UDP.read(buf, 4096);
      if( len > 0 ){
        Serial.println(buf);
        if( strstr(buf, devName) ) {
            Serial.println("Response");
            Serial.println(UDP.remoteIP());
            Serial.println(UDP.remotePort());
            Serial.println(resp);
            UDP.beginPacket(UDP.remoteIP(), UDP.remotePort());
            UDP.write(resp);
            UDP.endPacket();
            stopParing();
        }
      }
    }
}
