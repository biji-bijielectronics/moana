#include "bb_LED.h"
#define xpin A0
#define ypin A1
#define zpin A2


bb_LED led1(3);
bb_LED led2(5);
bb_LED led3(9);

enum {OFF, LED1, LED2, LED3, LED4, LED5};

unsigned long debounceTime = 1000;
unsigned long last_check;

int prevX = 0;
int prevY = 0;
int prevZ = 0;

int xThres = 10;
int yThres = 10;
int zThres = 10;

bool moving = false;



void setup() {
  Serial.begin(9600);
  led1.off();
  led2.off();
  led3.off();
  last_check = millis();
}

void checkSensor() {
  int newX = analogRead(xpin);
  int newY = analogRead(ypin);
  int newZ = analogRead(zpin);

  if (abs(newX - prevX) > xThres) {
    moving = true;
  }

}

bool stateChanged() {
  bool prevState = moving;
  checkSensor();
  if (prevState != moving)
    return true;
  else
    return false;
}

void runLEDs() {
  led1.run();
  led2.run();
  led3.run();
}

void wait(int waitTime) {
  unsigned long startTime = millis();
  while (millis() - startTime < waitTime && !stateChanged()) {
    runLEDs();
  }

}


void loop() {
  led1.fadeOn(1024);
  wait(1500);
 led1.fadeOff(1024);
  wait(1500);
   
}

