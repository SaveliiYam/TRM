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
    Serial.println("Load Programm to runner: ");
    for (int i = 0; i!=6; i++){
        programm_.setpointTemperature[i] = programm.setpointTemperature[i];
        programm_.time[i] = programm.time[i];
        Serial.print("programm runner temp: ");
        Serial.println(programm_.setpointTemperature[i]);
        Serial.print("programm main temp: ");
        Serial.println(programm.setpointTemperature[i]);
        Serial.println(".....");
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

    Serial.println("New pause");
}

Pair<byte, uint32_t> ProgrammRunner::runningProgramm(const float &temperature)
{
    Serial.print("Delay: ");
    Serial.println(timeDelay);
    Serial.print("temperature: ");
    Serial.println(temperature >= programm_.setpointTemperature[numberPause] - 1);
    Serial.print("start: ");
    Serial.println(can_to_start);
    Serial.println(".......");
    
    if ((timeDelay || temperature >= (programm_.setpointTemperature[numberPause] - 1)) && !can_to_start)
    {
        Serial.println("i here");
        can_to_start = true;
        startTime = true;
        startPause();
    }

    if (startTime && timer + programm_.time[numberPause] <= millis()) // если отсчет закончен
    {
        Pair<byte, uint32_t> res{numberPause, programm_.time[numberPause]};
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
        Serial.println("Stop pause");
        startTime = false;
        return res;
    }
    else if (startTime && timer + programm_.time[numberPause] > millis()) // если отсчет еще не закончен
    {
        Pair<byte, uint32_t> res{numberPause, programm_.time[numberPause]};
        Serial.println("Also work");
        return res;
    }
    else if (!startTime) // если еще не начался отсчет
    {
        Pair<byte, uint32_t> res{numberPause, programm_.time[numberPause]};
        //Serial.println("Wait delay");
        return res;
    }
    else
    {
        Pair<byte, uint32_t> res{numberPause, 0};
        return res;
    }
}