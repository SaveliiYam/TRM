#pragma once
#include <Arduino.h>

class MyButton
{
private:
    bool _flag = false;
    byte _pin;
    uint32_t _tmr = 0;

public:
    MyButton(const byte &pin) : _pin(pin) {}
    MyButton(){}
    void ini(const byte &pin);
    bool Clicked();
    bool Pressed();
};

void MyButton::ini(const byte &pin){_pin = pin;}

bool MyButton::Clicked()
{
    bool btnState = digitalRead(_pin);
    if (!btnState && !_flag && millis() - _tmr >= 100)
    {
        _flag = true;
        _tmr = millis();
        return true;
    }
    if (!btnState && _flag && millis() - _tmr >= 500)
    {
        _tmr = millis();
    }
    if (btnState && _flag)
    {
        _flag = false;
        _tmr = millis();
    }
    return false;
}
bool MyButton::Pressed()
{
    bool btnState = digitalRead(_pin);
    if (!btnState && !_flag && millis() - _tmr >= 100)
    {
        _flag = true;
        _tmr = millis();
        return false;
    }
    if (!btnState && _flag && millis() - _tmr >= 500)
    {
        _tmr = millis();
        return true;
    }
    if (btnState && _flag)
    {
        _flag = false;
        _tmr = millis();
    }
    return false;
}