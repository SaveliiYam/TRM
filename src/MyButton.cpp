#include "MyButton.h"

MyButton::MyButton(const byte &pin) : _pin(pin) {
        pinMode(_pin, INPUT_PULLUP);
    }

void MyButton::ini(const byte &pin){
    _pin = pin;
    pinMode(_pin, INPUT_PULLUP);
}

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