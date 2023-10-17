#pragma once
#include <Arduino.h>
#include "buttons/MyButton.h"
#include "thermocouple/MyThermoCouple.h"
#include "TemperatureProgramm.h"
#include "TemperaturePausesCollector.h"
#include "couples/Pair.h"

#include <EEPROM.h>
#include <WiFi.h>
#include "PIDRegulator.h"
#include "Structures.h"
#include "ProgrammRunner.h"
#include "couples/Third.h"
#include "couples/Fouth.h"

#include "LCDrus.h"

/*
    EEPROM:
    60 - 1 pause
    100 - 2 pause
    140 - 3 pause
    200 - PIDKoefficients
    250 - TRM
*/

class TRM
{
private:
    byte _upButton, _downButton, _numberButton, _motorPin, numberPause = 0;
    byte powerMax = 0, powerMin = 0; // переменные, связанные с макс и мин мощностями, должны загружаться из EEprom
    byte predel_temperature = 101;

    ProgrammRunner runner;
    TemperaturePauseCollector collector;
    MyButton settingsButton, startStopButton, motorButton, upButton, downButton;
    MyThermoCouple termoCouple;

    bool timeSet = false;   // переменная связанная со временем, она должна загружаться из EEprom (false == min)
    bool timeDelay = false; // переменная связанная с отложенным временем
    
    bool oper_mode = false; //режим работы
    byte power = 0; //мощность
    byte set_temp_for_th = 0;//Температура уставки 
    byte number_pause_for_th = 0;
    uint32_t set_time_for_th = 0;

    float calib_value = 0;

    LCD lcd;
    PIDRegulator regulator;

    byte getNumberPause() const;
    void enterTemperaturePauses();
    void settings();
    void startProgramm();
    void runningProgramm();
    void poweringProgramm();
    void tunningProgramm();
    void printMainMenu(const int &temperatureNew);
    void WiFiConnect(); // Подключение к wifi
    void saveParametrs();
    void loadParametrs();
    void baseParametrs();
    void chooseNumberPause();
    void tuningPID();

public:
    TRM(const byte &dwnBtn, const byte &upBtn, const byte &setBtn, const byte &strtBtn,
        const byte &nmbBtn, const byte &mtrBtn, const byte &mtrPin, const byte &sckPin, const byte &csPin, const byte &soPin);

    void ini();
    void main_programm();
    byte getPIDvalue();
    byte getPIDvalueTune() const;
    float getTemperature();
    float getCalibrationValue() const;
    void enterCalibrationValue(const float &value);
    byte getPredelTemperature() const;
    void enterPredelTemperature(const byte &predel);

    // серверная часть
    byte get_number_prog() const { return numberPause; }
    void start_program_from_server(const bool &start);
    void stop_program_from_server(const bool &stop);
    void put_number_prog(const byte &number);
    Fouth<bool, bool, byte, byte> getParametrs() const { return Fouth<bool, bool, byte, byte>(timeSet, timeDelay, powerMax, powerMin); }
    void save_parametrs_time(const byte &what, const bool &value);
    void save_parametrs_power(const byte &what, const byte &value);
    bool getMode() const{return oper_mode;}
    void enterMode(const bool& mode);
    byte getPowerValueNow()const {return power;}
    void enterPowerValueNow(const byte& value);
    Third<byte, uint32_t, byte> getOutputTimeTem() const{return Third<byte, uint32_t, byte>{set_temp_for_th, set_time_for_th, number_pause_for_th};}
};
