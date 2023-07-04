#pragma once

#include <Arduino.h>
#include "Pair.h"
#include <EEPROM.h>

/*
struct temperaturePausesStruct{
    byte setpointTemperature[6];
    int16_t time[6];
};
*/

class TemperatureProgramm{
private:
    temperaturePausesStruct _pausesStruct;
    
public:
    TemperatureProgramm(const temperaturePausesStruct& pauses)
        : _pausesStruct(pauses)
    {  //создание программы внесением в него структуры
        _pausesStruct = pauses;//надо сразу сохранить его в EEprom
    }
    
    //Чтение структуры из EEprom
    TemperatureProgramm(const byte& adress){//надо брать номер ячейки и считывать с него
        EEPROM.get(adress, _pausesStruct);
        savePauses(adress);
    }
    
    ~TemperatureProgramm(){}

    Pair<byte, uint16_t> readPause(const byte& pause) const{ //получаем значения с одной паузы
        Pair<byte, uint16_t> pp(_pausesStruct.setpointTemperature[pause], _pausesStruct.time[pause]);
        return pp;
    }
    
    void writePauses(char temp[6], uint16_t times[6]){ //запись всех пауз, затем надо сохранить
        for(int i = 0; i < 6; i++){_pausesStruct.setpointTemperature[i] = temp[i];}
        for(int i = 0; i < 6; i++){_pausesStruct.time[i] = times[i];}
    }

    void writePauses(const temperaturePausesStruct& structWrite){ //запись паузы, затем надо сохранить
        _pausesStruct = structWrite;
    }

    temperaturePausesStruct getPauseAll(){//считываем структуры с EEprom
        return _pausesStruct;
    }
    
    void savePauses(const int& adress){//надо записывать паузы в память
        EEPROM.put(adress, _pausesStruct);
    }

};