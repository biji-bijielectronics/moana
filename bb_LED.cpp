/*
  bb_LED.h - Library for controlling LEDs
  Created by Biji Biji Initiative Oct 2016.
  Released into the public domain.
*/

#include "Arduino.h"
#include "bb_LED.h"

bb_LED::bb_LED(int pin)
{
  pinMode(pin, OUTPUT);
  _pin = pin;
  _timerOn = false;
  _state = IDLE;
  _brightness = 0;
  _stepTime = 32;
}

void bb_LED::run()
{

  if (_timerOn && millis() - _timer > _stepTime) {
    switch (_state) {
      case FADE_UP:
        if (_counter <= 0) {
          _brightness = 255;
          _stopTimer();
          _state = LED_ON;
        } else {
          _brightness = slope[SLOPE_SIZE - _counter];
          _counter--;
          _resetTimer();
        }

        break;

      case FADE_DOWN:
        if (_counter <= 0) {
          _brightness = 0;
          _stopTimer();
          _state = LED_OFF;
        } else {
          _brightness = slope[_counter - 1];
          _counter--;
          _resetTimer();
        }

        break;


    }
    _updateLED();
  }

}


void bb_LED::on()
{
  _state = LED_ON;
  _brightness = 255;
  _updateLED();
  _stopTimer();

}

void bb_LED::off()
{
  _state = LED_OFF;
  _brightness = 0;
  _updateLED();
  _stopTimer();

}

void bb_LED::fadeOn(int interval)
{
  _state = FADE_UP;
  _stepTime = interval / SLOPE_SIZE;
  _startTimer();
}

void bb_LED::fadeOff(int interval)
{
  _state = FADE_DOWN;
  _stepTime = interval / SLOPE_SIZE;
  _startTimer();

}


void bb_LED::toggle()
{
  switch (_state) {
    case LED_ON:
      _state = FADE_DOWN;
      _startTimer();
      break;
    case FADE_UP:
      _state = FADE_DOWN;
      break;
    case FADE_DOWN:
      _state = FADE_UP;
      break;

    case LED_OFF:
      _state = FADE_UP;
      _startTimer();
      break;

  }

}

void bb_LED::_updateLED()
{
  analogWrite(_pin, _brightness);
}


void bb_LED::_startTimer()
{
  _timerOn = true;
  _timer = millis();
  _counter = SLOPE_SIZE;

}

void bb_LED::_resetTimer()
{
//  _timerOn = true;
  _timer = millis();

}



void bb_LED::_stopTimer()
{
  _timer = false;
}


