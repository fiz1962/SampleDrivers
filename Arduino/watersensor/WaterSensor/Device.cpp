#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

#include "config.h"

void startPairing();
void loopPair();
void handlePair();

void waterSetup();
String getWater();

ESP8266WebServer serverDevice(devPort);
String STServer;
String STPort;
String STUUID;
String lastState;

void handleRoot();
void handleNotFound2();
void handleRefresh();
void handlePair();
void handleProfile();
void handlePing();
void LogIt(String msg);

void startDevice() {
  LogIt("Starting device");
  waterSetup();
  pinMode(D7, INPUT_PULLUP);

  STServer  = "";
  STPort    = "";
  STUUID    = "";
  lastState = "dry";

  serverDevice.on("/", handleRoot);               // Call the 'handleRoot' function when a client requests URI "/"
  serverDevice.onNotFound(handleNotFound2);        // When a client requests an unknown URI (i.e. something other than "/"), call function "handleNotFound"

  serverDevice.on("/refresh", handleRefresh);
  serverDevice.on("/pair",     handlePair);
  serverDevice.on("/ping",     handlePing);
  
  char prof[1024];
  sprintf(prof, "/%s", devProfile);
  serverDevice.on(prof, handleProfile);
  
  serverDevice.begin();                           // Actually start the server
  Serial.println("HTTP server started");
}

void loopDevice() {
  serverDevice.handleClient();
  loopPair();
  String thisState = getWater();
  if( thisState != lastState ) {
    lastState = thisState;
    if( STServer != "" ) {
      HTTPClient http;
      WiFiClient client;
      http.begin(client, "http://"+STServer+":"+STPort+"/push-state");
      http.addHeader("Content-Type", "application/json");
      
      int httpResponseCode;
      if( lastState == "wet" )
          httpResponseCode = http.POST("{\"uuid\":\"" + STUUID + "\",\n\"water\":\"wet\"}");
      else
          httpResponseCode = http.POST("{\"uuid\":\"" + STUUID + "\",\n\"water\":\"dry\"}");

    }
  }

}

void handleRefresh() {
  
  char ref[512];

  sprintf(ref,"{\r\n" \
              "\"status\":\"HTTP/1.1 200 OK\",\r\n" \
              "\"Content-Type\":\"application/json\",\r\n" \
              "\"Cache-Control\":\"no-cache, private\",\r\n" \
              "\"water\":\"%s\"\r\n" \
              "}\r\n", getWater());

  LogIt(String(ref));
  serverDevice.send(200, "text/plain", ref);   // Send HTTP status 200 (Ok) and send some text to the browser/client
}

void handlePing() {
  STServer = serverDevice.arg("ip");
  STPort   = serverDevice.arg("port");
  STUUID   = serverDevice.arg("ext_uuid");

  serverDevice.send(200, "text/plain", "Ping");
}

void handlePair() {
  startPairing();
  serverDevice.send(200, "text/plain", "Pairing started");
}

void handleProfile() {
  char resp[1024];
  sprintf(resp, "<root xmlns=\"urn:schemas-upnp-org:device-1-0\" configId=\"1\">\r\n" \
                "<specVersion>\r\n" \
                "<major>2</major>\r\n" \
                "<minor>0</minor>\r\n" \
                "</specVersion>\r\n" \
                "<device>\r\n" \
                "<deviceType>urn:SmartThingsCommunity:device:WaterSensor:1</deviceType>\r\n" \
                "<presentationURL>/</presentationURL>\r\n" \
                "<friendlyName>WaterSensorESP8266</friendlyName>\r\n" \
                "<manufacturer>SmartThingsCommunity</manufacturer>\r\n" \
                "<manufacturerURL>https://community.smartthings.com</manufacturerURL>\r\n" \
                "<modelName>LAN Water</modelName>\r\n" \
                "<serialNumber>SN-ESP8266-899</serialNumber>\r\n" \
                "<UDN>uuid:9487da-SN-ESP8266-899</UDN>\r\n" \
                "</device></root>\r\n\r\n"); 
   serverDevice.send(200, "text/plain", resp);   // Send HTTP status 200 (Ok) and send some text to the browser/client
}

void handleRoot() {
  serverDevice.send(200, "text/plain", "Hello world!");   // Send HTTP status 200 (Ok) and send some text to the browser/client
}

void handleNotFound2(){
  serverDevice.send(404, "text/plain", "404: Not found"); // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
}