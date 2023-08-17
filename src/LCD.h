#pragma once
#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "Pair.h"
//#include "Simbols.h"




class LCD
{
private:
    LiquidCrystal_I2C lcd{0x27, 20, 4};
    void Point(const byte &parametr);

    byte RightArrow[8] = {
    0b00000,
    0b00100,
    0b00110,
    0b11111,
    0b11111,
    0b00110,
    0b00100,
    0b00000,
};

public:
    LCD();
    LCD(const LiquidCrystal_I2C& lcdNew);
    void ClearAll();
    void WriteOnLcdPause(const Pair<byte, uint16_t> &pause, const byte &numberPause);
    void WriteTemperature(const float &temperature);
    void Clear(const byte &curs);
    const void ClearLcd() { lcd.clear(); }
    void settingsMainMenu(const byte &parametr);
    void WiFi(const bool &parametr);
    void PowerLimits(const bool &parametr, const byte &minLimit, const byte &maxLimit);
    void PrintLimitMenu(const bool& parametr, const byte& limit);
    void TimeSettings(const bool &parametr);
    void TimeConcrete(const bool &parametr, const bool &value);
    void NumberProg(const byte& numberProgramm);
    void EnterTemperaturePause(byte numberProgramm, byte step, const bool &parametr, const byte &temperature, const uint16_t &time);
    void BaseSettings(const bool &parametr);
    void stopProgramm();
    void startProgramm();
    void mainMenu(const int &temperature, const byte &number);
    void mainMenu(const int &temperature, const byte& number, const byte max, const byte& min, const bool& delay, const bool& set);
    void workProgramm(const float& temperature, const byte& numberPause, const uint32_t& time, const char* timeParametr);
};

