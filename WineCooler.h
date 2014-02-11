#ifndef WINECOOLER_H
#define WINECOOLER_H

#include "WineComponents.h"
#include "Utils.h"

#include <Adafruit_LEDBackpack.h>
#include <Adafruit_GFX.h>

#include <PID_v1.h>

//==========================================================

class WineUI
{
  friend class WineCooler;  // Let the Cooler access our members

public:
  WineUI(Adafruit_7segment * display, int digitNum);

  void Setup(void);

  void Run(void);

  enum Keycode
  {
    TEMP_UP,
    TEMP_DOWN,
    LIGHT_TOGGLE  
  };

  void Keypress(enum Keycode key);
  int CheckBounds(int val);

private:
  Adafruit_7segment * mDisplay;
  int mDigitPos;

  int mDisplayValue;
  int mLastSetVal;
  int mCurTempVal;

  bool lightOn;
  bool settingTemp;

  RepeatAtInterval delay30Sec;
  
};


//==========================================================

class WineCooler 
{
public:
  WineCooler(WineUI* ui, 
            TempSensor * hotTemp,
            TempSensor * coolTemp,
            PWM* cool, 
            PWM* hotFan, PWM* coolFan  );
  
  void Init(void);
  void Run(void);

  void DebugStatus();
  
private:
  WineUI* mUI;
  TempSensor * mHotTemp;
  TempSensor * mCoolTemp;
  PWM* mCooler;
  PWM* mHotFan;
  PWM* mCoolFan; 

  bool mEnabled;
  RepeatAtInterval  every10Sec;

  double mDesiredTemp, mInputTemp, mOutputControl;
  PID mCoolPID;
  double mHotSetpoint, mHotTempVal, mHotFanControl;
  PID mFanPID;

};

#endif


