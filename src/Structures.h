#pragma once
#include <Arduino.h>

struct temperaturePausesStruct{ //30 byte + 10byte
    byte setpointTemperature[6]; //1 byte
    uint16_t time[6];            //4 byte
};