#pragma once

#include <Arduino.h>
#include "TemperatureProgramm.h"
#include "Pair.h"
#include "Structures.h"
/*
Каждый раз при использовании пауз, генерируется коллектор,
в который надо передавать номер программы и паузы
В зависимости от номера программы будет производится своя структура
*/
class TemperaturePauseCollector
{
private:
    byte _numberProgramms[3] = {0, 1, 2};
    byte _adress[3] = {60, 100, 140}; // ячейки в которые будут записываться паузы
public:
    TemperaturePauseCollector() {}

    void writePauses(const byte &numberProgramm, const temperaturePausesStruct &pause)
    {                                                                                       // записываем паузу целиком
        TemperatureProgramm *pauseClass = new TemperatureProgramm(_adress[numberProgramm]); // Создали паузу по адрессу
        pauseClass->writePauses(_adress[numberProgramm], pause);

        Serial.println("I write");
        Serial.print("Number pause: ");
        Serial.println(numberProgramm);
        for (int i = 0; i != 5; i++)
        {
            Serial.print("Temp: ");
            Serial.println(pause.setpointTemperature[i]);
            Serial.print("Time: ");
            Serial.println(pause.time[i]);
        }
        Serial.println("...");

        delete pauseClass;
    }
    Pair<byte, uint32_t> readPause(const byte &numberProgramm, const byte &numberPause)
    { // получаем одну паузу
        TemperatureProgramm *pauseClass = new TemperatureProgramm(_adress[numberProgramm]);
        Pair<byte, uint32_t> buffer = pauseClass->readPause(numberPause);
        delete pauseClass;
        return buffer;
    }

    temperaturePausesStruct getPause(const byte &numberProgramm)
    { // получить полную программу
        TemperatureProgramm *pauseClass = new TemperatureProgramm(_adress[numberProgramm]);
        temperaturePausesStruct buffer = pauseClass->getPauseAll();

        Serial.println("I read");
        Serial.print("Number pause: ");
        Serial.println(numberProgramm);
        for (int i = 0; i != 5; i++)
        {
            Serial.print("Temp: ");
            Serial.println(buffer.setpointTemperature[i]);
            Serial.print("Time: ");
            Serial.println(buffer.time[i]);
        }
        Serial.println("...");

        delete pauseClass;
        return buffer;
    }
};