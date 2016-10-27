/*
  bb_LED.h - Library for controlling LEDs
  Created by Biji Biji Initiative Oct 2016.
  Released into the public domain.
*/
#ifndef bb_LED_h
#define bb_LED_h

#include "Arduino.h"

class bb_LED
{
  public:
    bb_LED(int pin);
    void run();
    void on();
    void off();
    void fadeOn(int interval);
    void fadeOff(int interval);
    void toggle();
    enum { IDLE, FADE_UP, LED_ON, FADE_DOWN, LED_OFF }; // STATES


  private:
    void _updateLED();
    void _startTimer();
    void _stopTimer();
    void _resetTimer();
    int _pin;
    bool _timerOn;
    int _state;
    int _brightness;
    int _stepTime;
    int _counter;
    unsigned long _timer;
    static const uint8_t SLOPE_SIZE = 32;
    uint8_t slope[SLOPE_SIZE] = {0, 0, 1, 1, 2, 2, 3, 4, 5, 6, 7, 8, 10, 12, 15, 18, 22, 26, 31, 37, 44, 54, 63, 76, 90, 108, 127, 153, 180, 217, 230, 255};


};

#endif
