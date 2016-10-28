#include "bb_LED.h"

int state = 0;
int shaking = 0;

int xValue = 0;
int yValue = 0;
int zValue = 0;

const int xPin = A0;
const int yPin = A1;
const int zPin = A2;

int readingsX[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int readingsY[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int readingsZ[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int avgX = 0;
int avgY = 0;
int avgZ = 0;

int marker = 0;

int prevstate = 0;

int ledPins[] = {3, 5, 6, 9, 10};
enum {OFF, LED1, LED2, LED3, LED4, LED5};
bb_LED led1(3);
bb_LED led2(5);
bb_LED led3(6);
bb_LED led4(9);
bb_LED led5(10);

unsigned long last_state_change;
void setup() {

  Serial.begin(9600);

  int index;
  for (index = 0; index < 5; index++)
  {
    //pinMode(ledPins[index], OUTPUT);

    //digitalWrite(ledPins[index], LOW);
    Serial.println(ledPins[index]);
  }
  last_state_change = millis();
  state = OFF;
}

void loop() {
  int index;
  //digitalWrite(ledPins[0], HIGH);

  switch (state) {
    case OFF:
      wait(1000);
      if (shaking) {
        state = LED1;
      } else {
        led1.off();
        led2.off();
        led3.off();
        led4.off();
        led5.off();
      }
      break;
    case LED1:
      led1.fadeOn(1024);
      wait(2000);
      if (shaking) {
        state = LED2;
      } else {
        led1.fadeOff(1024);
        state = OFF;
      }
      break;
    case LED2:
      led2.fadeOn(1024);
      wait(2000);
      if (shaking) {
        state = LED3;
      } else {
        led2.fadeOff(1024);
        state = LED1;
      }
      break;
    case LED3:
      led3.fadeOn(1024);
      wait(2000);
      if (shaking) {
        state = LED4;
      } else {
        led3.fadeOff(1024);
        state = LED2;
      }
      break;
    case LED4:
      led4.fadeOn(1024);
      wait(2000);
      if (shaking) {
        state = LED5;
      } else {
        led4.fadeOff(1024);
        state = LED3;
      }
      break;
    case LED5:
      Serial.println("here");
      led5.fadeOn(1024);
      smallWait(5000);
      state = OFF;
      Serial.println("now here");
      led5.fadeOff(1024);
      led4.fadeOff(1024);
      led3.fadeOff(1024);
      led2.fadeOff(1024);
      led1.fadeOff(1024);
      smallWait(1024);
      break;
  }
  if (millis() - last_state_change > 2000) {
    if (!shaking && state != OFF) {
      state = state - 1;
      last_state_change = millis();
    }
  }
}


void check_sensor() {

  xValue = analogRead(xPin);
  readingsX[marker] = xValue;
  marker++;
  if (marker >= 10)
    marker = 0;
  int total = 0;
  for (int i = 0; i < 10; i++) {
    total += readingsX[i];
  }
  avgX =  total / 10;

  yValue = analogRead(yPin);
  readingsY[marker] = yValue;
  total = 0;
  for (int i = 0; i < 10; i++) {
    total += readingsY[i];
  }
  avgY =  total / 10;

  zValue = analogRead(zPin);
  readingsZ[marker] = zValue;
  total = 0;
  for (int i = 0; i < 10; i++) {
    total += readingsZ[i];
  }
  avgZ =  total / 10;

  if ( abs(avgX - xValue) > 2 ||  abs(avgY - yValue) > 2 ||  abs(avgZ - zValue) > 2) {
    shaking = 1;
  }
  else {
    if ( millis() - last_state_change > 1000) {
      shaking = 0;
    }
  }


}
bool state_changed () {


  check_sensor();

  if (prevstate != shaking ) {
    prevstate = shaking;
    Serial.print(state);
    Serial.print("\t");
    Serial.print(shaking);
    Serial.print("\t");
    Serial.println(millis() - last_state_change);
    last_state_change = millis();
    return true;
  }
  return false;
}
void runLEDs() {
  led1.run();
  led2.run();
  led3.run();
  led4.run();
  led5.run();
}
void smallWait(int interval) {
  unsigned long small_wait = millis();
  while ((millis() - small_wait) < interval)
  {
    runLEDs();
  }

}

void wait (int waittime )
{
  unsigned long starttime = millis();
  //  while ((unsigned long)(starttime - previousMillis) >= waittime)
  while ((millis() - starttime) < waittime)
  {

    if (state_changed()) {
      Serial.println("State Changed");
      break;
    }
    else {
      smallWait(30);
    }


  }

}

