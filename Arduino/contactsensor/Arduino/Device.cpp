#include <ESP8266WebServer.h>


#include "config.h"

void startPairing();
void loopPair();
void handlePair();

void tempSetup();
float getTemp();

ESP8266WebServer server(devPort);
void handleRoot();
void handleNotFound();
void handleRefresh();
void handlePair();
void handleProfile();

void startDevice() {
  tempSetup();

  server.on("/", handleRoot);               // Call the 'handleRoot' function when a client requests URI "/"
  server.onNotFound(handleNotFound);        // When a client requests an unknown URI (i.e. something other than "/"), call function "handleNotFound"

  server.on("/refresh", handleRefresh);
  server.on("/pair",     handlePair);
  
  char prof[1024];
  sprintf(prof, "/%s", devProfile);
  server.on(prof, handleProfile);
  
  server.begin();                           // Actually start the server
  Serial.println("HTTP server started");
}

void loopDevice() {
  server.handleClient();
  loopPair();
}

char* getContact() {
    char* contact = "closed";
    return(contact);
}

void handleRefresh() {
  
  char ref[1024];
  sprintf(ref,"{\r\n" \
              "\"status\":\"HTTP/1.1 200 OK\",\r\n" \
              "\"Content-Type\":\"application/json\",\r\n" \
              "\"Cache-Control\":\"no-cache, private\",\r\n" \
              "\"contact\":\"%s\"\r\n" \
              "}\r\n", getContact());
  Serial.println("Refresh");
  Serial.println(ref);
  server.send(200, "text/plain", ref);   // Send HTTP status 200 (Ok) and send some text to the browser/client
}

void handlePair() {
  startPairing();
  server.send(200, "text/plain", "Pairing started");
}

void handleProfile() {
  char resp[2048];
  sprintf(resp, "<root xmlns=\"urn:schemas-upnp-org:device-1-0\" configId=\"1\">\r\n" \
                "<specVersion>\r\n" \
                "<major>2</major>\r\n" \
                "<minor>0</minor>\r\n" \
                "</specVersion>\r\n" \
                "<device>\r\n" \
                "<deviceType>urn:SmartThingsCommunity:device:Light:1</deviceType>\r\n" \
                "<presentationURL>/</presentationURL>\r\n" \
                "<friendlyName>ContactSensorESP8266</friendlyName>\r\n" \
                "<manufacturer>SmartThingsCommunity</manufacturer>\r\n" \
                "<manufacturerURL>https://community.smartthings.com</manufacturerURL>\r\n" \
                "<modelName>LAN contactSensor</modelName>\r\n" \
                "<serialNumber>SN-ESP8266-899</serialNumber>\r\n" \
                "<UDN>uuid:9487da-SN-ESP8266-899</UDN>\r\n" \
                "</device></root>\r\n\r\n"); 
   server.send(200, "text/plain", resp);   // Send HTTP status 200 (Ok) and send some text to the browser/client
}

void handleRoot() {
  server.send(200, "text/plain", "Hello world!");   // Send HTTP status 200 (Ok) and send some text to the browser/client
}

void handleNotFound(){
  server.send(404, "text/plain", "404: Not found"); // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
}
