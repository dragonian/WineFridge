#include "WineComponents.h"
   
   
//==========================================================
//==========================================================
//==========================================================

PWM::PWM(int _pin)
{
  pin = _pin;
  onoff = false;
  rate = 150;
}

void PWM::Setup()
{
  pinMode(pin, OUTPUT);
}

void PWM::On()
{
  onoff = true;
  analogWrite(pin, rate);
}

void PWM::Off()
{
  onoff = false;
  analogWrite(pin, 0);
}

void PWM::SetRate(int _rate)
{
  rate = _rate;
  if (onoff)
    analogWrite(pin, rate);
}

void PWM::GetStatus(bool & _onff, int & _rate)
{
  _onff = onoff;
  _rate = rate;
}



//==========================================================
//==========================================================
//==========================================================


TempSensor::TempSensor(DallasTemperature * dt, DeviceAddress addr, float offset) : mDT(dt), mCB(NULL), mCorrectionOffset(offset)
{
   memcpy(mAddr,addr,8);
}

void TempSensor::SetTempCallback(const TempCallback* cb)
{
  mCB = cb;  
}
 
void TempSensor::ConversionComplete()
{
  mTemp = mDT->getTempF(mAddr) + mCorrectionOffset;
      
  if (mCB)
    mCB(mAddr, (int)mTemp);
        
}

float TempSensor::GetTemp()
{
  return mTemp;  
}
  
 
