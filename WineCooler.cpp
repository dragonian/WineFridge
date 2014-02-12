#include "WineCooler.h"

#define TEMP_MAX   (60)
#define TEMP_MIN   (40)
   
WineUI::WineUI(Adafruit_7segment * display, int digitNum ) :
  mDisplay(display), mDigitPos(digitNum), delay30Sec(30*1000)
{
  mDisplayValue = 0;
  mLastSetVal = 55;
  mCurTempVal = 0;

  lightOn = false;
  settingTemp = false;
}

void WineUI::Setup()
{
  mDisplay->writeDigitRaw(mDigitPos,   0x71);
  mDisplay->writeDigitRaw(mDigitPos+1, 0x71);
  mDisplay->writeDisplay();
}


void WineUI::Run()
{
    
  // Working on changing the temp
  if (settingTemp) 
  {
    // Temp selection is now complete
    if (delay30Sec.CheckInterval())
    {
      settingTemp = false;
      
      // Set the new temp value so it can be used by controller
      mLastSetVal = mDisplayValue;
  
      // flip display back to current temp
      mDisplayValue = mCurTempVal;
    }
  }
  else
  {
    // make sure the display reflects the current temp  
    mDisplayValue = mCurTempVal;
  }

  //if (lightOn) 
  //{
  //  mDisplay->writeDigitRaw(4, 1 << mDigitPos);  
  //}

  mDisplay->writeDigitNum(mDigitPos,   (mDisplayValue / 10) % 10);
  mDisplay->writeDigitNum(mDigitPos+1,  mDisplayValue % 10);
  mDisplay->writeDisplay();

}

void WineUI::Keypress(enum Keycode key)
{
  switch (key)
  {
    case TEMP_UP:
      settingTemp = true;
      delay30Sec.ResetTime();
      mDisplayValue = CheckBounds(mDisplayValue+1);
      break;

    case TEMP_DOWN:
      settingTemp = true;
      delay30Sec.ResetTime();
      mDisplayValue = CheckBounds(mDisplayValue-1);
      break;

    case LIGHT_TOGGLE:
      lightOn = !lightOn; 
      break;

  }
}

int WineUI::CheckBounds(int val)
{
  if (val > TEMP_MAX)
    return TEMP_MAX;
  if ( val < TEMP_MIN )
     return TEMP_MIN; 
  return val;  
}
   
//==========================================================
//==========================================================
//==========================================================


WineCooler::WineCooler(WineUI* ui, 
                       TempSensor * hotTemp, TempSensor * coolTemp,
                       PWM* cool, 
                       PWM* hotFan, PWM* coolFan ) :
  mUI(ui), mHotTemp(hotTemp), mCoolTemp(coolTemp),
  mCooler(cool),
  mHotFan(hotFan), mCoolFan(coolFan),
  every10Sec(5000),
  mCoolPID(&mInputTemp, &mOutputControl, &mDesiredTemp, 20,1,1, REVERSE),
  mFanPID(&mHotTempVal, &mHotFanControl, &mHotSetpoint, 20,1,1, REVERSE)  
{
   mEnabled = false;
   mDesiredTemp = 50;
   mOutputControl = 0;

   mHotTempVal = 0;
   mHotSetpoint = 70;
   mHotFanControl = 0;
}

void WineCooler::Init()
{
  mEnabled = true;
  //mCoolPID.SetMode(MANUAL);
  mCoolPID.SetMode(AUTOMATIC);
  mCoolPID.SetSampleTime(1000);  // 1 second sample time (matches conversion time on sensors)

  mFanPID.SetMode(AUTOMATIC);
  mFanPID.SetSampleTime(1000);

  // Setup Pins
  mHotFan->Setup();
  mCoolFan->Setup();
  mCooler->Setup();

  mCoolFan->SetRate(255);
  //mHotFan->SetRate(150);
  mHotFan->On();
  mCooler->On();
}

void WineCooler::Run()
{
  if (every10Sec.CheckInterval())
    DebugStatus(); 

  if (!mEnabled)
    return;

  // Get the latest temps from the sensors
  mInputTemp = mCoolTemp->GetTemp();
  mHotTempVal = mHotTemp->GetTemp();

  // Update UI with new values
  mUI->Run();  
  
  // aim for the middle of the temp value, i.e. 48.5
  mDesiredTemp = (mUI->mLastSetVal + 0.5);
  mUI->mCurTempVal = mInputTemp;

  // Run the PIDs with any input changes
  mCoolPID.Compute();
  mCooler->SetRate(mOutputControl);

  mFanPID.Compute();
  mHotFan->SetRate(mHotFanControl);

  //if (mHotTemp->GetTemp() > 70)
  //  mHotFan->On();
  //else
  //  mHotFan->Off();

  // If the Cooler is on, then the cool fan should be too
  if (mOutputControl > 0)
    mCoolFan->On();
  else
    mCoolFan->Off();

  mUI->Run();

}
 
void WineCooler::DebugStatus()
{

  Serial.print("\r\nWineCooler Status: ");
  mEnabled ?  Serial.print("ON "): Serial.print("OFF ");

  Serial.print("Desired Temp: ");
  Serial.print(mDesiredTemp);
  Serial.print(" F ");

  Serial.print("Current Temp: ");
  Serial.print(mInputTemp);
  Serial.print(" F ");

  Serial.print("Hot Temp: ");
  Serial.print(mHotTempVal);
  Serial.print(" F ");

  Serial.print("Output: ");
  Serial.print(mOutputControl);

  bool tmp1;
  int tmp2;
  mHotFan->GetStatus(tmp1, tmp2);
  Serial.print(" HotFan: ");
  Serial.print(tmp1);
  Serial.print(" ");
  Serial.print(tmp2);
  
  mCoolFan->GetStatus(tmp1, tmp2);
  Serial.print(" CoolFan: ");
  Serial.print(tmp1);
  Serial.print(" ");
  Serial.print(tmp2);
  
}
