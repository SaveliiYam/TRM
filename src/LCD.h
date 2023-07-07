#pragma once
#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "Pair.h"
#include "Simbols.h"

class LCD
{
private:
    LiquidCrystal_I2C lcd{0x27, 20, 4};

public:
    LCD()
    {
        lcd.init();
        lcd.backlight();
        lcd.setCursor(0, 0);
        lcd.createChar(0, RightArrow);
    }
    void Point(const byte &parametr);
    void WriteOnLcdPause(const Pair<byte, uint16_t> &pause, const byte &numberPause);
    void WriteTemperature(const float &temperature);
    void Clear(const byte &curs);
    const void ClearLcd() { lcd.clear(); }
    void settingsMainMenu(const byte &parametr);
    void WiFi(const bool &parametr);
};

void LCD::Point(const byte &parametr)
{
    lcd.setCursor(0, parametr);
    lcd.print(char(0));
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
    static byte parametrOld = 10; // Просто чтобы он показывал сразу
    if (parametrOld != parametr)
    {
        parametrOld = parametr;
        lcd.clear();
        Point(parametrOld);
        lcd.setCursor(1, 0);
        lcd.print("Pause settings");
        lcd.setCursor(1, 1);
        lcd.print("WiFi on/off");
        lcd.setCursor(1, 2);
        lcd.print("Power limits");
        lcd.setCursor(1, 3);
        lcd.print("sec/min settings");
    }
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