#pragma once
#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "Pair.h"
// #include "Simbols.h"
class LCD
{
private:
    LiquidCrystal_I2C lcd{0x38, 20, 4};
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
    char probel[4] = "   ";
    char yes[3] = "\xe0\x61";
    char no[4] = "\x48\x65\xBF";

public:
    LCD();
    void ini();
    void ConnectToWifi(const bool &connect);
    void Connecting();
    void ClearAll();
    void WriteOnLcdPause(const Pair<byte, uint16_t> &pause, const byte &numberPause);
    void WriteTemperature(const float &temperature);
    void Clear(const byte &curs);
    const void ClearLcd() { lcd.clear(); }
    void settingsMainMenu(const byte &parametr);
    void WiFi(const bool &parametr);
    void PowerLimits(const bool &parametr, const byte &minLimit, const byte &maxLimit);
    void PrintLimitMenu(const bool &parametr, const byte &limit);
    void TimeSettings(const bool &parametr);
    void TimeConcrete(const bool &parametr, const bool &value);
    void NumberProg(const byte &numberProgramm);
    void EnterTemperaturePause(byte numberProgramm, byte step, const bool &parametr, const byte &temperature, const uint16_t &time);
    void BaseSettings(const bool &parametr);
    void stopProgramm();
    void startProgramm();
    void mainMenu(const int &temperature, const byte &number);
    void mainMenu(const int &temperature, const byte &number, const byte max, const byte &min, const bool &delay, const bool &set);
    void workProgramm(const int &setpointTemperature, const float &temperature, const byte &numberPause, const uint32_t &time, const char *timeParametr);
    void TuneBaseSettings(const byte &parametr);
    void TuneBaseSettings(const byte &parametr, const bool &choise);
    void TuneBaseSettings(const byte &parametr, const float &calib);
    void Tuning(const byte &temperatureMax);
    void TuningProcces(const byte &percent, const byte &temperatureNow);
    void SuccessConection(const bool& status);
};

