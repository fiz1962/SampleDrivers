#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include "config.h"

WiFiUDP UDP;
//unsigned int multicastPort = 1900;  // local port to listen on
//IPAddress multicastIP(239,255,255,250);

void startPairing() {
    UDP.begin(multicastPort);
    UDP.beginMulticast(WiFi.localIP(), multicastIP, multicastPort);
    Serial.printf("Now listening at IP %s and %s, UDP port %d\n", WiFi.localIP().toString().c_str(), multicastIP.toString().c_str(), multicastPort);
}

void stopParing() {
    UDP.stop();
    Serial.println("Discovery stopped");
}

void loopPair() {
  char buf[2048];

    int packetSize = UDP.parsePacket();
    if (packetSize) {
      int len = UDP.read(buf, 4096);
      if( len > 0 ){
        Serial.println(buf);
        if( strstr(buf, devName) ) {
            sprintf(buf, "HTTP/1.1 200 OK\r\nCache-Control: max-age=100\r\n" \
                "EXT: SERVER: NodeMCU/Lua5.1.4 UPnP/1.1\r\n%s.1\r\n" \
                 "ST: upnp:rootdevice\r\nUSN: \r\nuuid:%s\r\nLocation: http://%s:%d/%s\r\n\r\n", devName, uuid, WiFi.localIP().toString().c_str(), devPort, devProfile);
 
            Serial.println("Response");
            Serial.println(UDP.remoteIP());
            Serial.println(UDP.remotePort());
            Serial.println(buf);
            UDP.beginPacket(UDP.remoteIP(), UDP.remotePort());
            UDP.write(buf);
            UDP.endPacket();
            stopParing();
        }
      }
    }
}
