#pragma once
#include <Arduino.h>
#include <GyverPID.h>


struct Koefficients
{
    unsigned int P = 0;
    unsigned int I = 0;
    unsigned int D = 0;
};

class PIDRegulator
{
private:
    Koefficients koefficients;
    GyverPID regulator;
    byte _setpointTemperature = 0;

public:
    PIDRegulator()
    { // Нужно из EEprom грузить коэффициенты и писать их в регулятор
        regulator.Kp = koefficients.P;
        regulator.Ki = koefficients.I;
        regulator.Kd = koefficients.D;
        //regulator.setDt(400);
        regulator.setDirection(NORMAL);
        regulator.setLimits(0, 255);
    }
    void const setLimits(const byte &minValue, const byte &maxValue) { regulator.setLimits(minValue, maxValue); }
    void const putTemperature(const byte &temperature) { _setpointTemperature = temperature; }
    int getValuePID(const float& temperatureIsTermoCouple){
        regulator.input = temperatureIsTermoCouple;
        return regulator.getResult();
    }
};