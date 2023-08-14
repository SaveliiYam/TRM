#include "LCD.h"

LCD::LCD()
{
    lcd.init();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("hi");
    lcd.createChar(0, RightArrow);
}

LCD::LCD(const LiquidCrystal_I2C &lcdNew)
{
    lcd = lcdNew;
    lcd.createChar(0, RightArrow);
}

void LCD::ClearAll() { lcd.clear(); }

void LCD::Point(const byte &parametr)
{
    lcd.setCursor(0, parametr);
    lcd.print(char(0));
}

void LCD::workProgramm(const float &temperature, const byte &numberPause, const uint16_t time)
{
    lcd.setCursor(0, 0);
    lcd.print("Temperature: ");
    lcd.print(temperature);
    lcd.setCursor(0, 1);
    lcd.print("Time: ");
    lcd.print(time);
    lcd.setCursor(0, 2);
    lcd.print("Pause: ");
    lcd.print(numberPause);
}

void LCD::mainMenu(const int &temperature, const byte &number)
{
    lcd.setCursor(0, 0);
    lcd.print("Number prog: ");
    lcd.print(number);
    lcd.setCursor(0, 1);
    lcd.print("Temperature: ");
    lcd.print(temperature);
}

void LCD::stopProgramm()
{
    lcd.setCursor(0, 0);
    lcd.print("Programm stop");
}

void LCD::startProgramm()
{
    lcd.setCursor(0, 0);
    lcd.print("Programm start");
}

void LCD::BaseSettings(const bool &parametr)
{
    if (parametr)
        Point(2);
    else
        Point(1);
    lcd.setCursor(0, 0);
    lcd.print("Return base setting");
    lcd.setCursor(1, 1);
    lcd.print("No");
    lcd.setCursor(1, 2);
    lcd.print("Yes");
}

void LCD::EnterTemperaturePause(byte numberProgramm, byte step, const bool &parametr, const byte &temperature, const uint16_t &time)
{
    NumberProg(numberProgramm);
    lcd.setCursor(0, 1);
    lcd.print("Step: ");
    lcd.print(step);
    if (!parametr)
        Point(2);
    else
        Point(3);
    lcd.setCursor(1, 2);
    lcd.print("Set temp: ");
    lcd.print(temperature);
    lcd.setCursor(1, 3);
    lcd.print("Set time: ");
    lcd.print(time);
}

void LCD::NumberProg(const byte& numberProgramm)
{
    lcd.setCursor(0, 0);
    lcd.print("Number programm: ");
    lcd.print(numberProgramm);
}

void LCD::TimeConcrete(const bool &parametr, const bool &value)
{
    if (parametr)
    {
        if (value)
        {
            Point(1);
        }
        else
        {
            Point(2);
        }
        lcd.setCursor(1, 0);
        lcd.print("min/sec");
        lcd.setCursor(1, 1);
        lcd.print("sec");
        lcd.setCursor(1, 2);
        lcd.print("min");
    }
    else
    {
        if (value)
        {
            Point(1);
        }
        else
        {
            Point(2);
        }
        lcd.setCursor(1, 0);
        lcd.print("time delay");
        lcd.setCursor(1, 1);
        lcd.print("no delay");
        lcd.setCursor(1, 2);
        lcd.print("delay");
    }
}

void LCD::TimeSettings(const bool &parametr)
{
    if (parametr)
    {
        Point(1);
    }
    else
    {
        Point(2);
    }
    lcd.setCursor(1, 0);
    lcd.print("Time Settings");
    lcd.setCursor(1, 1);
    lcd.print("min/sec");
    lcd.setCursor(1, 2);
    lcd.print("delay time");
}

void LCD::PowerLimits(const bool &parametr, const byte &minLimit, const byte &maxLimit)
{
    if (parametr)
    {
        Point(1);
    }
    else
    {
        Point(2);
    }
    lcd.setCursor(1, 0);
    lcd.print("Power limits:");
    lcd.setCursor(1, 1);
    lcd.print("minLimit: ");
    lcd.print(minLimit);
    lcd.setCursor(1, 2);
    lcd.print("maxLimit: ");
    lcd.print(maxLimit);
}

void LCD::PrintLimitMenu(const bool &parametr, const byte &limit)
{
    lcd.setCursor(1, 0);
    if (parametr)
    {
        lcd.print("MinLimit: ");
        lcd.print(limit);
    }
    else
    {
        lcd.print("MaxLimit: ");
        lcd.print(limit);
    }
}

void LCD::WiFi(const bool &parametr)
{
    if (parametr)
    {
        Point(1);
    }
    else
    {
        Point(2);
    }
    lcd.setCursor(1, 0);
    lcd.print("WiFi on/off");
    lcd.setCursor(1, 1);
    lcd.print("On");
    lcd.setCursor(1, 2);
    lcd.print("Off");
}

void LCD::settingsMainMenu(const byte &parametr)
{
    lcd.clear();
    Point(parametr);
    lcd.setCursor(1, 0);
    lcd.print("Pause settings");
    lcd.setCursor(1, 1);
    lcd.print("WiFi on/off");
    lcd.setCursor(1, 2);
    lcd.print("Power limits");
    lcd.setCursor(1, 3);
    lcd.print("sec/min settings");
}

void LCD::WriteOnLcdPause(const Pair<byte, uint16_t> &pause, const byte &numberPause)
{
    static byte temperatureOld;
    static uint16_t timeOld;
    static byte numberPauseOld;
    if (numberPauseOld != numberPause)
    {
        numberPauseOld = numberPause;
        Clear(1);
    }
    if (temperatureOld != pause.first() || timeOld != pause.second())
    {
        temperatureOld = pause.first();
        timeOld = pause.second();
        Clear(2);
    }
    lcd.setCursor(0, 1);
    lcd.print("Number pause: ");
    lcd.print(numberPauseOld);
    lcd.setCursor(0, 2);
    lcd.print("NeedTemp");
    lcd.print(temperatureOld);
    lcd.print(" Time: ");
    lcd.print(timeOld);
}

void LCD::WriteTemperature(const float &temperature)
{
    static float oldTemp;
    if (oldTemp != temperature)
    {
        oldTemp = temperature;
        Clear(0);
    }
    lcd.setCursor(0, 0);
    lcd.print("Temperature: ");
    lcd.print(temperature);
}

void LCD::Clear(const byte &curs)
{
    lcd.setCursor(0, curs);
    lcd.print("                    ");
}