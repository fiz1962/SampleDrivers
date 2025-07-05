#include <Arduino.h>
#include <time.h> 

#define MyTZ "CST6CDT,M3.2.0,M11.1.0"
const char* ntpServer = "pool.ntp.org";

void startNTP() {
  configTime(MyTZ, ntpServer);
}

String getDateTime() {
  char result[128];
  time_t epochTime = time (NULL);
   // Converting current time to local time
  struct tm *loc_time = localtime (&epochTime);

  snprintf(result, 128, asctime (loc_time));

  return result;
}
