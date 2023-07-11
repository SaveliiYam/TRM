#include "TRM.h"

TRM::TRM(const byte &dwnBtn, const byte &upBtn, const byte &setBtn, const byte &strtBtn,
         const byte &nmbBtn, const byte &mtrBtn, const byte& mtrPin , const byte &sckPin, const byte &csPin, const byte &soPin)
    : _upButton(upBtn), _downButton(dwnBtn), _numberButton(nmbBtn), _motorPin(mtrPin)
{
    settingsButton.ini(setBtn);
    startStopButton.ini(strtBtn);
    motorButton.ini(mtrBtn);
    
    pinMode(_motorPin, OUTPUT);
    digitalWrite(_motorPin, _motorState);

    termoCouple.ini(sckPin, csPin, soPin);
    regulator.setLimits(powerMin, powerMax);
}

float TRM::getTemperature()
{
    return termoCouple.ReadCelsius();
}

byte TRM::getNumberPause() const { return numberPause; }

// Кнопки up down создаются через динамическую память
void TRM::enterTemperaturePauses()
{
    temperaturePausesStruct pauseEnter = collector.getPause(numberPause);
    MyButton *upButton = new MyButton(_upButton);
    MyButton *downButton = new MyButton(_downButton);
    for (byte i = 0; i < 6; i++)
    {
        byte temp1 = pauseEnter.setpointTemperature[i];
        uint16_t time1 = pauseEnter.time[i];
        lcd.ClearAll();
        lcd.EnterTemperaturePause(numberPause, i, false, temp1, time1);
        while (!startStopButton.Clicked())
        { // ввод температуры
            if (downButton->Clicked())
            {
                temp1--;
                lcd.ClearAll();
                lcd.EnterTemperaturePause(numberPause, i, false, temp1, time1);
            }
            if (upButton->Clicked())
            {
                temp1++;
                lcd.ClearAll();
                lcd.EnterTemperaturePause(numberPause, i, false, temp1, time1);
            }
            temp1 = constrain(temp1, 0, 255);
        }
        lcd.ClearAll();
        lcd.EnterTemperaturePause(numberPause, i, true, temp1, time1);
        while (!startStopButton.Clicked())
        { // ввод времени
            if (downButton->Clicked())
            {
                time1--;
                lcd.ClearAll();
                lcd.EnterTemperaturePause(numberPause, i, true, temp1, time1);
            }
            if (upButton->Clicked())
            {
                time1++;
                lcd.ClearAll();
                lcd.EnterTemperaturePause(numberPause, i, true, temp1, time1);
            }
        }
        pauseEnter.setpointTemperature[i] = temp1;
        pauseEnter.time[i] = time1;
    }
    collector.writePauses(numberPause, pauseEnter);
    delete downButton, upButton;
}

void TRM::settings()
{ // настройки 1-паузы, 2-вкл?выкл wifi, 3-параметры по мощности, 4-диапазон времени(сек/мин), 5- отложенное время
    if (settingsButton.Clicked())
    {
        MyButton *upButton = new MyButton(_upButton);
        MyButton *downButton = new MyButton(_downButton);
        byte i = 0;
        lcd.ClearAll();
        lcd.settingsMainMenu(i);
        while (1)
        {   
            //Чтобы зайти в базовые настройки надо зажать клавищу с мотором
            if(motorButton.Pressed()){
                bool parametr = false;
                lcd.ClearAll();
                lcd.BaseSettings(parametr);
                while(1){
                    if(upButton->Clicked() || downButton->Clicked()){
                        parametr = true ? false : true;
                        lcd.ClearAll();
                        lcd.BaseSettings(parametr);
                    }
                    if(settingsButton.Clicked()){
                        if(parametr)
                            baseParametrs();
                        break;
                    }
                }
            }
            if (settingsButton.Clicked())
                break;
            if (upButton->Clicked())
            {
                i++;
                i = constrain(i, 0, 3);
                lcd.ClearAll();
                lcd.settingsMainMenu(i);
            } // Перемещение кнопками up down
            if (downButton->Clicked())
            {
                i--;
                i = constrain(i, 0, 3);
                lcd.ClearAll();
                lcd.settingsMainMenu(i);
            }
            if (startStopButton.Clicked())
            {
                switch (i)
                {
                case 0: // Настройки паузы (ok)
                    MyButton *numberButton = new MyButton(_numberButton);
                    lcd.ClearAll();
                    lcd.NumberProg(numberPause);
                    while (1)
                    { // сначала выбираем номер паузы, затем вводим паузу
                        if (settingsButton.Pressed())
                        {
                            break;
                        }
                        if (numberButton->Clicked())
                        {
                            numberPause++;
                            if (numberPause == 2)
                            {
                                numberPause = 0;
                            }
                            lcd.NumberProg(numberPause);
                        }
                        if (startStopButton.Clicked())
                        {
                            enterTemperaturePauses();
                        }
                    }
                    delete numberButton;
                    break;
                case 1: // вкл/выкл wifi
                    lcd.ClearAll();
                    lcd.WiFi(wifiOn);
                    while (1)
                    {
                        // lcd.WiFi(wifiOn);
                        if (settingsButton.Clicked())
                        {
                            if (wifiOn)
                            {
                                WiFiConnect();
                            }
                            break;
                        }
                        if (upButton->Clicked())
                        {
                            if (!wifiOn)
                            {
                                wifiOn = true;
                            }
                            else
                            {
                                wifiOn = false;
                            }
                            lcd.ClearAll();
                            lcd.WiFi(wifiOn);
                        }
                        if (downButton->Clicked())
                        {
                            if (!wifiOn)
                            {
                                wifiOn = true;
                            }
                            else
                            {
                                wifiOn = false;
                            }
                            lcd.ClearAll();
                            lcd.WiFi(wifiOn);
                        }
                    }
                    break;
                case 2: // параметры по мощности (ok)
                    bool parametr = false;
                    lcd.ClearAll();
                    lcd.PowerLimits(parametr, powerMin, powerMax);
                    while (1)
                    {
                        // Выбор что изменять
                        if (settingsButton.Clicked())
                        {
                            break;
                        }
                        if (upButton->Clicked())
                        {
                            parametr = true ? false : true;
                            lcd.ClearAll();
                            lcd.PowerLimits(parametr, powerMin, powerMax);
                        }
                        if (downButton->Clicked())
                        {
                            parametr = true ? false : true;
                            lcd.ClearAll();
                            lcd.PowerLimits(parametr, powerMin, powerMax);
                        }
                        // Меняем значение
                        if (startStopButton.Clicked())
                        {
                            while (1)
                            {
                                if (settingsButton.Clicked())
                                {
                                    regulator.setLimits(powerMin, powerMax);
                                    break;
                                }
                                if (upButton->Clicked() && parametr)
                                {
                                    powerMax++;
                                    powerMax = constrain(powerMax, 0, 255);
                                    lcd.ClearAll();
                                    lcd.PowerLimits(parametr, powerMin, powerMax);
                                }
                                if (upButton->Clicked() && !parametr)
                                {
                                    powerMin++;
                                    powerMax = constrain(powerMin, 0, 255);
                                    lcd.ClearAll();
                                    lcd.PowerLimits(parametr, powerMin, powerMax);
                                }
                                if (downButton->Clicked() && parametr)
                                {
                                    powerMax--;
                                    powerMax = constrain(powerMax, 0, 255);
                                    lcd.ClearAll();
                                    lcd.PowerLimits(parametr, powerMin, powerMax);
                                }
                                if (downButton->Clicked() && parametr)
                                {
                                    powerMin--;
                                    powerMax = constrain(powerMin, 0, 255);
                                    lcd.ClearAll();
                                    lcd.PowerLimits(parametr, powerMin, powerMax);
                                }
                            }
                            // надо сохранить все данные в EEprom
                        }
                    }
                    break;
                case 3: // выбор времени sec/min (ok!)
                    bool parametr = false;
                    lcd.ClearAll();
                    lcd.TimeSettings(parametr);
                    while (1)
                    {
                        if (settingsButton.Clicked())
                        {
                            break;
                        }
                        if (upButton->Clicked() || downButton->Clicked())
                        {
                            parametr = true ? false : true;
                            lcd.ClearAll();
                            lcd.TimeSettings(parametr);
                        }
                        if (startStopButton.Clicked())
                        {
                            lcd.ClearAll();
                            if (parametr)
                            {
                                lcd.TimeConcrete(parametr, timeSet);
                            }
                            else
                            {
                                lcd.TimeConcrete(parametr, timeDelay);
                            }
                            while (1)
                            {
                                if (settingsButton.Clicked())
                                {
                                    break;
                                }
                                if (upButton->Clicked() || downButton->Clicked() && parametr)
                                {
                                    timeSet = true ? false : true;
                                    lcd.ClearAll();
                                    lcd.TimeConcrete(parametr, timeSet);
                                }
                                if (upButton->Clicked() || downButton->Clicked() && !parametr)
                                {
                                    timeSet = true ? false : true;
                                    lcd.ClearAll();
                                    lcd.TimeConcrete(parametr, timeDelay);
                                }
                            }
                        }
                    }
                    // надо сохранить в EEprom
                    break;
                default:
                    break;
                }
            }
        }
        delete upButton, downButton;
    }
}


void TRM::main_programm()
{
    settings();
}

void TRM::WiFiConnect()
{ // Подключение к WiFi в режиме сначала раздачи, а затем приема
    WiFi.mode(WIFI_STA);
    WiFiManager wifiManager;
    // wifiManager.resetSettings(); //перезапись имени wifi каждый запуск
    bool res;                             // храним переменную для подключения
    res = wifiManager.autoConnect("PVK"); // подключение телефона к точке
    if (!res)
    {
        ESP.restart();
    }
    else
    {
        wifiOn = true;
    }
}

void TRM::motorOn(){
    if(motorButton.Pressed()){
        _motorState = true ? false : true;
        digitalWrite(_motorPin, _motorState);
    }
}

void TRM::saveParametrs(){
    parametrs param{timeSet, timeDelay, powerMax, powerMin};
    EEPROM.put(250, param);
}
void TRM::loadParametrs(){
    parametrs param;
    EEPROM.get(250, param);
    timeSet = param._timeSet;
    timeDelay = param._timeDelay;
    powerMax = param._powerMax;
    powerMin = param._powerMin;
}
void TRM::baseParametrs(){
    parametrs param;
    EEPROM.put(250, param);
}

