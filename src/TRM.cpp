#include "TRM.h"

TRM::TRM(const byte &dwnBtn, const byte &upBtn, const byte &setBtn, const byte &strtBtn,
         const byte &nmbBtn, const byte &mtrBtn, const byte &mtrPin, const byte &sckPin, const byte &csPin, const byte &soPin)
    : _upButton(upBtn), _downButton(dwnBtn), _motorPin(mtrPin)
{
    settingsButton.ini(setBtn);
    startStopButton.ini(strtBtn);
    upButton.ini(upBtn);
    downButton.ini(dwnBtn);
    numberButton.ini(nmbBtn);
    termoCouple.ini(sckPin, csPin, soPin);
}

void TRM::ini()
{
    lcd.ini();
    loadParametrs();
    regulator.loadKoefficients();
    regulator.setLimits(powerMin, powerMax);
    runner.putTimeSettings(timeSet, timeDelay);
    termoCouple.Calibration(calib_value);
}

float TRM::getTemperature()
{
    return termoCouple.ReadCelsius();
}

int TRM::getCalibrationValue() const
{
    return termoCouple.GetCalibValue();
}

void TRM::enterCalibrationValue(const int &value)
{
    if (calib_value == value)
    {
        return;
    }
    calib_value = constrain(value, -100, 100);
    termoCouple.Calibration(calib_value);
    saveParametrs();
}

byte TRM::getPredelTemperature() const { return predel_temperature; }
void TRM::enterPredelTemperature(const byte &predel)
{
    if (predel_temperature == predel)
    {
        return;
    }
    predel_temperature = predel;
    saveParametrs();
}

byte TRM::getNumberPause() const { return numberPause; }

// Кнопки up down создаются через динамическую память
void TRM::enterTemperaturePauses()
{
    temperaturePausesStruct pauseEnter = collector.getPause(numberPause);
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
                return;
            }
            if (upButton.Clicked())
            {
                temp1--;
                temp1 = constrain(temp1, 0, predel_temperature);
                lcd.ClearAll();
                lcd.EnterTemperaturePause(pauseForLCD, i, false, temp1, time1, timeSet);
            }
            if (downButton.Clicked())
            {
                temp1++;
                temp1 = constrain(temp1, 0, predel_temperature);
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
                return;
            }
            if (upButton.Clicked())
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
            if (downButton.Clicked())
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
}

void TRM::checkNumberProgButton()
{
    if (!numberButton.Clicked() || runner.is_programm_run())
    {
        return;
    }
    numberPause++;
    if (numberPause >= 7)
    {
        numberPause = 0;
    }
    byte number = numberPause;
    number++;
    lcd.mainMenu(getTemperature(), number, powerMax, powerMin, timeDelay, timeSet);
}

void TRM::settings()
{ // настройки 1-паузы, 2-вкл?выкл wifi, 3-параметры по мощности, 4-диапазон времени(сек/мин), 5- отложенное время
    if (!settingsButton.Clicked())
    {
        return;
    }
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
        if (settingsButton.Clicked())
        {
            lcd.ClearAll();
            printMainMenu(0);
            break;
        }

        if (upButton.Clicked())
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
        if (downButton.Clicked())
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
                    if (numberButton.Clicked())
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
                    if (upButton.Clicked())
                    {
                        parametrChoice++;
                        if (parametrChoice >= 5)
                        {
                            parametrChoice = 1;
                        }
                        lcd.ClearAll();
                        lcd.TuneBaseSettings(parametrChoice);
                    }
                    if (downButton.Clicked())
                    {
                        parametrChoice--;
                        if (parametrChoice == 0)
                        {
                            parametrChoice = 4;
                        }
                        lcd.ClearAll();
                        lcd.TuneBaseSettings(parametrChoice);
                    }
                    if (startStopButton.Clicked())
                    {
                        bool yes_no = false;
                        lcd.ClearAll();
                        if (parametrChoice < 3)
                            lcd.TuneBaseSettings(parametrChoice, yes_no);
                        else if (parametrChoice == 3)
                            lcd.TuneBaseSettings(parametrChoice, calib_value);
                        else
                            lcd.TuneBaseSettings(parametrChoice, oper_mode);
                        while (1 && parametrChoice < 3)
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
                                    lcd.ClearAll();
                                    lcd.TuneBaseSettings(parametrChoice);
                                    break;
                                }
                                else if (yes_no && parametrChoice == 2)
                                { // т.н
                                    tuningPID();
                                    break;
                                }
                                else if (yes_no && parametrChoice == 1)
                                {
                                    baseParametrs();
                                }
                                lcd.ClearAll();
                                lcd.TuneBaseSettings(parametrChoice);
                                break;
                            }
                            if (upButton.Clicked() || downButton.Clicked())
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
                            if (upButton.Clicked())
                            {
                                calib_value++;
                                lcd.ClearAll();
                                lcd.TuneBaseSettings(parametrChoice, calib_value);
                            }
                            if (downButton.Clicked())
                            {
                                calib_value--;
                                lcd.ClearAll();
                                lcd.TuneBaseSettings(parametrChoice, calib_value);
                            }
                        }
                        while (1 && parametrChoice == 4)
                        {
                            if (settingsButton.Clicked() || startStopButton.Clicked())
                            {
                                lcd.ClearAll();
                                lcd.TuneBaseSettings(parametrChoice);
                                break;
                            }
                            if (upButton.Clicked() || downButton.Clicked())
                            {
                                if (oper_mode)
                                    oper_mode = false;
                                else
                                    oper_mode = true;
                                lcd.ClearAll();
                                lcd.TuneBaseSettings(parametrChoice, oper_mode);
                            }
                        }
                    }
                }
                break;
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
                    if (upButton.Clicked())
                    {
                        parametr++;
                        if (parametr >= 5)
                        {
                            parametr = 1;
                        }
                        lcd.ClearAll();
                        lcd.PowerLimits(parametr, powerMin, powerMax, predel_temperature);
                    }
                    if (downButton.Clicked())
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
                                if (upButton.Clicked())
                                {
                                    powerMin++;
                                    if (powerMin > 100)
                                    {
                                        powerMin = 100;
                                    }
                                    lcd.ClearAll();
                                    lcd.PrintLimitMenu(parametr, powerMin);
                                }
                                if (downButton.Clicked())
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
                                if (upButton.Clicked())
                                {
                                    powerMax++;
                                    if (powerMax > 100)
                                    {
                                        powerMax = 100;
                                    }
                                    lcd.ClearAll();
                                    lcd.PrintLimitMenu(parametr, powerMax);
                                }
                                if (downButton.Clicked())
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
                            if (upButton.Clicked())
                            {
                                predel_temperature++;
                                lcd.ClearAll();
                                lcd.PrintLimitMenu(parametr, predel_temperature);
                            }
                            if (downButton.Clicked())
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
                    if (upButton.Clicked() || downButton.Clicked())
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
                            if ((upButton.Clicked() || downButton.Clicked()) && parametrTime)
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
                            else if ((upButton.Clicked() || downButton.Clicked()) && !parametrTime)
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
    lcd.ClearAll();
}

void TRM::startProgramm()
{
    if (oper_mode)
    {
        return;
    }
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
    checkNumberProgButton();
    if (runner.is_programm_run())
    {
        runningProgramm();
    }
    else if (regulator.getTuneInfo())
    {
        tunningProgramm();
    }
    else if (oper_mode)
    {
        poweringProgramm();
    }
    else
    {
        printMainMenu(getTemperature());
        set_temp_for_th = 0;
        set_time_for_th = 0;
        number_pause_for_th = 0;
    }
}

void TRM::poweringProgramm()
{
    if (upButton.Clicked())
    {
        power++;
        if (power > 100)
            power = 0;
    }
    if (downButton.Clicked())
    {
        power--;
        if (power > 100)
            power = 100;
    }
    lcd.Powering(power, getTemperature());
}

void TRM::runningProgramm()
{
    Third<byte, uint32_t, byte> pause = runner.runningProgramm(termoCouple.ReadCelsius());
    byte pause1 = pause.first() + 1;
    number_pause_for_th = pause1;
    uint32_t time_lcd = pause.second();
    if (set_temp_for_th != pause.third())
    {
        set_temp_for_th = pause.third();
        regulator.putTemperature(set_temp_for_th);
    }
    static uint32_t miniTimer;
    if (millis() - miniTimer >= 1000)
    {
        // lcd.ClearAll();
        miniTimer = millis();
        if (timeSet)
        {
            // lcd.workProgramm(termoCouple.ReadCelsius(), pause.first(), time_lcd);
            set_time_for_th = time_lcd / 1000;
            lcd.workProgramm(set_temp_for_th, termoCouple.ReadCelsius(), pause1, set_time_for_th, timeSet);
        }
        else if (!timeSet)
        {
            // lcd.workProgramm(termoCouple.ReadCelsius(), pause.first(), time_lcd);
            set_time_for_th = time_lcd / 60000;
            lcd.workProgramm(set_temp_for_th, termoCouple.ReadCelsius(), pause1, set_time_for_th + 1, timeSet);
        }
    }
}
void TRM::tunningProgramm()
{
    lcd.TuningProcces(regulator.GetParametrsLCD().second(), termoCouple.ReadCelsius());
}

void TRM::printMainMenu(const float &temperatureNew)
{
    static float temp;
    if (temp == temperatureNew)
    {
        return;
    }
    temp = temperatureNew;
    byte pause = getNumberPause() + 1;
    // lcd.ClearAll();
    //  lcd.mainMenu(temp, pause);
    lcd.mainMenu(temp, pause, powerMax, powerMin, timeDelay, timeSet); // для отладки
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
    else if (oper_mode)
    {
        if (getTemperature() >= predel_temperature)
        {
            return 0;
        }
        return map(power, 0, 100, 0, 255);
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
    if (oper_mode)
    {
        return;
    }
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
    if (oper_mode)
    {
        return;
    }
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
    printMainMenu(10);
}

void TRM::save_parametrs_time(const byte &what, const bool &value)
{
    switch (what)
    {
    case 1:
    {
        if (timeSet != value)
        {
            timeSet = value;
            saveParametrs();
        }
        break;
    }
    case 2:
    {
        if (timeDelay != value)
        {
            timeDelay = value;
            saveParametrs();
        }
        break;
    }
    }
}
void TRM::save_parametrs_power(const byte &what, const byte &value)
{
    switch (what)
    {
    case 3:
    {
        if (powerMax != value)
        {
            powerMax = constrain(value, 0, 100);
            saveParametrs();
        }
        break;
    }
    case 4:
    {
        if (powerMin != value)
        {
            powerMin = constrain(value, 0, 100);
            saveParametrs();
        }
        break;
    }
    }
}

void TRM::enterMode(const bool &mode)
{
    if (oper_mode != mode)
    {
        oper_mode = mode;
    }
    if (!mode)
    {
        printMainMenu(10);
    }
}

void TRM::enterPowerValueNow(const int &value)
{
    power = value;
}