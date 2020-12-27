#ifndef WINECOMPONENTS_H
#define WINECOMPONENTS_H

#include <stdlib.h>
#include <Arduino.h>

#include <DallasTemperature.h>
//#include <OneWire.h>

class PWM 
{
public:
  PWM(int pin);
  
  void Setup(void);

  void On(void);
  void Off(void);
  
  void SetRate(int rate);

  void GetStatus(bool & onff, int & rate);

private:
  int pin;
  int rate;
  boolean onoff;

};


//==========================================================

class TempSensor
{
public:
  TempSensor(DallasTemperature * dt, DeviceAddress addr, float offset = 0.0);
  
  typedef void TempCallback(DeviceAddress, unsigned int temp);
  
  void SetTempCallback(const TempCallback*);
  
  void ConversionComplete();

  float GetTemp();
  
private:

  DeviceAddress mAddr;
  DallasTemperature * mDT;  
  TempCallback * mCB;

  float mTemp;
  float mCorrectionOffset;
};

#endif
