#pragma once
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

class LCDScreen{
private:
    LiquidCrystal_I2C lcd{0x27, 4, 20};
public:
    LCDScreen(){
        
    }

    
};