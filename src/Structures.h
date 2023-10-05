#pragma once
#include <Arduino.h>

/*
    EEPROM:
    0 - 1 pause
    35 - 2 pause
    70 - 3 pause
    105 - 4 pause
    140 - 4 pause
    175 - 5 pause
    210 - 6 pause
    
    250 - TRM

    500 - PIDKoefficients
*/

struct temperaturePausesStruct{ //30 byte + 10byte
    byte setpointTemperature[6]; //1 byte
    uint32_t time[6];            //4 byte
};

struct parametrs{
    parametrs(const bool& timeSet, const bool& timeDelay, const byte& powerMax, const byte& powerMin, const float& calibr)
            : _timeSet(timeSet)
            , _timeDelay(timeDelay)
            , _powerMax(powerMax)
            , _powerMin(powerMin) 
            , _calibr(calibr) {}
    parametrs(){}
    bool _timeSet;
    bool _timeDelay;
    byte _powerMax = 255, _powerMin = 0;
    float _calibr = 0;
};


struct Koefficients
{
    float P = 1;
    float I = 1;
    float D = 1;
};
