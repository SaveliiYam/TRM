#include "TRM.h"
#include "LCD.h"
#include <LiquidCrystal_I2C.h>
#include <SimplePortal.h>
#include <ThingerESP32.h>
#include <EEPROM.h>
#include "arduino_secrets.h"
#include "MyButton.h"
#include "Fouth.h"
#define TINGER_SERIAL_DEBUG
ThingerESP32 thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);

int a = 0, b;
bool high_low = false;
bool start_programm = false;
float temp;
uint32_t timer = millis();
Fouth<bool, bool, byte, byte> parametrs{false, false, 0, 255};

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

void connectWiFi();
void setup()
{
    Serial.begin(9600);
    EEPROM.begin(600);

    connectWiFi();
    
    delay(500);
    // LCD lcdClass(lcd);
    pinMode(output_pin, OUTPUT);
    digitalWrite(output_pin, LOW);

    TRM trm{downButton, upButton, settingsButtn, startButton, numberButton, mtrButton, mtrRelay, sck, cs, so};

    thing["temperature"] >> [](pson &out)
    {
        out = temp;
    };
    thing["start programm"] << [](pson &in)
    {
        start_programm = in ? true : false;
    };

    while (1)
    {
        trm.main_programm();
        temp = trm.getTemperature();
        a = trm.getPIDvalue();
        b = map(a, 0, 255, 255, 0);

        if (!high_low && (millis() - timer >= b))
        {
            digitalWrite(output_pin, HIGH);
            timer = millis();
            high_low = true;
        }
        if (high_low && (millis() - timer >= a))
        {
            digitalWrite(output_pin, LOW);
            timer = millis();
            high_low = false;
        }
        if (trm.check_wifi())
        {
            thing.handle();
            parametrs = trm.getParametrs();
        }
    }
}

void loop()
{
}

void connectWiFi()
{
    LCD *lcd = new LCD();
    MyButton *upBtn = new MyButton(upButton);
    MyButton *downBtn = new MyButton(downButton);
    MyButton *startBtn = new MyButton(startButton);
    bool connect = false;
    lcd->ClearAll();
    lcd->ConnectToWifi(connect);

    while (1)
    {
        if (upBtn->Clicked() || upBtn->Clicked())
        {
            if (connect)
                connect = false;
            else
                connect = true;
            lcd->ClearAll();
            lcd->ConnectToWifi(connect);
        }
        if (startBtn->Clicked())
        {
            if (connect)
            {
                lcd->ClearAll();
                lcd->Connecting();
                portalRun();
            }
            delete lcd, upBtn, downBtn, startBtn;
            return;
        }
    }
}