#pragma once
#include <Arduino.h>
#include <GyverPID.h>
#include <PIDtuner.h>
#include <EEPROM.h>

struct Koefficients
{
    float P = 0;
    float I = 0;
    float D = 0;
};

class PIDRegulator
{
private:
    Koefficients koefficients;
    GyverPID regulator;
    PIDtuner tuner;

public:
    PIDRegulator()
    { // Нужно из EEprom грузить коэффициенты и писать их в регулятор
        enterPIDKoefficients(koefficients);
        regulator.setDt(400);
        regulator.setDirection(NORMAL);
        regulator.setLimits(0, 255);
    }

    //Надо каждый раз вводить лимиты
    void const setLimits(const byte &minValue, const byte &maxValue) { regulator.setLimits(minValue, maxValue); }
    //Получить значение
    byte getValuePID(const float &temperatureNow);
    //Надо вызывать перед каждым тюном
    void tuneInitialization(const float &temperatureNeed);
    //тюн
    bool tunePID(const float &temperatureNow);
    //вставить коэффициенты в регулятор
    void enterPIDKoefficients(const Koefficients &koefficients);
    //
    void loadKoefficients();
    void saveKoefficients();
    void baseKoefficients();
};

