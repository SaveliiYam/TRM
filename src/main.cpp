#include "TRM.h"
#include "LCD.h"
#include <LiquidCrystal_I2C.h>
// #include <ThingerESP32.h>
// #include "arduino_secrets.h"
// #define TINGER_SERIAL_DEBUG
// ThingerESP32 thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);
int a = 0, b;
bool high_low = false;
uint32_t timer = millis();

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
    // LCD lcdClass(lcd);
    pinMode(output_pin, OUTPUT);
    digitalWrite(output_pin, LOW);

    TRM trm{downButton, upButton, settingsButtn, startButton, numberButton, mtrButton, mtrRelay, sck, cs, so};
    while (1)
    {
        trm.main_programm();
        
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
        //Serial.print("PID value: ");
        //Serial.println(a);
    }
}

void loop()
{
}
