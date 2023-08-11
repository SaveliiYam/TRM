#include "ProgrammRunner.h"

ProgrammRunner::ProgrammRunner(const temperaturePausesStruct &programm, const bool &time, const bool &delay)
    : programm_(programm), timeSet(time), timeDelay(delay) {}
ProgrammRunner::ProgrammRunner(const bool &time, const bool &delay) : timeSet(time), timeDelay(delay) {}
ProgrammRunner::ProgrammRunner() {}
Pair<byte, uint16_t> ProgrammRunner::runningProgramm(const float &temperature)
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
    else{
        Pair<byte, uint16_t> res{numberPause, 0};
        return res;
    }
}