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
#include <WiFiManager.h> //библиотека для подключения к wifi
#include <strings_en.h> 
#include "PIDRegulator.h"
#include "Structures.h"

/*
    EEPROM:
    60 - 1 pause
    100 - 2 pause
    140 - 3 pause
    200 - PIDKoefficients
    250 - TRM
*/



struct parametrs{
    parametrs(const bool& timeSet, const bool& timeDelay, const byte& powerMax, const byte& powerMin)
            : _timeSet(timeSet)
            , _timeDelay(timeDelay)
            , _powerMax(powerMax)
            , _powerMin(powerMin) {}
    parametrs(){}
    bool _timeSet = false;
    bool _timeDelay = false;
    byte _powerMax = 255, _powerMin = 0;

};

class TRM
{
private:
    byte _upButton, _downButton, _numberButton, _motorPin, numberPause = 0;
    TemperaturePauseCollector collector;
    //MyButton downButton;
    //MyButton upButton;
    MyButton settingsButton, startStopButton, motorButton;
    //MyButton numberButton;
    MyThermoCouple termoCouple;
    byte checkButtonClicked();
    byte checkButtonPressed();
    bool timeSet = false; //переменная связанная со временем, она должна загружаться из EEprom (false == min)
    bool timeDelay = false;//переменная связанная с отложенным временем
    bool programmRun = false; // Переменная связанная с запуском программы
    byte powerMax, powerMin;//переменные, связанные с макс и мин мощностями, должны загружаться из EEprom
    void WiFiConnect(); //Подключение к wifi
    bool wifiOn = false, _motorState = false;
    LCD lcd;
    PIDRegulator regulator;
    void saveParametrs();
    void loadParametrs();
    void baseParametrs();
    float getTemperature();
    void chooseNumberPause();
    byte getNumberPause() const;
    void enterTemperaturePauses();
    void settings();
    void motorOn();
    void startProgramm();
    void runProgramm();
    
public:
    TRM(const byte& dwnBtn, const byte& upBtn, const byte& setBtn, const byte& strtBtn,
        const byte& nmbBtn, const byte& mtrBtn, const byte& mtrPin , const byte& sckPin, const byte& csPin, const byte& soPin);
    
    void main_programm();
    byte getPIDvalue();
};



