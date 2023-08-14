#include <GyverDimmer.h>
#include "TimerUs.h"
#include "TRM.h"
#include "LCD.h"
#include <LiquidCrystal_I2C.h>
// #include <ThingerESP32.h>
// #include "arduino_secrets.h"
// #define TINGER_SERIAL_DEBUG
//ThingerESP32 thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);

void isr();

const byte input_pin = 32;
const byte output_pin = 33;
const byte upButton = 23;
const byte downButton = 19;
const byte settingsButtn = 18;
const byte startButton = 5;
const byte mtrButton = 16;
const byte numberButton = 17;
const byte sck = 12;
const byte cs = 14;
const byte so = 27;
const byte mtrRelay = 14;
/*
    TRM(const byte& dwnBtn, const byte& upBtn, const byte& setBtn, const byte& strtBtn,
        const byte& nmbBtn, const byte& mtrBtn, const byte& mtrPin , const byte& sckPin, const byte& csPin, const byte& soPin);
*/

Dimmer<output_pin> dimmer;
TimerUs timer;

void setup()
{
    Serial.begin(9600);
    attachInterrupt(input_pin, isr, RISING);

    // thing["temperature"] >> [](pson &out)
    // {
    //     out["temperature"] = trm.getTemperature();
    // };

    LiquidCrystal_I2C lcd(0x27, 20, 4);
    lcd.init();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("hi");

    delay(500);
    //LCD lcdClass(lcd);
    TRM trm{downButton, upButton, settingsButtn, startButton, numberButton, mtrButton, mtrRelay, sck, cs, so};
    while (1)
    {
        trm.main_programm();
    }
    
    
    //trm.getLCD(lcdClass);
}

void loop()
{
    //trm.main_programm();
    // в "прерывании" таймера
    if (timer.ready())
    {
        dimmer.tickTimer(); // вызвать tickTimer()
        timer.stop();       // остановить таймер
    }
    //dimmer.write(trm.getPIDvalue()); // принимает 0-255
    //thing.handle();
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