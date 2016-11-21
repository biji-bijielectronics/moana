#include "bb_LED.h"

int state = 0;
int shaking = 0;
int sensorPin = 2;
int ledPin = 13;

int shakeState = 0;

unsigned long last_shake;

int shake_length = 1000;

int prevstate = 0;

enum {OFF, LED1, LED2, LED3, LED4, LED5};
bb_LED led1(3);
bb_LED led2(5);
bb_LED led3(6);
bb_LED led4(9);
bb_LED led5(10);
bb_LED led6(11);

unsigned long last_state_change;

void setup() {

  Serial.begin(9600);

  pinMode(sensorPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);

  last_shake = millis();
  last_state_change = millis();

  state = OFF;

  led_test();

}


void led_test() {

  led1.fadeOn(512);
  smallWait(255);
  led2.fadeOn(512);
  smallWait(255);
  led3.fadeOn(512);
  smallWait(255);
  led4.fadeOn(512);
  led6.fadeOn(512);
  smallWait(255);
  led5.fadeOn(512);
  
  smallWait(512);
  
  delay(1000);
  led1.fadeOff(1024);
  led2.fadeOff(1024);
  led3.fadeOff(1024);
  led4.fadeOff(1024);
  led5.fadeOff(1024);
  led6.fadeOff(1024);
  smallWait(1024);
}

void loop() {

  switch (state) {
    case OFF:
      initialWait();
      if (shaking) {
        state = LED1;
      } else {
        led1.off();
        led2.off();
        led3.off();
        led4.off();
        led5.off();
        led6.off();
  
      }
      break;
    case LED1:
      led1.fadeOn(1024);
      wait(3000);
      if (shaking) {
        state = LED2;
      } else {
        led1.fadeOff(1024);
        state = OFF;
      }
      break;
    case LED2:
      led2.fadeOn(1024);
      wait(3000);
      if (shaking) {
        state = LED3;
      } else {
        led2.fadeOff(1024);
        state = LED1;
      }
      break;
    case LED3:
      led3.fadeOn(1024);
      wait(3000);
      if (shaking) {
        state = LED4;
      } else {
        led3.fadeOff(1024);
        state = LED2;
      }
      break;
    case LED4:
      led4.fadeOn(1024);
      led6.fadeOn(1024);
      
      wait(3000);
      if (shaking) {
        state = LED5;
      } else {
        led6.fadeOff(1024);
        led4.fadeOff(1024);
        state = LED3;
      }
      break;
    case LED5:
      Serial.println("here");
      led5.fadeOn(1024);
      smallWait(10000);
      state = OFF;
      Serial.println("now here");
      led6.fadeOff(2024);
      led5.fadeOff(2024);
      led4.fadeOff(2024);
      led3.fadeOff(2024);
      led2.fadeOff(2024);
      led1.fadeOff(2024);
      smallWait(2024);
      break;
  }

  /*
    // Go back a state if not
    if (millis() - last_state_change > 2000) {
    if (!shaking && state != OFF) {
      state = state - 1;
      last_state_change = millis();
    }
    }
  */

}



void check_sensor() {
  int newShake = digitalRead(sensorPin);
  if (newShake != shakeState) {
    shakeState = newShake;
    last_shake = millis();
    if (!shaking) {
      shaking = 1;
      digitalWrite(ledPin, HIGH);
    }
  }
  if (millis() - last_shake > shake_length) {
    if (shaking) {
      shaking = 0;
      digitalWrite(ledPin, LOW);

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
  led6.run();
  
}
void smallWait(int interval) {
  unsigned long small_wait = millis();
  while ((millis() - small_wait) < interval)
  {
    runLEDs();
  }

}



void initialWait() {
  unsigned long firstShakeTime;
  bool firstShake = false;
  Serial.println("InitialWait Begin");
  while (true) {

    check_sensor();

    if (shaking) {

      if (!firstShake) {
        Serial.println("First Shake");

        firstShakeTime = millis();
        firstShake = true;
      }

      if (millis() - firstShakeTime > 2000) {
        Serial.println("InitalWait Over");
        return;
      }

    } else {
      firstShake = false;
    }

    smallWait(10);
  }
}




void wait (int waitTime )
{
  unsigned long startTime = millis();
  while ((millis() - startTime) < waitTime)
  {

    if (state_changed() && (millis() - startTime) > 1000 ) {
      Serial.println("State Changed");
      break;
    }
    else {
      smallWait(10);
    }


  }

}



