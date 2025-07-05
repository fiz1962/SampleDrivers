#include <Arduino.h>
#include <LittleFS.h>

String getDateTime();

void LogIt(String msg) {
  if( !LittleFS.exists("/logs/") )
     LittleFS.mkdir("/logs/");

    File f = LittleFS.open("/logs/log.txt", "a");
  if (f) {
    if( f.size() > 20*1024 ) {
      f.close();
      LittleFS.remove("/logs/log2.txt");
      LittleFS.rename("/logs/log.txt", "/logs/log2.txt");
      f = LittleFS.open("/logs/log.txt", "a");
    } 

    String dt = getDateTime();
    dt.trim();
    f.print(dt);
    f.print(":");
    f.println(msg);
    f.close();
  }
}