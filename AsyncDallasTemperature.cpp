
#include "AsyncDallasTemperature.h"

AsyncDallasTemperature::AsyncDallasTemperature(OneWire* wire) : DallasTemperature(wire), mCB(NULL)
{
}


void AsyncDallasTemperature::SetConversionCompleteCB(const ConversionCompleteCB* cb)
{
  mCB = cb;  
}
 
 
void AsyncDallasTemperature::CheckTemps()
{
    
  // If we don't have a cb, it's not necessary to run conversions
  if(!mCB)
    return;

  if (mLastTempRequest == 0)
  {
    setWaitForConversion(false);  // makes it async
    requestTemperatures(); // Send the command to get temperatures
    mLastTempRequest = millis(); 
  } 

  // Is the request done?
  if (millis() - mLastTempRequest > 1000)
  {    
    if (mCB)
      mCB();
      
    // Kick off another...
    setWaitForConversion(false);  // makes it async
    requestTemperatures(); // Send the command to get temperatures
    mLastTempRequest = millis(); 

  }    
}
