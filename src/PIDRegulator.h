#pragma once
#include <Arduino.h>
#include <GyverPID.h>
#include <PIDtuner.h>
#include <EEPROM.h>
#include "Structures.h"
#include "Pair.h"
#include "Third.h"

class PIDRegulator
{
private:
    Koefficients koefficients;
    GyverPID regulator;
    PIDtuner tuner;
    bool tune = false;
    Pair<byte, byte> parametrsForLCD{0, 0};

public:
    PIDRegulator();

    // Надо каждый раз вводить лимиты
    void setLimits(const byte &minValue, const byte &maxValue)
    {
        regulator.setLimits(map(minValue, 0, 100, 0, 255), map(maxValue, 0, 100, 0, 255));
    }
    // Получить значение
    int getValuePID(const int &temperatureNow);
    // Внести температуру уставки
    void putTemperature(const int &setTemperature);
    // Надо вызывать перед каждым тюном
    void tuneInitialization(const float &temperatureNeed);
    // тюн
    Third<bool, byte, byte> tunePID(const int &temperatureNow);
    Pair<byte, byte> GetPIDValueTune(const int &temperatureNow);
    // вставить коэффициенты в регулятор
    void enterPIDKoefficients(const Koefficients &koefficients);
    //
    void loadKoefficients();
    void saveKoefficients();
    void baseKoefficients();
    bool getTuneInfo() const;
    Pair<byte, byte> GetParametrsLCD() const;
};
