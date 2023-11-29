#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include "config.h"

WiFiUDP UDP;


void LogIt(String msg, bool filter=true);

void startPairing() {


    UDP.begin(multicastPort);
    UDP.beginMulticast(WiFi.localIP(), multicastIP, multicastPort);
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
                "EXT: SERVER: NodeMCU/Lua5.1.4 UPnP/1.1\r\n%s\r\n" \
                "ST: upnp:rootdevice\r\nUSN: %s\r\nLocation: http://%s:%d/%s \r\n\r\n", \
                devName, "1535", WiFi.localIP().toString().c_str(), devPort, devProfile);

            UDP.beginPacket(UDP.remoteIP(), UDP.remotePort());
            UDP.write(buf);
            UDP.endPacket();
            stopParing();
        }
      }
    }
}
