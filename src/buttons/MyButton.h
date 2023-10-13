#pragma once
#include <Arduino.h>

class MyButton
{
private:
    bool _flag = false;
    byte _pin;
    uint32_t _tmr = 0;

public:
    MyButton(const byte &pin);
    MyButton(){}
    void ini(const byte &pin);
    bool Clicked();
    bool Pressed();
};

