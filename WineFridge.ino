
#include "HT16K33_Keypress.h"
#include "WineCooler.h"

#include <Wire.h> 
#include <PID_v1.h>

#include "Adafruit_LEDBackpack.h"
#include "Adafruit_GFX.h"

#include "AsyncDallasTemperature.h"
#include <DallasTemperature.h>
#include <OneWire.h>
#include <EEPROM.h>
#include <avr/wdt.h>

#define PIN_LED     (13)
#define PIN_1WIRE   (8)


#define PIN_FAN1    (5)
#define PIN_FAN2    (6)
#define PIN_COOL1   (12)

#define PIN_FAN3    (10)
#define PIN_FAN4    (11)
#define PIN_COOL2   (9)

#define I2C_ADDR    (0x70)

#define KP_INT      (2)


LED led(PIN_LED);

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(PIN_1WIRE);

// Pass our oneWire reference to Dallas Temperature.
AsyncDallasTemperature sensors(&oneWire);

Adafruit_7segment matrix;
HT16K33_Keypress kp(I2C_ADDR);

// Discovered addresses to temp sensors, hardcoded here
TempSensor ts1(&sensors, (DeviceAddress){0x28, 0x6A, 0x13, 0x30, 0x05, 0x00, 0x00, 0x5D}, -4.0);
TempSensor ts2(&sensors, (DeviceAddress){0x28, 0x03, 0x13, 0x30, 0x05, 0x00, 0x00, 0x08} );

PWM cool1(PIN_COOL2);
PWM fan1(PIN_FAN3);
PWM fan2(PIN_FAN4);

WineUI topui(&matrix, 0);

/* ui, hotTemp, coolTemp, cool, hotFan, coolFan */
WineCooler tophalf(1, &topui, &ts2, &ts1, &cool1, &fan1, &fan2);

//------------------------------------------------------------------------------------------
TempSensor ts3(&sensors, (DeviceAddress){0x28, 0x29, 0x22, 0x30, 0x05, 0x00, 0x00, 0x38}, -3.0);
TempSensor ts4(&sensors, (DeviceAddress){0x28, 0xFF, 0x09, 0x5F, 0x05, 0x00, 0x00, 0xCC});

PWM cool2(PIN_COOL1);
PWM fan3(PIN_FAN1);
PWM fan4(PIN_FAN2);

WineUI bottomui(&matrix, 1);

/* ui, hotTemp, coolTemp, cool, hotFan, coolFan */
WineCooler bottomhalf(2, &bottomui, &ts4, &ts3, &cool2, &fan3, &fan4);
//------------------------------------------------------------------------------------------


void setup()
{
  wdt_disable();
  Serial.begin(9600);
  led.Setup();

  // turn on Display controller
  matrix.begin(I2C_ADDR);
 
  sensors.begin(); // IC Default 9 bit. If you have troubles consider upping it 12. Ups the delay giving the IC more time to process the temperature measurement
  sensors.setResolution(12);
  sensors.SetConversionCompleteCB(&TempConversionComplete);  

  tophalf.Init();
  bottomhalf.Init();
  
  // Setup Keypress interrupt
  kp.Setup(KP_INT);
  kp.RegisterKeypressCallback(TopKeypressCB);
  kp.RegisterKeypressCallback(BottomKeypressCB);
  
  
  wdt_enable(WDTO_1S);
}


void loop()
{
  led.Tick();
  sensors.CheckTemps();
  kp.CheckForKeypress();

  tophalf.Run();
  bottomhalf.Run();
  
  //discoverOneWireDevices();
 
  delay(10);
  wdt_reset();
}


void TempConversionComplete()
{
  ts1.ConversionComplete();
  ts2.ConversionComplete();
  ts3.ConversionComplete();
  ts4.ConversionComplete();
}

void TopKeypressCB(uint16_t * keydata)
{
  switch (keydata[0])
  {
    case 0x0800:
      topui.Keypress(WineUI::TEMP_UP);
      break;
      
    case 0x0400:
       topui.Keypress(WineUI::TEMP_DOWN);
     break;
      
    case 0x0200:
       topui.Keypress(WineUI::LIGHT_TOGGLE);
     break;  
  }
}

void BottomKeypressCB(uint16_t * keydata)
{
  switch (keydata[1])
  {
    case 0x0800:
      bottomui.Keypress(WineUI::TEMP_UP);
      break;
      
    case 0x0400:
       bottomui.Keypress(WineUI::TEMP_DOWN);
     break;
      
    case 0x0200:
       bottomui.Keypress(WineUI::LIGHT_TOGGLE);
     break;  
  }
}

//==========================================================




//==========================================================

#if 0
void discoverOneWireDevices(void) 
{
  byte i;
  byte present = 0;
  byte data[12];
  byte addr[8];
  
  Serial.print("Looking for 1-Wire devices...\n\r");
  while(oneWire.search(addr)) {
    Serial.print("\n\rFound \'1-Wire\' device with address:\n\r");
    for( i = 0; i < 8; i++) {
      Serial.print("0x");
      if (addr[i] < 16) {
        Serial.print('0');
      }
      Serial.print(addr[i], HEX);
      if (i < 7) {
        Serial.print(", ");
      }
    }
    if ( OneWire::crc8( addr, 7) != addr[7]) {
        Serial.print("CRC is not valid!\n");
        return;
    }
  }
  Serial.print("\n\r\n\rThat's it.\r\n");
  oneWire.reset_search();
  return;
}
#endif

/*
  while (Serial.available() > 0) {
    Serial.print("Enter rate: ");
    
    // look for the next valid integer in the incoming serial stream:
    int rate = Serial.parseInt(); 
    
    Serial.print("Setting speed to ");
    Serial.print(rate);  
    Serial.print("0% \n");
    analogWrite(5, rate*25);   
    analogWrite(6, rate*25);
    analogWrite(10, rate*25);   
    analogWrite(11, rate*25);
  }
*/  
