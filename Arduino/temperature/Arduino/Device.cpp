#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

#include "config.h"

void startPairing();
void loopPair();
void handlePair();

void tempSetup();
float getTemp();
void LogIt(String msg, bool filter=true);

ESP8266WebServer server(devPort);
String STServer;
String STPort;
String STUUID;
int lastState;

void handleRoot();
void handleNotFound();
void handleRefresh();
void handlePair();
void handleProfile();
void handlePing();

void startDevice() {
  tempSetup();
  pinMode(D7, INPUT_PULLUP);

  STServer  = "";
  STPort    = "";
  STUUID    = "";
  lastState = -1;

  server.on("/", handleRoot);               // Call the 'handleRoot' function when a client requests URI "/"
  server.onNotFound(handleNotFound);        // When a client requests an unknown URI (i.e. something other than "/"), call function "handleNotFound"

  server.on("/refresh", handleRefresh);
  server.on("/pair",     handlePair);
  server.on("/ping",     handlePing);
  
  char prof[1024];
  sprintf(prof, "/%s", devProfile);
  server.on(prof, handleProfile);
  
  server.begin();                           // Actually start the server
  Serial.println("HTTP server started");
}

void loopDevice() {
  server.handleClient();
  loopPair();
  if( digitalRead(D7)!= lastState ) {
    lastState = digitalRead(D7);
    if( STServer != "" ) {
      HTTPClient http;
      WiFiClient client;
      String cl = "http://"+STServer+":"+STPort+"/push-state";
      http.begin(client, cl);
      http.addHeader("Content-Type", "application/json");
      char buf[1024];
      
      int httpResponseCode;
      if( lastState == 1 ) {
          sprintf(buf, "{\"uuid\":\"%s\",\"contact\":\"open\"}", STUUID.c_str());
          httpResponseCode = http.POST(buf);
          LogIt("Door opened");
          LogIt(cl.c_str());          
          LogIt(buf);
      }
      else
      {
          httpResponseCode = http.POST("{\"uuid\":\"" + STUUID + "\",\n\"contact\":\"closed\"}");
          LogIt("Door closed");
      }

    }
  }

}

void handleRefresh() {
  
  char ref[512];
  char contactState[16];
  sprintf(contactState, "closed");
  if( digitalRead(D7)==1 )
      sprintf(contactState, "open");

  sprintf(ref,"{\r\n" \
              "\"status\":\"HTTP/1.1 200 OK\",\r\n" \
              "\"Content-Type\":\"application/json\",\r\n" \
              "\"Cache-Control\":\"no-cache, private\",\r\n" \
              "\"temperature\":\"%f\",\r\n" \
              "\"contact\":\"%s\"\r\n" \
              "}\r\n", getTemp(), contactState);

  LogIt(ref);
  server.send(200, "text/plain", ref);   // Send HTTP status 200 (Ok) and send some text to the browser/client
}

void handlePing() {
  STServer = server.arg("ip");
  STPort   = server.arg("port");
  STUUID   = server.arg("ext_uuid");

  server.send(200, "text/plain", "Ping");
}

void handlePair() {
  startPairing();
  server.send(200, "text/plain", "Pairing started");
}

void handleProfile() {
  char resp[1024];
  sprintf(resp, "<root xmlns=\"urn:schemas-upnp-org:device-1-0\" configId=\"1\">\r\n" \
                "<specVersion>\r\n" \
                "<major>2</major>\r\n" \
                "<minor>0</minor>\r\n" \
                "</specVersion>\r\n" \
                "<device>\r\n" \
                "<deviceType>urn:SmartThingsCommunity:device:Light:1</deviceType>\r\n" \
                "<presentationURL>/</presentationURL>\r\n" \
                "<friendlyName>TemperatureESP8266</friendlyName>\r\n" \
                "<manufacturer>SmartThingsCommunity</manufacturer>\r\n" \
                "<manufacturerURL>https://community.smartthings.com</manufacturerURL>\r\n" \
                "<modelName>LAN Temperature</modelName>\r\n" \
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