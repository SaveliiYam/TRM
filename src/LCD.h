#pragma once
#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "Pair.h"

class LCD{
private:
    LiquidCrystal_I2C lcd{0x27, 20, 4};
public:
    LCD(){
        lcd.init();
        lcd.backlight();
        lcd.setCursor(0,0);
    }
    void WriteOnLcdPause(const Pair<byte, uint16_t>& pause, const byte& numberPause);
    void WriteTemperature(const byte& temperature);
    void Clear(const byte& curs);
    const void ClearLcd(){lcd.clear();}
};

void LCD::WriteOnLcdPause(const Pair<byte, uint16_t>& pause, const byte& numberPause){
    Clear(1);
    Clear(2);
    lcd.setCursor(0, 1);
    lcd.print("Number pause: ");
    lcd.print(numberPause);
    lcd.setCursor(0, 2);
    lcd.print("NeedTemp");
    lcd.print(pause.first());
    lcd.print(" Time: ");
    lcd.print(pause.second());
}

void LCD::WriteTemperature(const byte& temperature){
    Clear(0);
    lcd.setCursor(0,0);
    lcd.print("Temperature: ");
    lcd.print(temperature);
}

void LCD::Clear(const byte& curs){
    lcd.setCursor(0, curs);
    lcd.print("                    ");
}