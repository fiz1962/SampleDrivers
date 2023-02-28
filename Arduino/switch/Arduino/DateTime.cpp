#include <WiFiUdp.h>
#include <NTPClient.h>
#include "time.h"

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

void startDateTime() {
  timeClient.begin();  
  timeClient.setTimeOffset(0);
  timeClient.update();
  delay(1000); //give ntp a bit of time to get started
}

String getDateTime() {
  timeClient.update();
  unsigned long epochTime = timeClient.getEpochTime();
  struct tm *ptm = gmtime ((time_t *)&epochTime); 
  int monthDay     = ptm->tm_mday;
  int currentMonth = ptm->tm_mon+1;
  int currentYear = ptm->tm_year+1900;

  String t = String(currentMonth) + "/" + String(monthDay) + "/" + String(currentYear) + " " + timeClient.getFormattedTime();
  Serial.println(t);
  
  return t;
}