#include "TRM.h"

TRM::TRM(const byte &dwnBtn, const byte &upBtn, const byte &setBtn, const byte &strtBtn,
         const byte &nmbBtn, const byte &mtrBtn, const byte &mtrPin, const byte &sckPin, const byte &csPin, const byte &soPin)
    : _upButton(upBtn), _downButton(dwnBtn), _numberButton(nmbBtn), _motorPin(mtrPin)
{
    settingsButton.ini(setBtn);
    startStopButton.ini(strtBtn);
    motorButton.ini(mtrBtn);

    pinMode(_motorPin, OUTPUT);
    digitalWrite(_motorPin, _motorState);

    loadParametrs();

    termoCouple.ini(sckPin, csPin, soPin);

    regulator.setLimits(powerMin, powerMax);

    runner.putTimeSettings(timeSet, timeDelay);

    Serial.print("Time set: ");
    if (timeSet)
    {
        Serial.println("true");
    }
    else
    {
        Serial.println("false");
    }
    Serial.print("Time delay: ");
    if (timeDelay)
    {
        Serial.println("true");
    }
    else
    {
        Serial.println("false");
    }
    Serial.print("POwer MAX: ");
    Serial.println(powerMax);
    Serial.print("POwer MIN: ");
    Serial.println(powerMin);
}

void TRM::getLCD(const LCD &lcdNew)
{
    lcd = lcdNew;
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
    byte pauseForLCD = numberPause + 1;
    for (byte i = 0; i < 6; i++)
    {
        byte temp1 = pauseEnter.setpointTemperature[i];
        uint32_t time1;
        if (timeSet)
        {
            time1 = pauseEnter.time[i] / 1000;
            // time1 = pauseEnter.time[i];
        }
        if (!timeSet)
        {
            time1 = pauseEnter.time[i] / 60000;
            // time1 = pauseEnter.time[i];
        }
        lcd.ClearAll();
        lcd.EnterTemperaturePause(pauseForLCD, i, false, temp1, time1);
        while (!startStopButton.Clicked())
        { // ввод температуры
            if (settingsButton.Clicked())
            {
                collector.writePauses(numberPause, pauseEnter);
                delete downButton, upButton;
                return;
            }
            if (downButton->Clicked())
            {
                temp1--;
                lcd.ClearAll();
                lcd.EnterTemperaturePause(pauseForLCD, i, false, temp1, time1);
            }
            if (upButton->Clicked())
            {
                temp1++;
                lcd.ClearAll();
                lcd.EnterTemperaturePause(pauseForLCD, i, false, temp1, time1);
            }
        }
        lcd.ClearAll();
        lcd.EnterTemperaturePause(pauseForLCD, i, true, temp1, time1);
        while (!startStopButton.Clicked())
        { // ввод времени
            if (settingsButton.Clicked())
            {
                collector.writePauses(numberPause, pauseEnter);
                delete downButton, upButton;
                return;
            }
            if (downButton->Clicked())
            {
                time1--;
                if (!timeSet)
                {
                    if (time1 > 120 || time1 < 0)
                    {
                        time1 = 0;
                    }
                }
                else
                {
                    if (time1 > 7200 || time1 < 0)
                    {
                        time1 = 7200;
                    }
                }
                Serial.println(time1);
                lcd.ClearAll();
                lcd.EnterTemperaturePause(pauseForLCD, i, true, temp1, time1);
            }
            if (upButton->Clicked())
            {
                time1++;
                if (!timeSet)
                {
                    if (time1 > 120)
                    {
                        time1 = 120;
                    }
                }
                else
                {
                    if (time1 > 7200)
                    {
                        time1 = 7200;
                    }
                }
                Serial.println(time1);
                lcd.ClearAll();
                lcd.EnterTemperaturePause(pauseForLCD, i, true, temp1, time1);
            }
        }
        pauseEnter.setpointTemperature[i] = temp1;
        if (timeSet)
        {
            pauseEnter.time[i] = time1 * 1000;
            // pauseEnter.time[i] = time1;
            Serial.println("sec");
        }
        else if (!timeSet)
        {
            pauseEnter.time[i] = time1 * 60000;
            // pauseEnter.time[i] = time1;
            Serial.println("min");
        }
        Serial.print("Time: ");
        Serial.println(time1);
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
        int whatMenu = 0;
        whatMenu++;
        lcd.ClearAll();
        lcd.settingsMainMenu(whatMenu);
        whatMenu--;
        lcd.settingsMainMenu(whatMenu);
        bool printSettings = false;

        while (1)
        {
            if (printSettings)
            {
                printSettings = false;
                lcd.ClearAll();
                lcd.settingsMainMenu(whatMenu);
            }
            // Чтобы зайти в базовые настройки надо зажать клавищу с мотором
            if (motorButton.Pressed())
            {
                bool parametr = false;
                lcd.ClearAll();
                lcd.BaseSettings(parametr);
                while (1)
                {
                    if (upButton->Clicked() || downButton->Clicked())
                    {
                        parametr = true ? false : true;
                        lcd.ClearAll();
                        lcd.BaseSettings(parametr);
                    }
                    if (settingsButton.Clicked())
                    {
                        if (parametr)
                            baseParametrs();
                        break;
                    }
                }
            }
            if (settingsButton.Pressed())
            {
                lcd.ClearAll();
                printMainMenu(0);
                break;
            }

            if (upButton->Clicked())
            {
                whatMenu++;
                // whatMenu = constrain(whatMenu, 0, 3);
                if (whatMenu >= 4)
                {
                    whatMenu = 0;
                }
                lcd.ClearAll();
                lcd.settingsMainMenu(whatMenu);
            } // Перемещение кнопками up down
            if (downButton->Clicked())
            {
                whatMenu--;
                // whatMenu = constrain(whatMenu, 0, 3);
                if (whatMenu < 0 || whatMenu >= 4)
                {
                    whatMenu = 3;
                }
                lcd.ClearAll();
                lcd.settingsMainMenu(whatMenu);
            }
            if (startStopButton.Clicked())
            {
                switch (whatMenu)
                {
                case 0: // Настройки паузы (ok)
                {
                    printSettings = true;
                    MyButton *numberButton = new MyButton(_numberButton);
                    lcd.ClearAll();
                    byte numberForLCD = numberPause;
                    numberForLCD++;
                    lcd.NumberProg(numberForLCD);
                    while (1)
                    { // сначала выбираем номер паузы, затем вводим паузу
                        if (settingsButton.Clicked())
                        {
                            break;
                        }
                        if (numberButton->Clicked())
                        {
                            numberPause++;
                            if (numberPause >= 3)
                            {
                                numberPause = 0;
                            }
                            byte numberForLCD = numberPause;
                            numberForLCD++;
                            lcd.NumberProg(numberForLCD);
                        }
                        if (startStopButton.Clicked())
                        {
                            enterTemperaturePauses();
                            break;
                        }
                    }
                    delete numberButton;
                }
                break;
                case 1: // вкл/выкл wifi
                {
                    printSettings = true;
                    lcd.ClearAll();
                    lcd.WiFi(wifiOn);
                    while (1)
                    {
                        lcd.WiFi(wifiOn);
                        if (settingsButton.Clicked() || startStopButton.Clicked())
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
                }
                break;
                case 2: // параметры по мощности (ok)
                {
                    printSettings = true;
                    bool parametr = true;
                    lcd.ClearAll();
                    lcd.PowerLimits(parametr, powerMin, powerMax);

                    while (1)
                    {
                        // Выбор что изменять
                        if (settingsButton.Clicked())
                        {
                            break;
                        }
                        if (upButton->Clicked() || downButton->Clicked())
                        {
                            if (parametr)
                            {
                                parametr = false;
                            }
                            else
                            {
                                parametr = true;
                            }
                            lcd.ClearAll();
                            lcd.PowerLimits(parametr, powerMin, powerMax);
                        }
                        // Меняем значение
                        if (startStopButton.Clicked())
                        {
                            lcd.ClearAll();
                            if (parametr)
                            {
                                lcd.PrintLimitMenu(parametr, powerMin);
                            }
                            else
                            {
                                lcd.PrintLimitMenu(parametr, powerMax);
                            }

                            while (1)
                            {
                                if (settingsButton.Clicked() || startStopButton.Clicked())
                                {
                                    lcd.ClearAll();
                                    lcd.PowerLimits(parametr, powerMin, powerMax);
                                    regulator.setLimits(powerMin, powerMax);
                                    break;
                                }
                                if (parametr)
                                {
                                    if (upButton->Clicked())
                                    {
                                        powerMin++;
                                        lcd.ClearAll();
                                        lcd.PrintLimitMenu(parametr, powerMin);
                                    }
                                    if (downButton->Clicked())
                                    {
                                        powerMin--;
                                        lcd.ClearAll();
                                        lcd.PrintLimitMenu(parametr, powerMin);
                                    }
                                }
                                else if (!parametr)
                                {
                                    if (upButton->Clicked())
                                    {
                                        powerMax++;
                                        lcd.ClearAll();
                                        lcd.PrintLimitMenu(parametr, powerMax);
                                    }
                                    if (downButton->Clicked())
                                    {
                                        powerMax--;
                                        lcd.ClearAll();
                                        lcd.PrintLimitMenu(parametr, powerMax);
                                    }
                                }
                            }
                            // надо сохранить все данные в EEprom
                        }
                    }
                    lcd.ClearAll();
                    lcd.settingsMainMenu(2);
                }
                break;
                case 3: // выбор времени sec/min (ok!)
                {
                    printSettings = true;
                    bool parametrTime = true;
                    lcd.ClearAll();
                    lcd.TimeSettings(parametrTime);
                    while (1)
                    {
                        if (settingsButton.Clicked())
                        {
                            break;
                        }
                        if (upButton->Clicked() || downButton->Clicked())
                        {
                            if (parametrTime)
                            {
                                parametrTime = false;
                            }
                            else
                            {
                                parametrTime = true;
                            }
                            lcd.ClearAll();
                            lcd.TimeSettings(parametrTime);
                        }
                        if (startStopButton.Clicked())
                        {
                            lcd.ClearAll();
                            if (parametrTime)
                            {
                                lcd.TimeConcrete(parametrTime, timeSet);
                            }
                            else
                            {
                                lcd.TimeConcrete(parametrTime, timeDelay);
                            }
                            while (1)
                            {
                                if (settingsButton.Clicked() || startStopButton.Clicked())
                                {
                                    lcd.ClearAll();
                                    lcd.TimeSettings(parametrTime);
                                    break;
                                }
                                if ((upButton->Clicked() || downButton->Clicked()) && parametrTime)
                                {
                                    if (timeSet)
                                    {
                                        timeSet = false;
                                    }
                                    else
                                    {
                                        timeSet = true;
                                    }
                                    Serial.print("timeSet: ");
                                    Serial.println(timeSet);
                                    lcd.ClearAll();
                                    lcd.TimeConcrete(parametrTime, timeSet);
                                }
                                else if ((upButton->Clicked() || downButton->Clicked()) && !parametrTime)
                                {
                                    if (timeDelay)
                                    {
                                        timeDelay = false;
                                    }
                                    else
                                    {
                                        timeDelay = true;
                                    }
                                    lcd.ClearAll();
                                    lcd.TimeConcrete(parametrTime, timeDelay);
                                }
                            }
                        }
                    }
                    runner.putTimeSettings(timeSet, timeDelay);
                    // надо сохранить в EEprom
                }
                break;
                default:
                    break;
                }
            }
        }
        saveParametrs();
        delete upButton, downButton;
    }
}

void TRM::startProgramm()
{
    if (startStopButton.Pressed() && runner.is_programm_run())
    {
        lcd.ClearAll();
        lcd.stopProgramm();
        runner.programm_stop();
        delay(5000);
    }
    if (startStopButton.Clicked() && !runner.is_programm_run())
    {
        lcd.ClearAll();
        lcd.startProgramm();
        runner.putTimeSettings(timeSet, timeDelay);
        Serial.print("Number Programm:");
        Serial.println(numberPause);
        temperaturePausesStruct temp = collector.getPause(numberPause);
        runner.putProgramm(temp);
    }
}

void TRM::main_programm()
{
    settings();
    // motorOn();
    startProgramm();
    if (runner.is_programm_run())
    {
        runningProgramm();
    }
    else
    {
        printMainMenu(getTemperature());
    }
}

void TRM::runningProgramm()
{
    Pair<byte, uint32_t> pause = runner.runningProgramm(termoCouple.ReadCelsius());
    uint32_t time_lcd = pause.second();
    static uint32_t miniTimer;
    if (millis() - miniTimer >= 1000)
    {
        lcd.ClearAll();
        miniTimer = millis();
        if (timeSet)
        {
            lcd.workProgramm(termoCouple.ReadCelsius(), pause.first(), time_lcd);
            // lcd.workProgramm(termoCouple.ReadCelsius(), pause.first(), time_lcd / 1000);
        }
        else if (!timeSet)
        {
            lcd.workProgramm(termoCouple.ReadCelsius(), pause.first(), time_lcd);
            // lcd.workProgramm(termoCouple.ReadCelsius(), pause.first(), time_lcd / 60000);
        }
    }
}

void TRM::printMainMenu(const int &temperatureNew)
{
    static int temp;
    if (temp != temperatureNew)
    {
        temp = temperatureNew;
        byte pause = getNumberPause() + 1;
        lcd.ClearAll();
        // lcd.mainMenu(temp, pause);
        lcd.mainMenu(temp, pause, powerMax, powerMin, timeDelay, timeSet); // для отладки
    }
}

byte TRM::getPIDvalue()
{
    if (runner.is_programm_run())
    {
        return regulator.getValuePID(getTemperature());
    }
    else
    {
        return 0;
    }
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

void TRM::motorOn()
{
    if (motorButton.Pressed())
    {
        _motorState = true ? false : true;
        digitalWrite(_motorPin, _motorState);
    }
}

void TRM::saveParametrs()
{
    parametrs param{timeSet, timeDelay, powerMax, powerMin};
    EEPROM.put(250, param);
    EEPROM.commit();
}
void TRM::loadParametrs()
{
    parametrs param;
    EEPROM.get(250, param);
    timeSet = param._timeSet;
    timeDelay = param._timeDelay;
    powerMax = param._powerMax;
    powerMin = param._powerMin;
}
void TRM::baseParametrs()
{
    parametrs param;
    EEPROM.put(250, param);
}

void TRM::start_program_from_server()
{
    lcd.ClearAll();
    lcd.startProgramm();
    runner.putTimeSettings(timeSet, timeDelay);
    runner.putProgramm(collector.getPause(numberPause));
}
void TRM::stop_program_from_server()
{
    lcd.ClearAll();
    lcd.stopProgramm();
    runner.programm_stop();
}

void TRM::put_number_prog(const byte &number)
{
    numberPause = number;
}
