#include <GyverDimmer.h>
#include "TimerUs.h"
#include "TRM.h"


void isr();

const byte input_pin = 32;
const byte output_pin = 33;
const byte upButton = 23;
const byte downButton = 19;
const byte settingsButtn = 18;
const byte startButton = 5;
const byte mtrButton = 16;
const byte numberButton = 17;
const byte sck = 27;
const byte cs = 26;
const byte so = 25;
const byte mtrRelay = 14;
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