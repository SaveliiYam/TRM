#pragma once
#include <Arduino.h>
#include "Structures.h"
#include "Pair.h"

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
    void putTimeSet(const bool &time) { timeSet = time; }
    void putTimeDelay(const bool &delay) { timeDelay = delay; }
    void putTimeSettings(const bool &time, const bool &delay)
    {
        timeSet = time;
        timeDelay = delay;
    }
    void putProgramm(const temperaturePausesStruct &programm) { programm_ = programm; }
    bool is_programm_run() const { return is_stopped; } // если программа закончена, то false
    void programm_stop() { is_stopped = false; }
    void startPause()
    {
        timer = millis();
        ticker = programm_.time[numberPause];
    }
    Pair<byte, uint32_t> runningProgramm(const float &temperature);
};

