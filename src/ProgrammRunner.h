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
    uint16_t timer = millis();
    uint16_t ticker = 0;
    byte numberPause = 0;

public:
    ProgrammRunner(const temperaturePausesStruct &programm, const bool &time, const bool &delay)
        : programm_(programm), timeSet(time), timeDelay(delay) {}
    ProgrammRunner(const bool &time, const bool &delay) : timeSet(time), timeDelay(delay) {}
    void putTimeSet(const bool &time) { timeSet = time; }
    void putTimeDelay(const bool &delay) { timeDelay = delay; }
    void putProgramm(const temperaturePausesStruct &programm) { programm_ = programm; }
    
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
        switch (numberPause)
        {
        case 0:
            if(startTime && timer + programm_.time[numberPause] >= millis()){
                Pair<byte, uint16_t> res{numberPause, programm_.time[numberPause]};
                can_to_start = false;
                numberPause++;
                return res;
            }
            else if(startTime && timer + programm_.time[numberPause] < millis()){
                Pair<byte, uint16_t> res{numberPause, programm_.time[numberPause]};
                return res;
            }
            else if(!startTime){
                Pair<byte, uint16_t> res{numberPause, programm_.time[numberPause]};
                return res;
            }
            break;
        case 1:
            
            break;
        case 2:

            break;
        case 3:

            break;
        case 4:

            break;
        case 5:

            break;
        default:
            numberPause = 0;
            break;
        }
    }
};
