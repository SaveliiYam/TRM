#pragma once
#include <Arduino.h>
#include "Structures.h"
#include "couples/Pair.h"
#include "couples/Third.h"
// #include "TemperaturePausesCollector.h"

class ProgrammRunner
{
private:
    temperaturePausesStruct programm_;
    bool timeSet = false;
    bool timeDelay = false;
    bool startTime = false;
    bool can_to_start = false;
    bool is_stopped = false;
    uint32_t timer = millis();
    uint32_t ticker = 0;
    byte numberPause = 0;

public:
    ProgrammRunner(const temperaturePausesStruct &programm, const bool &time, const bool &delay);
    ProgrammRunner(const bool &time, const bool &delay);
    ProgrammRunner();
    void putTimeSet(const bool &time);
    void putTimeDelay(const bool &delay);
    void putTimeSettings(const bool &time, const bool &delay);
    void putProgramm(const temperaturePausesStruct &programm);
    bool is_programm_run() const; // если программа закончена, то false
    void programm_stop();
    void startPause();
    //Pair<byte, uint32_t> runningProgramm(const float &temperature);
    Third<byte, uint32_t, byte> runningProgramm(const float &temperature);
};
