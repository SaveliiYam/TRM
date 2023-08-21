#pragma once
#include <Arduino.h>

/*
    EEPROM:
    60 - 1 pause
    100 - 2 pause
    140 - 3 pause
    200 - PIDKoefficients
    250 - TRM

    300 - 4 pause
    340 - 5 pause
    380 - 6 pause
    420 - 7 pause
    460 - 8 pause
    500 - 9 pause
    540 - 10 pause
*/

struct temperaturePausesStruct{ //30 byte + 10byte
    byte setpointTemperature[6]; //1 byte
    uint32_t time[6];            //4 byte
};

struct parametrs{
    parametrs(const bool& timeSet, const bool& timeDelay, const byte& powerMax, const byte& powerMin)
            : _timeSet(timeSet)
            , _timeDelay(timeDelay)
            , _powerMax(powerMax)
            , _powerMin(powerMin) {}
    parametrs(){}
    bool _timeSet;
    bool _timeDelay;
    byte _powerMax = 255, _powerMin = 0;
};