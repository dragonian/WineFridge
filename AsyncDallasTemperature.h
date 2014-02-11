#ifndef __AsyncDallasTemperature_h__
#define __AsyncDallasTemperature_h__

#include <OneWire.h>
#include <DallasTemperature.h>

class AsyncDallasTemperature : public DallasTemperature
{
public:

  AsyncDallasTemperature(OneWire*);
  
  typedef void ConversionCompleteCB();
  
  void SetConversionCompleteCB(const ConversionCompleteCB * cb);
  
  void CheckTemps();
  
private:
  unsigned long mLastTempRequest;
  ConversionCompleteCB * mCB;

};   

#endif
