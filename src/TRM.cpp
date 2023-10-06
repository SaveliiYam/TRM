#include "TRM.h"

TRM::TRM(const byte &dwnBtn, const byte &upBtn, const byte &setBtn, const byte &strtBtn,
         const byte &nmbBtn, const byte &mtrBtn, const byte &mtrPin, const byte &sckPin, const byte &csPin, const byte &soPin)
    : _upButton(upBtn), _downButton(dwnBtn), _numberButton(nmbBtn), _motorPin(mtrPin)
{
    settingsButton.ini(setBtn);
    startStopButton.ini(strtBtn);
    termoCouple.ini(sckPin, csPin, soPin);
}

void TRM::ini()
{
    lcd.ini();
    loadParametrs();
    regulator.setLimits(powerMin, powerMax);
    runner.putTimeSettings(timeSet, timeDelay);
    termoCouple.Calibration(calib_value);
}

float TRM::getTemperature()
{
    return termoCouple.ReadCelsius();
}

float TRM::getCalibrationValue() const
{
    return termoCouple.GetCalibValue();
}

void TRM::enterCalibrationValue(const float &value)
{
    calib_value = value;
    termoCouple.Calibration(calib_value);
    saveParametrs();
}

byte TRM::getPredelTemperature() const { return predel_temperature; }
void TRM::enterPredelTemperature(const byte &predel)
{
    predel_temperature = predel;
    saveParametrs();
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
            // time1 = pauseEnter.time[i]; //показывает время в мс
        }
        if (!timeSet)
        {
            time1 = pauseEnter.time[i] / 60000;
            // time1 = pauseEnter.time[i]; //показывает время в мс
        }
        lcd.ClearAll();
        lcd.EnterTemperaturePause(pauseForLCD, i, false, temp1, time1, timeSet);
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
                lcd.EnterTemperaturePause(pauseForLCD, i, false, temp1, time1, timeSet);
            }
            if (upButton->Clicked())
            {
                temp1++;
                lcd.ClearAll();
                lcd.EnterTemperaturePause(pauseForLCD, i, false, temp1, time1, timeSet);
            }
        }
        lcd.ClearAll();
        lcd.EnterTemperaturePause(pauseForLCD, i, true, temp1, time1, timeSet);
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
                    if (time1 >= 120 || time1 < 0)
                    {
                        time1 = 120;
                    }
                }
                else
                {
                    if (time1 >= 7200 || time1 < 0)
                    {
                        time1 = 7200;
                    }
                }
                lcd.ClearAll();
                lcd.EnterTemperaturePause(pauseForLCD, i, true, temp1, time1, timeSet);
            }
            if (upButton->Clicked())
            {
                time1++;
                if (!timeSet)
                {
                    if (time1 > 120)
                    {
                        time1 = 0;
                    }
                }
                else
                {
                    if (time1 > 7200)
                    {
                        time1 = 0;
                    }
                }
                lcd.ClearAll();
                lcd.EnterTemperaturePause(pauseForLCD, i, true, temp1, time1, timeSet);
            }
        }
        pauseEnter.setpointTemperature[i] = temp1;
        if (timeSet)
        {
            pauseEnter.time[i] = time1 * 1000;
        }
        else if (!timeSet)
        {
            pauseEnter.time[i] = time1 * 60000;
        }
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
                            if (numberPause >= 7)
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
                case 1: // тюн и базовые
                {
                    printSettings = true;
                    byte parametrChoice = 1;
                    lcd.ClearAll();
                    lcd.TuneBaseSettings(parametrChoice);
                    while (1)
                    {
                        if (settingsButton.Clicked())
                        {
                            break;
                        }
                        if (upButton->Clicked())
                        {
                            parametrChoice++;
                            if (parametrChoice == 4)
                            {
                                parametrChoice = 1;
                            }
                            lcd.ClearAll();
                            lcd.TuneBaseSettings(parametrChoice);
                        }
                        if (downButton->Clicked())
                        {
                            parametrChoice--;
                            if (parametrChoice == 0)
                            {
                                parametrChoice = 3;
                            }
                            lcd.ClearAll();
                            lcd.TuneBaseSettings(parametrChoice);
                        }
                        if (startStopButton.Clicked())
                        {
                            bool yes_no = false;
                            lcd.ClearAll();
                            if (parametrChoice != 3)
                                lcd.TuneBaseSettings(parametrChoice, yes_no);
                            else
                                lcd.TuneBaseSettings(parametrChoice, calib_value);
                            while (1 && parametrChoice != 3)
                            {
                                if (settingsButton.Clicked())
                                {
                                    lcd.ClearAll();
                                    lcd.TuneBaseSettings(parametrChoice);
                                    break;
                                }
                                if (startStopButton.Clicked())
                                {
                                    if (!yes_no)
                                    {
                                    }
                                    else if (yes_no && !parametrChoice)
                                    { // т.н
                                        tuningPID();
                                        break;
                                    }
                                    else if (yes_no && parametrChoice)
                                    {
                                        baseParametrs();
                                    }
                                    lcd.ClearAll();
                                    lcd.TuneBaseSettings(parametrChoice);
                                    break;
                                }
                                if (upButton->Clicked() || downButton->Clicked())
                                {
                                    if (yes_no)
                                    {
                                        yes_no = false;
                                    }
                                    else
                                    {
                                        yes_no = true;
                                    }
                                    lcd.ClearAll();
                                    lcd.TuneBaseSettings(parametrChoice, yes_no);
                                }
                            }
                            while (1 && parametrChoice == 3)
                            {
                                if (settingsButton.Clicked() || startStopButton.Clicked())
                                {
                                    lcd.ClearAll();
                                    lcd.TuneBaseSettings(parametrChoice);
                                    break;
                                }
                                if (upButton->Clicked())
                                {
                                    calib_value += 0.10;
                                    lcd.ClearAll();
                                    lcd.TuneBaseSettings(parametrChoice, calib_value);
                                }
                                if (downButton->Clicked())
                                {
                                    calib_value -= 0.10;
                                    lcd.ClearAll();
                                    lcd.TuneBaseSettings(parametrChoice, calib_value);
                                }
                            }
                        }
                    }
                }
                break;
                case 2: // параметры по мощности (ok)
                {
                    printSettings = true;
                    byte parametr = 1;
                    lcd.ClearAll();
                    lcd.PowerLimits(parametr, powerMin, powerMax, predel_temperature);

                    while (1)
                    {
                        if (settingsButton.Clicked())
                        {
                            break;
                        }
                        if (upButton->Clicked())
                        {
                            parametr++;
                            if (parametr >= 4)
                            {
                                parametr = 1;
                            }
                            lcd.ClearAll();
                            lcd.PowerLimits(parametr, powerMin, powerMax, predel_temperature);
                        }
                        if (downButton->Clicked())
                        {
                            parametr--;
                            if (parametr == 0)
                            {
                                parametr = 3;
                            }
                            lcd.ClearAll();
                            lcd.PowerLimits(parametr, powerMin, powerMax, predel_temperature);
                        }
                        if (startStopButton.Clicked())
                        {
                            lcd.ClearAll();
                            if (parametr == 1)
                            {
                                lcd.PrintLimitMenu(parametr, powerMin);
                            }
                            else if (parametr == 2)
                            {
                                lcd.PrintLimitMenu(parametr, powerMax);
                            }
                            else
                            {
                                lcd.PrintLimitMenu(parametr, predel_temperature);
                            }

                            while (1 && parametr != 3)
                            {
                                if (settingsButton.Clicked() || startStopButton.Clicked())
                                {
                                    lcd.ClearAll();
                                    lcd.PowerLimits(parametr, powerMin, powerMax, predel_temperature);
                                    regulator.setLimits(powerMin, powerMax);
                                    break;
                                }
                                if (parametr == 1)
                                {
                                    if (upButton->Clicked())
                                    {
                                        powerMin++;
                                        if (powerMin > 100)
                                        {
                                            powerMin = 100;
                                        }
                                        lcd.ClearAll();
                                        lcd.PrintLimitMenu(parametr, powerMin);
                                    }
                                    if (downButton->Clicked())
                                    {
                                        powerMin--;
                                        if (powerMin == 255)
                                        {
                                            powerMin = 0;
                                        }
                                        lcd.ClearAll();
                                        lcd.PrintLimitMenu(parametr, powerMin);
                                    }
                                }
                                else if (parametr == 2)
                                {
                                    if (upButton->Clicked())
                                    {
                                        powerMax++;
                                        if (powerMax > 100)
                                        {
                                            powerMax = 100;
                                        }
                                        lcd.ClearAll();
                                        lcd.PrintLimitMenu(parametr, powerMax);
                                    }
                                    if (downButton->Clicked())
                                    {
                                        powerMax--;
                                        if (powerMax == 255)
                                        {
                                            powerMax = 0;
                                        }
                                        lcd.ClearAll();
                                        lcd.PrintLimitMenu(parametr, powerMax);
                                    }
                                }
                            }
                            while (1 && parametr == 3)
                            {
                                if (settingsButton.Clicked() || startStopButton.Clicked())
                                {
                                    lcd.ClearAll();
                                    lcd.PowerLimits(parametr, powerMin, powerMax, predel_temperature);
                                    break;
                                }
                                if (upButton->Clicked())
                                {
                                    predel_temperature++;
                                    lcd.ClearAll();
                                    lcd.PrintLimitMenu(parametr, predel_temperature);
                                }
                                if (downButton->Clicked())
                                {
                                    predel_temperature--;
                                    lcd.ClearAll();
                                    lcd.PrintLimitMenu(parametr, predel_temperature);
                                }
                            }
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
        termoCouple.Calibration(calib_value);
        delete upButton, downButton;
        lcd.ClearAll();
    }
}

void TRM::startProgramm()
{
    if (startStopButton.Clicked())
    {
        if (runner.is_programm_run())
        {
            lcd.ClearAll();
            lcd.stopProgramm();
            runner.programm_stop();
            delay(1000);
            printMainMenu(1);
        }
        else
        {
            lcd.ClearAll();
            lcd.startProgramm();
            runner.putTimeSettings(timeSet, timeDelay);
            runner.putProgramm(collector.getPause(numberPause));
            delay(1000);
        }
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
    else if (regulator.getTuneInfo())
    {
        tunningProgramm();
    }
    else
    {
        printMainMenu(getTemperature());
    }
}

void TRM::runningProgramm()
{
    Third<byte, uint32_t, byte> pause = runner.runningProgramm(termoCouple.ReadCelsius());
    byte pause1 = pause.first() + 1;
    uint32_t time_lcd = pause.second();
    static byte setpointTemp;
    if (setpointTemp != pause.third())
    {
        setpointTemp = pause.third();
        regulator.putTemperature(setpointTemp);
    }
    static uint32_t miniTimer;
    if (millis() - miniTimer >= 1000)
    {
        // lcd.ClearAll();
        miniTimer = millis();
        if (timeSet)
        {
            // lcd.workProgramm(termoCouple.ReadCelsius(), pause.first(), time_lcd);
            lcd.workProgramm(setpointTemp, termoCouple.ReadCelsius(), pause1, time_lcd / 1000, timeSet);
        }
        else if (!timeSet)
        {
            // lcd.workProgramm(termoCouple.ReadCelsius(), pause.first(), time_lcd);
            lcd.workProgramm(setpointTemp, termoCouple.ReadCelsius(), pause1, time_lcd / 60000 + 1, timeSet);
        }
    }
}
void TRM::tunningProgramm()
{
    lcd.TuningProcces(regulator.GetParametrsLCD().second(), termoCouple.ReadCelsius());
}

void TRM::printMainMenu(const int &temperatureNew)
{
    static int temp;
    if (temp != temperatureNew)
    {
        temp = temperatureNew;
        byte pause = getNumberPause() + 1;
        // lcd.ClearAll();
        //  lcd.mainMenu(temp, pause);
        lcd.mainMenu(temp, pause, powerMax, powerMin, timeDelay, timeSet); // для отладки
    }
}

byte TRM::getPIDvalue()
{
    if (runner.is_programm_run())
    {
        if (getTemperature() >= predel_temperature)
        {
            return 0;
        }
        return regulator.getValuePID(getTemperature());
    }
    else if (regulator.getTuneInfo())
    {
        return regulator.GetPIDValueTune(getTemperature()).first();
    }
    else
    {
        return 0;
    }
}

void TRM::tuningPID()
{
    MyButton *upButton = new MyButton(_upButton);
    MyButton *downButton = new MyButton(_downButton);
    byte temperatureMax = 80;
    lcd.ClearAll();
    lcd.Tuning(temperatureMax);
    while (1)
    {
        if (upButton->Clicked())
        {
            temperatureMax++;
            lcd.Tuning(temperatureMax);
        }
        if (downButton->Clicked())
        {
            temperatureMax--;
            lcd.Tuning(temperatureMax);
        }
        if (settingsButton.Clicked())
        {
            break;
        }
        if (startStopButton.Clicked())
        {
            regulator.tuneInitialization(temperatureMax);
            break;
        }
    }
    delete upButton, downButton;
    return;
}

void TRM::saveParametrs()
{
    parametrs param{timeSet, timeDelay, powerMax, powerMin, calib_value, predel_temperature};
    regulator.setLimits(powerMin, powerMax);
    runner.putTimeSettings(timeSet, timeDelay);
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
    calib_value = param._calibr;
    predel_temperature = param._predel;
}
void TRM::baseParametrs()
{
    parametrs param(false, false, 100, 0, 0.0, 101);
    EEPROM.put(250, param);
    timeSet = param._timeSet;
    timeDelay = param._timeDelay;
    powerMax = param._powerMax;
    powerMin = param._powerMin;
    calib_value = param._calibr;
    predel_temperature = 101;
    runner.putTimeSettings(timeSet, timeDelay);
    regulator.setLimits(powerMin, powerMax);
    regulator.baseKoefficients();
    temperaturePausesStruct programms{{0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}};
    EEPROM.put(0, programms);
    EEPROM.put(35, programms);
    EEPROM.put(70, programms);
    EEPROM.put(105, programms);
    EEPROM.put(140, programms);
    EEPROM.put(175, programms);
    EEPROM.put(210, programms);
    EEPROM.commit();
}

void TRM::start_program_from_server(const bool &start)
{
    if (start && !runner.is_programm_run())
    {
        lcd.ClearAll();
        lcd.startProgramm();
        delay(500);
        runner.putTimeSettings(timeSet, timeDelay);
        runner.putProgramm(collector.getPause(numberPause));
    }
}
void TRM::stop_program_from_server(const bool &stop)
{
    if (stop && runner.is_programm_run())
    {
        lcd.ClearAll();
        lcd.stopProgramm();
        runner.programm_stop();
        delay(500);
        printMainMenu(10);
    }
}

void TRM::put_number_prog(const byte &number)
{
    numberPause = number;
    numberPause--;
}

void TRM::save_parametrs_time(const byte &what, const bool &value)
{
    switch (what)
    {
    case 1:
    {
        timeSet = value;
        break;
    }
    case 2:
    {
        timeDelay = value;
        break;
    }
    }
    saveParametrs();
}
void TRM::save_parametrs_power(const byte &what, const byte &value)
{
    switch (what)
    {
    case 3:
    {
        powerMax = value;
        break;
    }
    case 4:
    {
        powerMin = value;
        break;
    }
    }
    saveParametrs();
}
