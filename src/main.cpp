

#include "TRM.h"
#include "LCD.h"
#include <LiquidCrystal_I2C.h>
// #include <ThingerESP32.h>
// #include "arduino_secrets.h"
// #define TINGER_SERIAL_DEBUG
//ThingerESP32 thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);
int i = 0;

const byte input_pin = 25;
const byte output_pin = 26;
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

    //TRM(const byte& dwnBtn, const byte& upBtn, const byte& setBtn, const byte& strtBtn,
    //    const byte& nmbBtn, const byte& mtrBtn, const byte& mtrPin , const byte& sckPin, const byte& csPin, const byte& soPin);



void setup()
{
    Serial.begin(9600);
    EEPROM.begin(300);

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

    
}

void loop()
{
}




/*
// тест одноканального диммера
// библиотека универсальная, поэтому требуется свой таймер
// в этом примере делаем на микросе!
#include <Arduino.h>

int a = 20, b;
bool high_low = false;
uint32_t timer = millis();

void setup(){
    pinMode(26, OUTPUT);
}

void loop(){
    //a-=5;
    // b+=5;

    if(a == 0){a = 255;}
    b = map(a, 0, 255, 255, 0);

    if(high_low && (millis() - timer >= b)){
        digitalWrite(26, HIGH);
        timer = millis();
        high_low = false;
    }
    if(!high_low && (millis() - timer >= a)){
        digitalWrite(26, LOW);
        timer = millis();
        high_low = true;
    }

    
}
*/



