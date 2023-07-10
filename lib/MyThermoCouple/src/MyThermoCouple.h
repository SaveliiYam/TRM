#pragma once
#include "Arduino.h"

class MyThermoCouple {
public:
  MyThermoCouple(const byte& SCLK, const byte& CS, const byte& MISO);
  MyThermoCouple();

  void ini(const byte& SCLK, const byte& CS, const byte& MISO);

  float ReadCelsius(void);
  float ReadFahrenheit(void);

  /*!    @brief  For compatibility with older versions
         @returns Temperature in F or NAN on failure! */
  float ReadFarenheit(void) { return ReadFahrenheit(); }

private:
  byte sclk, miso, cs;
  uint8_t spiread(void);
};