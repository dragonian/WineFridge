#ifndef UTILS_H
#define UTILS_H

//==========================================================

#include <stdlib.h>
#include <Arduino.h>

class RepeatAtInterval {
public:

  RepeatAtInterval(unsigned long interval);

  void ResetTime(void);
  
  boolean CheckInterval(void);
  
private:
  unsigned long mLastCheck;
  unsigned long mInterval;

};



class LED {
public:
  LED(int pin);

  void Setup(void);
  
   void Tick(void);
  
private:
  bool ledState;
  unsigned long previousMillis;
  int pin;

};


#endif

