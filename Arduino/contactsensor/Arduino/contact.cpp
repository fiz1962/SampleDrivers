#include <Arduino.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

#define pin D2

char* getContact();

String STServer;
String STPort;
String STUUID;
String lastState;

void CheckContact() {
  if( lastState != getContact() )
  {
    lastState = getContact();
    Serial.print("State changed to ");
    Serial.println(lastState);
    if( STServer != "" ) {
      HTTPClient http;
      WiFiClient client;
      http.begin(client, "http://"+STServer+":"+STPort+"/push-state");
      http.addHeader("Content-Type", "application/json");
      
      int httpResponseCode = http.POST("{\"uuid\":\"" + STUUID + "\",\n\"contact\":\""+lastState+"\"}");
      Serial.println("http://"+STServer+":"+STPort+"/push-state?value="+lastState);
      Serial.println("{\"uuid\":\"" + STUUID + "\",\n\"contact\":\""+lastState+"\"}");
    }

  }
}

void setST(String thisServer, String thisPort, String thisUUID) {
  STServer = thisServer;
  STPort = thisPort;
  STUUID = thisUUID;

  String message = "";
  message += "Server:"+STServer;
  message += "\nPort:"+STPort;
  message += "\nUUID:"+STUUID;
  Serial.println(message);
}

void contactSetup() {
  STServer = "";
  STPort = "";
  String STUUID = "";
  lastState = "";

  pinMode(pin, INPUT_PULLUP);
}

char* getContact() {
  char* contact;
  if( digitalRead(pin) == LOW )
      contact = (char*)"closed";
    else
      contact = (char*)"open";
  //Serial.println(contact);
  return(contact);
}