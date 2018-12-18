#ifndef LED_H
#define LED_H
  
/**
*    WiFi Devices, firmware for connecting devices to already available 
*    android applicaion.
*
*    Copyright (C) 2018  Abhishek Safui
*
*    This program is free software: you can redistribute it and/or modify
*    it under the terms of the GNU General Public License as published by
*    the Free Software Foundation, either version 3 of the License, or
*    (at your option) any later version.
*
*    This program is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU General Public License for more details.
*
*    You should have received a copy of the GNU General Public License
*    along with this program.  If not, see <https://www.gnu.org/licenses/>
*/

#include<Arduino.h>
enum ACTIVE_MODE{
  ACTIVE_LOW=0,
  ACTIVE_HIGH=1
};
class LED{
  
  uint8_t _pin;
  uint8_t _activeMode;
  uint8_t _state;
  unsigned int timestamp;
  
public:
  LED(uint8_t pin, uint8_t activeMode):
  _pin(pin), _activeMode(activeMode){
    pinMode(_pin,OUTPUT);
    OFF();
    timestamp = millis();

  }
  virtual void ON(){
    digitalWrite(_pin,
      _activeMode == ACTIVE_HIGH? HIGH: LOW);
      _state=HIGH;
      timestamp = millis();
  }
  virtual void OFF(){
    digitalWrite(_pin,
      _activeMode == ACTIVE_HIGH? LOW: HIGH);
      _state = LOW;
      timestamp = millis();
  }
  virtual void toggle(){
    if(_state==HIGH){
      OFF();
    }else{
      ON();
    }
    timestamp = millis();
  }


  uint8_t state(){
    return _state;
  }
};

#endif
