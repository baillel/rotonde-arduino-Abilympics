#include <Arduino.h>
#include <CmdMessenger.h>

#define MAX_RESISTANCE_VALUE (1023.0f)
#define OUT_VOLTAGE_BOARD (5.0f)

#define MIN_TEMPERATURE_SCALE (0.5f)

#define MILLI (1000.0f)

#define SAMPLE (30)

enum {
  kCheckTemperature,
};

double last_temperature;
double sample_temp[SAMPLE];
double med_temp;
int count = 0;

double getTemperature(void) {
  int sensorValue = analogRead(A1);
  double voltage = sensorValue / MAX_RESISTANCE_VALUE * OUT_VOLTAGE_BOARD;
  return ((voltage * MILLI - 500) / 10);
}

void calculMedian() {
  med_temp = 0;
  for (int i = 0; i < SAMPLE; i++)
    med_temp += sample_temp[i];
  med_temp /= SAMPLE;
}

void checkTemperature() {
  double temperature = getTemperature();
  sample_temp[count] = temperature;
  count++;
  if (count == SAMPLE) {
      calculMedian();
      if (fabs(med_temp - last_temperature) > MIN_TEMPERATURE_SCALE) {
	  cmdMessenger.sendCmdStart((int)kCheckTemperature);
	  cmdMessenger.sendCmdArg(med_temp);
	  cmdMessenger.sendCmdEnd();
	  last_temperature = med_temp;
	}
      count = 0;
    }
}
