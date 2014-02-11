
#include "Utils.h"


RepeatAtInterval::RepeatAtInterval(unsigned long interval) :
  mLastCheck(0), mInterval(interval)
{
}

void RepeatAtInterval::ResetTime(void)
{
  mLastCheck = millis(); 
}

boolean RepeatAtInterval::CheckInterval(void)
{
  if (mLastCheck == 0)
  {
    mLastCheck = millis(); 
    return true;
  } 

  // Is the request done?
  if (millis() - mLastCheck > mInterval)
  {    
    mLastCheck = millis();
    return true; 
  }    
  return false;
} 

LED::LED(int _pin) :
  pin(_pin)
{
  ledState = LOW;    
  previousMillis = 0;
}

void LED::Setup(void)
{
  pinMode(pin, OUTPUT); 
}


void LED::Tick(void)
{
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis > 1000)
  {
    // save the last time you blinked the LED
    previousMillis = currentMillis;  

    // if the LED is off turn it on and vice-versa:
    if (ledState == LOW)
      ledState = HIGH;
    else
      ledState = LOW;

    // set the LED with the ledState of the variable:
    digitalWrite(pin, ledState);
  }
}
