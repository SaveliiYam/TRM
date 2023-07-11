#define input_pin 2
#define output_pin 3
#define upButton 17
#define downButton 33
#define settingsButtn 18
#define startButton 19
#define mtrButton 32
#define numberButton 23
#define sck 25
#define cs 26
#define so 27
#define mtrRelay 5

#include <GyverDimmer.h>
#include "TimerUs.h"
#include "TRM.h"

/*
    TRM(const byte& dwnBtn, const byte& upBtn, const byte& setBtn, const byte& strtBtn,
        const byte& nmbBtn, const byte& mtrBtn, const byte& mtrPin , const byte& sckPin, const byte& csPin, const byte& soPin);
*/

Dimmer<output_pin> dimmer;
TimerUs timer;
TRM trm{downButton, upButton, settingsButtn, startButton, numberButton, mtrButton, mtrRelay, sck, cs, so};
void setup()
{
    attachInterrupt(input_pin, isr, RISING);
}

void loop()
{
    trm.main_programm();
    // в "прерывании" таймера
    if (timer.ready())
    {
        dimmer.tickTimer(); // вызвать tickTimer()
        timer.stop();    // остановить таймер
    }
    dimmer.write(trm.getPIDvalue()); // принимает 0-255
}

void isr()
{
    // вызывать в прерывании детектора нуля
    // если tickZero() - true - нужно перезапустить таймер с периодом getPeriod()
    if (dimmer.tickZero())
        timer.setPeriod(dimmer.getPeriod());
    else
        timer.restart();
    // иначе перезапустить со старым
}