#pragma once
#include <Arduino.h>
#include "MyButton.h"
#include "MyThermoCouple.h"
#include "TemperatureProgramm.h"
#include "TemperaturePausesCollector.h"
#include "Pair.h"

#include <EEPROM.h>
#include <WiFi.h>
#include "PIDRegulator.h"
#include "Structures.h"
#include "ProgrammRunner.h"
#include "Third.h"
#include "Fouth.h"

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
    byte powerMax, powerMin; // переменные, связанные с макс и мин мощностями, должны загружаться из EEprom
    ProgrammRunner runner;
    TemperaturePauseCollector collector;
    MyButton settingsButton, startStopButton, motorButton;
    MyThermoCouple termoCouple;

    bool timeSet = false;   // переменная связанная со временем, она должна загружаться из EEprom (false == min)
    bool timeDelay = false; // переменная связанная с отложенным временем

    LCD lcd;
    PIDRegulator regulator;

    byte getNumberPause() const;
    void enterTemperaturePauses();
    void settings();
    void motorOn();
    void startProgramm();
    void runningProgramm();
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
    byte getPIDvalueTune();
    float getTemperature();

    // серверная часть
    byte get_number_prog() const { return numberPause; }
    void start_program_from_server(const bool &start);
    void stop_program_from_server(const bool &stop);
    void put_number_prog(const byte &number);
    Fouth<bool, bool, byte, byte> getParametrs() const { return Fouth<bool, bool, byte, byte>(timeSet, timeDelay, powerMax, powerMin); }
    void save_parametrs_time(const byte &what, const bool &value);
    void save_parametrs_power(const byte &what, const byte &value);
};
