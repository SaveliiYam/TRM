// this library is public domain. enjoy!
// https://learn.adafruit.com/thermocouple/

#include "MyThermoCouple.h"

MyThermoCouple::MyThermoCouple(const byte &SCK, const byte &CS, const byte &SO)
{
  sck = SCK;
  cs = CS;
  so = SO;

  pinMode(cs, OUTPUT);
  pinMode(sck, OUTPUT);
  pinMode(so, INPUT);

  digitalWrite(cs, HIGH);
}

MyThermoCouple::MyThermoCouple() {}

void MyThermoCouple::ini(const byte &SCK, const byte &CS, const byte &SO)
{
  sck = SCK;
  cs = CS;
  so = SO;

  pinMode(cs, OUTPUT);
  pinMode(sck, OUTPUT);
  pinMode(so, INPUT);

  digitalWrite(cs, HIGH);
}

float MyThermoCouple::ReadCelsius(void)
{
  static float temperature = 0;
  if (millis() - _tmr > 400)
  {
    _tmr = millis();
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
      return -100;
    }

    v >>= 3;

    temperature = v * 0.25;
  }
  return temperature;
}

float MyThermoCouple::ReadFahrenheit(void) { return ReadCelsius() * 9.0 / 5.0 + 32; }

byte MyThermoCouple::spiread(void)
{
  int i;
  byte d = 0;

  for (i = 7; i >= 0; i--)
  {
    digitalWrite(sck, LOW);
    delayMicroseconds(10);
    if (digitalRead(so))
    {
      // set the bit to 0 no matter what
      d |= (1 << i);
    }

    digitalWrite(sck, HIGH);
    delayMicroseconds(10);
  }

  return d;
}