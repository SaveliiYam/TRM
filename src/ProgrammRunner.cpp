#include "ProgrammRunner.h"

ProgrammRunner::ProgrammRunner(const temperaturePausesStruct &programm, const bool &time, const bool &delay)
    : programm_(programm), timeSet(time), timeDelay(delay) {}
ProgrammRunner::ProgrammRunner(const bool &time, const bool &delay) : timeSet(time), timeDelay(delay) {}
ProgrammRunner::ProgrammRunner() {}

void ProgrammRunner::putTimeSet(const bool &time) { timeSet = time; }
void ProgrammRunner::putTimeDelay(const bool &delay) { timeDelay = delay; }
void ProgrammRunner::putTimeSettings(const bool &time, const bool &delay)
{
    timeSet = time;
    timeDelay = delay;
}
void ProgrammRunner::putProgramm(const temperaturePausesStruct &programm)
{
    for (int i = 0; i!=6; i++){
        programm_.setpointTemperature[i] = programm.setpointTemperature[i];
        programm_.time[i] = programm.time[i];
    }

    is_stopped = true;
    can_to_start = false;
}

bool ProgrammRunner::is_programm_run() const { return is_stopped; } // если программа закончена, то false
void ProgrammRunner::programm_stop() { is_stopped = false; }
void ProgrammRunner::startPause()
{
    timer = millis();
    ticker = programm_.time[numberPause];
}

Third<byte, uint32_t, byte> ProgrammRunner::runningProgramm(const float &temperature)
{
    
    if ((timeDelay || temperature >= (programm_.setpointTemperature[numberPause] - 1)) && !can_to_start)
    {
        can_to_start = true;
        startTime = true;
        startPause();
    }

    if (startTime && timer + programm_.time[numberPause] <= millis()) // если отсчет закончен
    {
        Third<byte, uint32_t, byte> res{numberPause, 0, programm_.setpointTemperature[numberPause]};
        can_to_start = false;
        numberPause++;
        if (numberPause == 6)
        {
            numberPause = 0;
            programm_stop();
        }
        else
        {
            startPause();
        }
        startTime = false;
        return res;
    }
    else if (startTime && timer + programm_.time[numberPause] > millis()) // если отсчет еще не закончен
    {
        uint32_t finalTime = timer + programm_.time[numberPause] - millis();
        Third<byte, uint32_t, byte> res{numberPause, finalTime, programm_.setpointTemperature[numberPause]};
        return res;
    }
    else if (!startTime) // если еще не начался отсчет
    {
        Third<byte, uint32_t, byte> res{numberPause, programm_.time[numberPause], programm_.setpointTemperature[numberPause]};
        return res;
    }
    else
    {
        Third<byte, uint32_t, byte> res{numberPause, 0, programm_.setpointTemperature[numberPause]};
        return res;
    }
}