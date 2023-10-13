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

  void Calibration(const float& value);
  float GetCalibValue() const;
private:
  byte sck, so, cs;
  float calib_value = 0;
  uint8_t spiread();
  uint32_t _tmr = millis();
};