#include <Arduino.h>
#include <CmdMessenger.h>

#define TIME_VIB (500)
#define VIB_PIN (3)

enum {
  kSetVibration,
};

unsigned int time = 0;

void onVibrate() {
  time = TIME_VIB;
}

void vibrate() {
  if (time > 0)
    {
      digitalWrite(VIB_PIN, HIGH);
      --time;
      return ;
    }
  else
    digitalWrite(VIB_PIN, LOW);
}
