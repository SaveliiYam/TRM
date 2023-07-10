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


struct temperaturePausesStruct{ //30 byte + 10byte
    byte setpointTemperature[6]; //1 byte
    uint16_t time[6];            //4 byte
};

class TRM
{
private:
    byte _upButton, _downButton, _numberButton;
    TemperaturePauseCollector collector;
    //MyButton downButton;
    //MyButton upButton;
    MyButton settingsButton;
    MyButton startStopButton;
    //MyButton numberButton;
    MyButton motorButton;
    MyThermoCouple termoCouple;
    byte numberPause = 0;
    byte checkButtonClicked();
    byte checkButtonPressed();
    bool timeSet = false; //переменная связанная со временем, она должна загружаться из EEprom (false == min)
    bool timeDelay = false;//переменная связанная с отложенным временем
    byte powerMax, powerMin;//переменные, связанные с макс и мин мощностями, должны загружаться из EEprom
    void WiFiConnect(); //Подключение к wifi
    bool wifiOn = false;
    LCD lcd;
    PIDRegulator regulator;
public:
    TRM(const byte& dwnBtn, const byte& upBtn, const byte& setBtn, const byte& strtBtn,
        const byte& nmbBtn, const byte& mtrBtn, const byte& sckPin, const byte& csPin, const byte& soPin);
    float getTemperature();
    void chooseNumberPause();
    byte getNumberPause() const;
    void enterTemperaturePauses();
    void settings();
    void main_programm();
};



