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

void LCD::ConnectToWifi(const bool &connect)
{
    lcd.setCursor(0, 0);
    lcd.print("Do you want to");
    lcd.setCursor(0, 1);
    lcd.print("connect to wifi?");
    if (connect)
    {
        Point(2);
    }
    else
    {
        Point(3);
    }
    lcd.setCursor(1, 2);
    lcd.print("Yes");
    lcd.setCursor(1, 3);
    lcd.print("No");
}

void LCD::workProgramm(const int &setpointTemperature, const float &temperature, const byte &numberPause, const uint32_t &time, const char *timeParametr)
{
    lcd.setCursor(0, 0);
    lcd.print("Setpoint: ");
    lcd.print(setpointTemperature);
    lcd.print(probel);
    lcd.setCursor(0, 1);
    lcd.print("Temperature: ");
    lcd.print(temperature);
    lcd.print("  ");
    lcd.setCursor(0, 2);
    lcd.print("Time: ");
    lcd.print(time);
    lcd.print(timeParametr);
    lcd.setCursor(0, 3);
    lcd.print("Pause: ");
    lcd.print(numberPause);
    lcd.print(probel);
}

void LCD::mainMenu(const int &temperature, const byte &number)
{
    lcd.setCursor(0, 0);
    lcd.print("Number prog: ");
    lcd.print(number);
    lcd.print(probel);
    lcd.setCursor(0, 1);
    lcd.print("Temperature: ");
    lcd.print(temperature);
    lcd.print(probel);
}

void LCD::mainMenu(const int &temperature, const byte &number, const byte max, const byte &min, const bool &delay, const bool &set)
{
    lcd.setCursor(0, 0);
    lcd.print("Number prog: ");
    lcd.print(number);
    lcd.print(probel);
    lcd.setCursor(0, 1);
    lcd.print("Temperature: ");
    lcd.print(temperature);
    lcd.print(probel);

    // lcd.setCursor(0, 2);
    // lcd.print("Max: "); lcd.print(max);
    // lcd.print(" Min: "); lcd.print(min);
    // lcd.setCursor(0, 3);
    // lcd.print("Set: "); lcd.print(set);
    // lcd.print(" Del: "); lcd.print(delay);
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
void LCD::Connecting()
{
    lcd.setCursor(0, 0);
    lcd.print("Connect...");
}

void LCD::NumberProg(const byte &numberProgramm)
{
    lcd.setCursor(0, 0);
    lcd.print("Number programm: ");
    lcd.print(numberProgramm);
    lcd.print(probel);
}

void LCD::TimeConcrete(const bool &parametr, const bool &value)
{
    if (parametr)
    {
        if (value)
        {
            Point(1);
            Serial.println("Point 1");
        }
        else if (!value)
        {
            Point(2);
            Serial.println("Point 2");
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
        else if (!value)
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
    byte min = map(minLimit, 0, 255, 0, 100);
    byte max = map(maxLimit, 0, 255, 0, 100);
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
    lcd.print(min);
    lcd.print('%');
    lcd.setCursor(1, 2);
    lcd.print("maxLimit: ");
    lcd.print(max);
    lcd.print('%');
}

void LCD::PrintLimitMenu(const bool &parametr, const byte &limit)
{
    byte lim = map(limit, 0, 255, 0, 100);
    lcd.setCursor(1, 0);
    if (parametr)
    {
        lcd.print("MinLimit: ");
        lcd.print(lim);
    }
    else
    {
        lcd.print("MaxLimit: ");
        lcd.print(lim);
    }
    lcd.print('%');
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
    lcd.print("Other settings");
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

void LCD::TuneBaseSettings(const bool &parametr)
{
    lcd.setCursor(0, 0);
    lcd.print("Other Settings");
    if (parametr)
    {
        Point(1);
    }
    else
    {
        Point(2);
    }
    lcd.setCursor(1, 1);
    lcd.print("Base koefficients");
    lcd.setCursor(1, 2);
    lcd.print("Tune Settings");
}
void LCD::TuneBaseSettings(const bool &parametr, const bool &choise)
{
    lcd.setCursor(0, 0);
    if (parametr)
    {
        lcd.print("Base Koeff");
    }
    else
    {
        lcd.print("Tune Settings");
    }
    if (choise)
    {
        Point(1);
    }
    else
    {
        Point(2);
    }
    lcd.setCursor(1, 1);
    lcd.print("Yes");
    lcd.setCursor(1, 2);
    lcd.print("No");
}

void LCD::Tuning(const byte &temperatureMax)
{
    lcd.setCursor(0, 0);
    lcd.print("Enter max temp: ");
    lcd.print(temperatureMax);
    lcd.print(probel);
}
void LCD::TuningProcces(const byte &percent, const byte &temperatureNow)
{
    lcd.setCursor(0, 0);
    lcd.print("Percent: ");
    lcd.print(percent);
    lcd.print(probel);
    lcd.setCursor(0, 1);
    lcd.print("Temperature: ");
    lcd.print(temperatureNow);
    lcd.print(probel);
}