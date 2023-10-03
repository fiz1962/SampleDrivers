#ifndef __FSBROWSER__
#define __FSBROWSER__

#include <ESP8266WiFi.h>

#include <string>
#include <FS.h>
#include <LittleFS.h>
#include <ESPAsyncWebServer.h>

void startDateTime();
char* getDateTime();

AsyncWebServer fileServer(8080);
String lastLogs[8];
int LEDOnOff;

void LogIt(String msg, bool filter=true) {
    File f = LittleFS.open("/f.txt", "a");
  if (f) {
    if( f.size() > 20*1024 ) {
      f.close();
      LittleFS.remove("/f2.txt");
      LittleFS.rename("/f.txt", "/f2.txt");
      f = LittleFS.open("/f.txt", "a");
    }
    
  bool updated = false;
  for(int n=0; n<8; n++) {
    if( lastLogs[n] == msg )
        return;

    if( msg.substring(0,5) == lastLogs[n].substring(0,5) ){
        lastLogs[n] = msg;
        updated = true;
    }
  }

    if( !updated ) {
      for(int n=1; n<8; n++)
          lastLogs[n-1] = lastLogs[n];
      lastLogs[7] = msg;
    }
    

    f.print(getDateTime());
    f.println(msg);
    f.close();
  }
}

void handleFileList(AsyncWebServerRequest *request) {
    String path = "";
    if (request->hasParam("path")) {
      path = request->getParam("path")->value();
    } else {
      path = "/";
    }  

  Dir dir = LittleFS.openDir(path);
    //Serial.println(path);
  
  String output = "";
  while (dir.next()) {
    output += dir.fileSize();
    output += "\t<a href=\"/cat?file=" + dir.fileName() + "\">" + dir.fileName() +"</a></br>\r\n";
  }
  Serial.print(output);
  
  request->send(200, "text/html", output);
}

void handleFileCat(AsyncWebServerRequest *request) {
    String fParam;
    if( LEDOnOff == 1)
        LEDOnOff = 0;
        else
        LEDOnOff = 1;
        
      digitalWrite(2, LEDOnOff);
    if (request->hasParam("file")) {
      fParam = request->getParam("file")->value();
    } else {
      fParam = "./f.txt";
    }

  request->send(LittleFS, fParam, "text/plain");
}

void handleReboot(AsyncWebServerRequest *request) {
    ESP.restart();
    request->send(202, "text/plain", "Reboot");
}

void handleFileDel(AsyncWebServerRequest *request) {
    String fParam;
    String response = "File not found";
    if (request->hasParam("file")) {
      fParam = request->getParam("file")->value();
      LittleFS.remove(fParam);
      response = "OK";
    }

  request->send(202, "text/plain", response);
}

void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}

void setServer() {
  LEDOnOff = 1;
  pinMode(2, OUTPUT);
  digitalWrite(2, LEDOnOff);
  fileServer.on("/list",  HTTP_GET, [](AsyncWebServerRequest *request) {handleFileList(request);});
  fileServer.on("/cat",   HTTP_GET, [](AsyncWebServerRequest *request) {handleFileCat(request);});
  fileServer.on("/del",   HTTP_GET, [](AsyncWebServerRequest *request) {handleFileDel(request);});
  
  fileServer.onNotFound(notFound);

  LittleFS.begin();
    
  fileServer.serveStatic("/fs", LittleFS, "/");

  startDateTime();
  File f = LittleFS.open("/Version.txt", "w");
  //String formattedTime = getFullFormattedTime()+" ";
  f.println(getDateTime());
  f.println("1.0.5");
  f.println(WiFi.getHostname());
  f.close();

  for(int n=0; n<8; n++)
      lastLogs[n] = "";
      
  fileServer.begin();
}

#endif;