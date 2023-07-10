// this library is public domain. enjoy!
// https://learn.adafruit.com/thermocouple/

#include "MyThermoCouple.h"

MyThermoCouple::MyThermoCouple(const byte &SCLK, const byte &CS, const byte &MISO)
{
  sclk = SCLK;
  cs = CS;
  miso = MISO;

  pinMode(cs, OUTPUT);
  pinMode(sclk, OUTPUT);
  pinMode(miso, INPUT);

  digitalWrite(cs, HIGH);
}

MyThermoCouple::MyThermoCouple() {}

void MyThermoCouple::ini(const byte &SCLK, const byte &CS, const byte &MISO)
{
  sclk = SCLK;
  cs = CS;
  miso = MISO;

  pinMode(cs, OUTPUT);
  pinMode(sclk, OUTPUT);
  pinMode(miso, INPUT);

  digitalWrite(cs, HIGH);
}

float MyThermoCouple::ReadCelsius(void)
{

  uint16_t v;

  digitalWrite(cs, LOW);
  delayMicroseconds(10);

  v = spiread();
  v <<= 8;
  v |= spiread();

  digitalWrite(cs, HIGH);

  if (v & 0x4)
  {
    // uh oh, no thermocouple attached!
    return NAN;
    // return -100;
  }

  v >>= 3;

  return v * 0.25;
}

float MyThermoCouple::ReadFahrenheit(void) { return ReadCelsius() * 9.0 / 5.0 + 32; }

byte MyThermoCouple::spiread(void)
{
  int i;
  byte d = 0;

  for (i = 7; i >= 0; i--)
  {
    digitalWrite(sclk, LOW);
    delayMicroseconds(10);
    if (digitalRead(miso))
    {
      // set the bit to 0 no matter what
      d |= (1 << i);
    }

    digitalWrite(sclk, HIGH);
    delayMicroseconds(10);
  }

  return d;
}