#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <CmdMessenger.h>

#define MAX_TEMP 60.0f
#define MIN_TEMP 0.0f

#define NUM_LED 12
#define LED_PIN 2

#define CYCLE 1000

CmdMessenger cmdMessenger = CmdMessenger(Serial);
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LED, LED_PIN, NEO_GRB + NEO_KHZ800);
int blink = 0;
int warning = 0;

enum
{
  kSetLED,
};

void warning_blink()
{
  if (warning < 0 && blink >= CYCLE / 2)
    {
      for(int j=0; j < NUM_LED; j++) {
	strip.setPixelColor(j, strip.Color(0, 0, 255));
      }
    }
  else if (warning > 0 && blink >= CYCLE / 2)
    {
      for(int j=0; j < NUM_LED; j++) {
	strip.setPixelColor(j, strip.Color(255, 0, 0));
      }
    }
  else if (warning != 0 && blink < CYCLE / 2)
    {
      for(int j=0; j < NUM_LED; j++) {
	strip.setPixelColor(j, strip.Color(0, 0, 0));
      }
    }
  blink = (blink == CYCLE) ? 0 : blink + 1;
  strip.show();
}

void warning_temp(double temp)
{
  warning = (temp <= MIN_TEMP) ? -1 : 1;
}

void setLed(double temp)
{
  char rgb[3];
  rgb[1] = 0;
  if (temp <= MIN_TEMP || temp >= MAX_TEMP)
    warning_temp(temp);
  else
    {
      warning = 0;
      rgb[2] = 255 - (temp / MAX_TEMP * 255);
      rgb[0] = 255 * (temp / MAX_TEMP * 255);
      double nbr_led = (temp / MAX_TEMP) * NUM_LED;
      for(int j=0; j < NUM_LED; j++) {
	strip.setPixelColor(j, strip.Color(0, 0, 0));
      }
      for(int i=0; i < nbr_led; i++) {
	strip.setPixelColor(i, strip.Color(rgb[0], rgb[1], rgb[2]));
      }
      strip.show();
    }
}

void OnSetLed()
{
  Serial.println("OnTemperure");
  double temp = cmdMessenger.readDoubleArg();
  setLed(temp);
  Serial.print("temperature : ");
  Serial.println(temp);
}
