#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include "config.h"
#include "FSBrowser.h"

WiFiUDP UDP;
//unsigned int multicastPort = 1900;  // local port to listen on
//IPAddress multicastIP(239,255,255,250);
char buf[4096];
char resp[4096];

void startPairing() {
    sprintf(resp, "HTTP/1.1 200 OK\r\nCache-Control: max-age=100\r\n" \
    "EXT: SERVER: NodeMCU/Lua5.1.4 UPnP/1.1\r\n%s.1\r\n" \
    "ST: upnp:rootdevice\r\nUSN: \r\ESP8266-%li:%s\r\nLocation: http://%s:%d/%s\r\n\r\n", devName, ESP.getChipId(), WiFi.localIP(), devPort, devProfile);
 
    LogIt(resp);

    UDP.begin(multicastPort);
    UDP.beginMulticast(WiFi.localIP(), multicastIP, multicastPort);
    //Serial.printf("Now listening at IP %s and %s, UDP port %d\n", WiFi.localIP().toString().c_str(), multicastIP.toString().c_str(), multicastPort);
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
