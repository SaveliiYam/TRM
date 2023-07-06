#pragma once
#include "Arduino.h"

class MyThermoCouple {
public:
  MyThermoCouple(const byte& SCK, const byte& CS, const byte& SO);
  MyThermoCouple();

  void ini(const byte& SCK, const byte& CS, const byte& SO);

  float ReadCelsius();
  float ReadFahrenheit();

  float ReadFarenheit() {return ReadFahrenheit();}

private:
  byte sck, so, cs;
  uint8_t spiread();
  uint32_t _tmr = millis();
};