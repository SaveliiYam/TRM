#pragma once
#include <Arduino.h>
#include <GyverPID.h>
#include <PIDtuner.h>
#include <EEPROM.h>

struct Koefficients
{
    float P = 1;
    float I = 1;
    float D = 1;
};

class PIDRegulator
{
private:
    Koefficients koefficients;
    GyverPID regulator;
    PIDtuner tuner;

public:
    PIDRegulator();

    //Надо каждый раз вводить лимиты
    void setLimits(const byte &minValue, const byte &maxValue) { regulator.setLimits(minValue, maxValue); }
    //Получить значение
    int getValuePID(const int &temperatureNow);
    //Внести температуру уставки
    void putTemperature(const int& setTemperature);
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

