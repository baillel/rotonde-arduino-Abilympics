#include <Arduino.h>
#include <CmdMessenger.h>

#define MAX_RESISTANCE_VALUE (1023.0f)
#define MIN_VOLTAGE_SCALE (0.05f)
#define NO_PRESSURE (0.0f)

enum {
  kCheckPressure,
};

double last_send = 0.0;

void checkPressure() {
  int sensorValue = analogRead(A0);
  double voltage = sensorValue / MAX_RESISTANCE_VALUE;
  if ((fabs(voltage - last_send) >= MIN_VOLTAGE_SCALE) ||
      (voltage == NO_PRESSURE && last_send != NO_PRESSURE)) {
      cmdMessenger.sendCmd((int)kCheckPressure, voltage);
      last_send = voltage;
    }
}

