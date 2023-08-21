#pragma once
#include <Arduino.h>
#include "MyButton.h"
#include "MyThermoCouple.h"
#include "TemperatureProgramm.h"
#include "TemperaturePausesCollector.h"
#include "Pair.h"
#include "LCD.h"
#include <EEPROM.h>
#include <WiFi.h>
#include "PIDRegulator.h"
#include "Structures.h"
#include "ProgrammRunner.h"
#include "Third.h"
#include "Fouth.h"

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
    // MyButton downButton;
    // MyButton upButton;
    MyButton settingsButton, startStopButton, motorButton;
    // MyButton numberButton;
    MyThermoCouple termoCouple;

    bool timeSet = false;   // переменная связанная со временем, она должна загружаться из EEprom (false == min)
    bool timeDelay = false; // переменная связанная с отложенным временем
    bool wifiOn = false, _motorState = false;

    LCD lcd;
    PIDRegulator regulator;

    byte getNumberPause() const;
    void enterTemperaturePauses();
    void settings();
    void motorOn();
    void startProgramm();
    void runningProgramm();
    void printMainMenu(const int &temperatureNew);
    void WiFiConnect(); // Подключение к wifi
    void saveParametrs();
    void loadParametrs();
    void baseParametrs();
    void chooseNumberPause();

public:
    TRM(const byte &dwnBtn, const byte &upBtn, const byte &setBtn, const byte &strtBtn,
        const byte &nmbBtn, const byte &mtrBtn, const byte &mtrPin, const byte &sckPin, const byte &csPin, const byte &soPin);

    void main_programm();
    byte getPIDvalue();
    float getTemperature();
    void getLCD(const LCD &lcdNew);

    // серверная часть
    void start_program_from_server();
    void stop_program_from_server();
    void put_number_prog(const byte &number);
    bool check_wifi() const { return wifiOn; }
    Fouth<bool, bool, byte, byte> getParametrs() const { return Fouth<bool, bool, byte, byte>(timeSet, timeDelay, powerMax, powerMax); }
};
