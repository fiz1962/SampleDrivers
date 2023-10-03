#include <ESP8266WebServer.h>

#include "config.h"

void startPairing();
void loopPair();
bool pairMode;

void tempSetup();
float getTemp();

void LogIt(String msg, bool filter=true);

ESP8266WebServer server(devPort);
void handleRoot();
void handlereBoot();
void handleNotFound();
void handleRefresh();
void handlePair();
void handleProfile();

void startDevice() {
  pairMode = false;
  tempSetup();

  server.on("/", handleRoot);               // Call the 'handleRoot' function when a client requests URI "/"
  server.onNotFound(handleNotFound);        // When a client requests an unknown URI (i.e. something other than "/"), call function "handleNotFound"

  server.on("/refresh", handleRefresh);
  server.on("/pair",     handlePair);
  
  char prof[64];
  sprintf(prof, "/%s", devProfile);
  server.on(prof, handleProfile);

  server.begin();                           // Actually start the server
  Serial.println("HTTP server started");
}

void loopDevice() {
  server.handleClient();
  if( pairMode )
      loopPair();
}

void handleRefresh() {
  
  char ref[1024];
  sprintf(ref,"{\r\n" \
              "\"status\":\"HTTP/1.1 200 OK\",\r\n" \
              "\"Content-Type\":\"application/json\",\r\n" \
              "\"Cache-Control\":\"no-cache, private\",\r\n" \
              "\"temperature\":\"%f\",\r\n" \
              "\"TemperatureUnit\":\"F\"\r\n"
              "}\r\n", getTemp());
  Serial.println("Refresh");
  LogIt(ref);
  server.send(200, "text/plain", ref);   // Send HTTP status 200 (Ok) and send some text to the browser/client
}

void handleProfile() {
  char resp[2048];
  pairMode = false;
  //sprintf(resp, "Temperature.xml");
  //LogIt("Handle Profile");
  sprintf(resp, "<root xmlns=\"urn:schemas-upnp-org:device-1-0\" configId=\"1\">\r\n" \
                "<specVersion>\r\n" \
                "<major>2</major>\r\n" \
                "<minor>0</minor>\r\n" \
                "</specVersion>\r\n" \
                "<device>\r\n" \
                "<deviceType>urn:SmartThingsCommunity:device:TemperatureESP8266:1</deviceType>\r\n" \
                "<presentationURL>/</presentationURL>\r\n" \
                "<friendlyName>TemperatureESP8266</friendlyName>\r\n" \
                "<manufacturer>SmartThingsCommunity</manufacturer>\r\n" \
                "<manufacturerURL>https://community.smartthings.com</manufacturerURL>\r\n" \
                "<modelName>LAN Temperature</modelName>\r\n" \
                "<serialNumber>SN-%s</serialNumber>\r\n" \
                "<UDN>uuid:SN-%s</UDN>\r\n" \
                "</device></root>\r\n\r\n", WiFi.hostname().c_str(), WiFi.hostname().c_str());
  LogIt(resp);
  server.send(200, "text/plain", resp);   // Send HTTP status 200 (Ok) and send some text to the browser/client
}

void handlePair() {
  pairMode = true;
  startPairing();
  server.send(200, "text/plain", "Pairing started");
}

void handleRoot() {
  server.send(200, "text/plain", "Ok");   // Send HTTP status 200 (Ok) and send some text to the browser/client
}

void handleNotFound(){
  LogIt("Not found");
  LogIt(server.uri());
  server.send(404, "text/plain", "404: Not found"); // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
}