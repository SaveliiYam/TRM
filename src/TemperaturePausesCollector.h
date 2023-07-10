#pragma once

#include <Arduino.h>
#include "TemperatureProgramm.h"
#include "Pair.h"
/*
Каждый раз при использовании пауз, генерируется коллектор, 
в который надо передавать номер программы и паузы
В зависимости от номера программы будет производится своя структура
*/
class TemperaturePauseCollector{
private:
    byte _numberProgramms[3] = {0, 1, 2};
    byte _adress[3] = {60, 100, 140};//ячейки в которые будут записываться паузы
public:
    TemperaturePauseCollector(){}

    void writePauses(const byte& numberProgramm, const temperaturePausesStruct& pause){//записываем паузу целиком
        TemperatureProgramm* pauseClass = new TemperatureProgramm(_adress[numberProgramm]);//Создали паузу по адрессу
        pauseClass->writePauses(_adress[numberProgramm], pause);
        delete pauseClass;
    }
    Pair<byte, uint16_t> readPause(const byte& numberProgramm, const byte& numberPause){ //получаем одну паузу
        TemperatureProgramm* pauseClass = new TemperatureProgramm(_adress[numberProgramm]);
        Pair<byte, uint16_t> buffer = pauseClass->readPause(numberPause);
        delete pauseClass;
        return buffer;
    }

    temperaturePausesStruct getPause(const byte& numberProgramm){ //получить полную программу
        TemperatureProgramm* pauseClass = new TemperatureProgramm(_adress[numberProgramm]);
        temperaturePausesStruct buffer = pauseClass->getPauseAll();
        delete pauseClass; 
        return buffer;
    }

};