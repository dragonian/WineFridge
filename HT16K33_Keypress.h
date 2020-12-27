#ifndef HT16K33_KEYPRESS_H
#define HT16K33_KEYPRESS_H

//==========================================================

#include <stdlib.h>
#include <Arduino.h>

const int MAX_CB = 4;

class HT16K33_Keypress
{
public:
  HT16K33_Keypress(uint8_t addr);

  void Setup(int pin);
  
  boolean CheckForKeypress(void);
  void ReadKeyData(void);

  typedef void KeypressCB(uint16_t * keydata );
 
  void RegisterKeypressCallback(const KeypressCB * cb);

  static void isr(void);

private:

  unsigned long kpMillis;
  uint8_t i2c_addr;

  KeypressCB * cb[MAX_CB]; 

  volatile static bool int_pending;

};


#endif

