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
    uint16_t timer = millis();
    uint16_t ticker = 0;
    byte numberPause = 0;

public:
    ProgrammRunner(const temperaturePausesStruct &programm, const bool &time, const bool &delay)
        : programm_(programm), timeSet(time), timeDelay(delay) {}
    ProgrammRunner(const bool &time, const bool &delay) : timeSet(time), timeDelay(delay) {}
    ProgrammRunner(){}
    void putTimeSet(const bool &time) { timeSet = time; }
    void putTimeDelay(const bool &delay) { timeDelay = delay; }
    void putTimeSettings(const bool &time, const bool &delay)
    {
        timeSet = time;
        timeDelay = delay;
    }
    void putProgramm(const temperaturePausesStruct &programm) { programm_ = programm; }
    bool is_programm_run() { return is_stopped; } // если программа закончена, то false
    void programm_stop() { is_stopped = false; }
    void startPause()
    {
        timer = millis();
        ticker = programm_.time[numberPause];
    }

    Pair<byte, uint16_t> runningProgramm(const float &temperature)
    {

        if (!timeDelay || temperature >= (programm_.setpointTemperature[numberPause] - 1) && !can_to_start)
        {
            can_to_start = true;
            startTime = true;
            startPause();
        }
        else
        {
            startTime = false;
        }

        if (startTime && timer + programm_.time[numberPause] >= millis()) // если отсчет закончен
        {
            Pair<byte, uint16_t> res{numberPause, programm_.time[numberPause]};
            can_to_start = false;
            numberPause++;
            if (numberPause == 6)
            {
                numberPause = 0;
                programm_stop();
            }
            return res;
        }
        else if (startTime && timer + programm_.time[numberPause] < millis()) // если отсчет еще не закончен
        {
            Pair<byte, uint16_t> res{numberPause, programm_.time[numberPause]};
            return res;
        }
        else if (!startTime) // если еще не начался отсчет
        {
            Pair<byte, uint16_t> res{numberPause, programm_.time[numberPause]};
            return res;
        }
    }
};
