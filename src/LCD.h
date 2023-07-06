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
    void WriteTemperature(const float& temperature);
    void Clear(const byte& curs);
    const void ClearLcd(){lcd.clear();}
};

void LCD::WriteOnLcdPause(const Pair<byte, uint16_t>& pause, const byte& numberPause){
    static byte temperatureOld;
    static uint16_t timeOld;
    static byte numberPauseOld;
    if(numberPauseOld != numberPause){
        numberPauseOld = numberPause;
        Clear(1);
    }
    if(temperatureOld != pause.first() || timeOld != pause.second()){
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

void LCD::WriteTemperature(const float& temperature){
    static float oldTemp;
    if(oldTemp != temperature){
        oldTemp = temperature;
        Clear(0);
    }
    lcd.setCursor(0,0);
    lcd.print("Temperature: ");
    lcd.print(temperature);
}

void LCD::Clear(const byte& curs){
    lcd.setCursor(0, curs);
    lcd.print("                    ");
}