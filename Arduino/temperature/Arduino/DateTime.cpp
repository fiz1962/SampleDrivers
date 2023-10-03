#include <WiFiUdp.h>
#include <NTPClient.h>
#include "time.h"

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");
void LogIt(String msg, bool filter=true);

void startDateTime() {
  timeClient.begin();  
  timeClient.setTimeOffset(-5*3600);
  timeClient.update();
  delay(1000); //give ntp a bit of time to get started
}

char* getDateTime() {
  //timeClient.update();
  time_t epochTime = timeClient.getEpochTime();
  char* strtime = asctime(localtime(&epochTime));

  return strtime;
}