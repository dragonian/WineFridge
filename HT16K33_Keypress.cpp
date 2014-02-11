
#include "HT16K33_Keypress.h"
#include <Wire.h>


volatile bool HT16K33_Keypress::int_pending = 0;

HT16K33_Keypress::HT16K33_Keypress(uint8_t addr) :
   i2c_addr(addr)
{
  kpMillis = 0;

  for (int i=0; i<MAX_CB; i++)
  {
    cb[i] = NULL;
  }

}

void HT16K33_Keypress::Setup(int intno)
{
  Wire.beginTransmission(i2c_addr);
  Wire.write(0xA0 | 0x1);  // Setup interrupt active low
  Wire.endTransmission();

  pinMode(0, INPUT_PULLUP); // 
  attachInterrupt(intno, isr, FALLING);  
}

void HT16K33_Keypress::isr(void)
{
  int_pending = 1;
}

boolean HT16K33_Keypress::CheckForKeypress(void)
{
  if(millis() - kpMillis > 600) 
  {
    kpMillis = millis(); 
           
    if (int_pending == 1)
    {
      int_pending = 0;
      ReadKeyData();
      return true;
    }

  }
  return false;
  
} 

void HT16K33_Keypress::ReadKeyData()
{
  //uint8_t data[6] = {0,0,0,0,0,0};
  uint16_t data[3] = {0,0,0};
  uint8_t* next = (uint8_t*)data;
    
  Wire.beginTransmission(i2c_addr);
  Wire.write(0x40);
  Wire.endTransmission();
  Wire.requestFrom(i2c_addr, (uint8_t)6); 
  
  while(Wire.available())    // slave may send less than requested
  {
    char c = Wire.read();    // receive a byte as character
    *next++ = c; 
  }

  for (int i=0; i<MAX_CB; i++)
  {
    if (cb[i])
      cb[i](data);
  }
}  

void HT16K33_Keypress::RegisterKeypressCallback(const KeypressCB * _cb)
{

  for (int i=0; i<MAX_CB; i++)
  {
    if (cb[i] == NULL)
    {
      cb[i] = _cb; 
      break;
    }
  }


}
  /*
  switch (data[1])
  {
    case 0x8:
      Serial.print("Left Up\n");
      left++;
      break;
      
    case 0x4:
      Serial.print("Left Down\n");
      left--;
      break;
      
    case 0x2:
      Serial.print("Left LED\n");
      if (light == LOW)
        light = HIGH;
      else
        light = LOW;
        
      //digitalWrite(1, light);
      break;  
  }
  
  switch (data[3])
  {
    case 0x8:
      Serial.print("Right Up\n");
      right++;
      break;
      
    case 0x4:
      Serial.print("Right Down\n");
      right--;
      break;
      
    case 0x2:
      Serial.print("Right LED\n");
      //if (light == LOW)
      //  light = HIGH;
      //else
      //  light = LOW;
        
      //digitalWrite(1, light);
      break;  
  }
*/

