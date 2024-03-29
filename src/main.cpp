#include <WiFiManager.h>
#include "TRM.h"
#include "LCDrus.h"
#include <LiquidCrystal_I2C.h>
#include <ThingerESP32.h>
#include <EEPROM.h>
#include "arduino_secrets/arduino_secrets.h"
#include "buttons/MyButton.h"
#include "couples/Fouth.h"
#include "couples/Pair.h"
#include "couples/Third.h"
#define TINGER_SERIAL_DEBUG

ThingerESP32 thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);

byte number_prog = 1, number_prog_old = 0;
int a = 0, b;
bool high_low = false;
bool start_programm = false, stop_programm = false;
bool wifiOn = false;

uint32_t timer = millis();

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

TRM trm{downButton, upButton, settingsButtn, startButton, numberButton, mtrButton, mtrRelay, sck, cs, so};

void connectWiFi();
void dimmer(const byte &pidValue);
bool wifiManager();

void setup()
{
    Serial.begin(9600);
    EEPROM.begin(600);

    connectWiFi();

    pinMode(output_pin, OUTPUT);
    digitalWrite(output_pin, LOW);

    trm.ini();
    
    thing["temperature"] >> [](pson &out)
    {
        out = trm.getTemperature();
    };
    thing["start_programm"] << [](pson &in)
    {
        if (in.is_empty())
        {
            in = start_programm;
        }
        start_programm = in ? true : false;
    };
    thing["stop_button"] << [](pson &in)
    {
        if (in.is_empty())
        {
            in = stop_programm;
        }
        stop_programm = in ? true : false;
    };
    thing["Number_programm"] << [](pson &in)
    {
        if (in.is_empty())
        {
            in = number_prog;
        }
        number_prog = in;
    };
    thing["Time_set"] << [](pson &in)
    {
        if (in.is_empty())
        {
            in = trm.getTimeSet();
        }
        trm.save_parametrs_time(1, in);
    };
    thing["Time_delay"] << [](pson &in)
    {
        if (in.is_empty())
        {
            in = !trm.getTimeDelay();
        }
        trm.save_parametrs_time(2, !in);
    };
    thing["Power_max"] << [](pson &in)
    {
        if (in.is_empty())
        {
            in = trm.getPowerMax();
        }
        trm.save_parametrs_power(3, in);
    };
    thing["Power_min"] << [](pson &in)
    {
        if (in.is_empty())
        {
            in = trm.getPowerMin();
        }
        trm.save_parametrs_power(4, in);
    };
    thing["Calibration_temperature"] << [](pson &in)
    {
        if (in.is_empty())
        {
            in = trm.getCalibrationValue();
        }
        trm.enterCalibrationValue(in);
    };
    thing["Predel_temperature"] << [](pson &in)
    {
        if (in.is_empty())
        {
            in = trm.getPredelTemperature();
        }
        trm.enterPredelTemperature(in);
    };
    thing["mode_work"] << [](pson &in)
    {
        if (in.is_empty())
        {
            in = trm.getMode();
        }
        trm.enterMode(in);
    };
    thing["Power_value"] << [](pson &in)
    {
        if (in.is_empty())
        {
            in = trm.getPowerValueNow();
        }
        trm.enterPowerValueNow(in);
    };
    thing["OutputTimeTemperature"] >> [](pson & out){
        Third<byte, uint32_t, byte> out_val = trm.getOutputTimeTem();
        out["timeOutput"] = out_val.second();
        out["tempOutput"] = out_val.first();
        out["numberPauseOutput"] = out_val.third();
    };
    thing["ParametrsShow"] >> [](pson & out){
        out["CalibrShow"] = trm.getCalibrationValue();
        out["PowerMaxShow"] = trm.getPowerMax();
        out["PowerMinShow"] = trm.getPowerMin();
        out["PredelTempShow"] = trm.getPredelTemperature();
    };
}

void loop()
{
    trm.main_programm();
    dimmer(trm.getPIDvalue());

    if (wifiOn)
    {
        if (number_prog_old != number_prog)
        {
            trm.put_number_prog(number_prog);
            number_prog_old = number_prog;
        }
        trm.start_program_from_server(start_programm);
        trm.stop_program_from_server(stop_programm);
        thing.handle();
    }
}

void dimmer(const byte &pidValue)
{
    a = pidValue;
    b = map(a, 0, 255, 255, 0);

    if (!high_low && (millis() - timer >= b))
    {
        digitalWrite(output_pin, HIGH);
        timer = millis();
        high_low = true;
    }
    if (high_low && (millis() - timer >= a))
    {
        if(b != 0){
            digitalWrite(output_pin, LOW);
        }
        timer = millis();
        high_low = false;
    }
}

void connectWiFi()
{
    LCD *lcd = new LCD();
    lcd->ini();
    MyButton *upBtn = new MyButton(upButton);
    MyButton *downBtn = new MyButton(downButton);
    MyButton *startBtn = new MyButton(startButton);
    bool connect = false;
    lcd->ClearAll();
    lcd->ConnectToWifi(connect);

    while (1)
    {
        if (upBtn->Clicked() || downBtn->Clicked())
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
                if (wifiManager())
                {
                    lcd->SuccessConection(true);
                    wifiOn = true;
                    delay(200);
                }
                else
                {
                    lcd->SuccessConection(false);
                    delay(500);
                    ESP.restart();
                }
            }
            delete lcd, upBtn, downBtn, startBtn;
            return;
        }
    }
}

bool wifiManager()
{
    WiFi.mode(WIFI_STA);
    WiFiManager wifiManager;
    //wifiManager.resetSettings();          // перезапись имени wifi каждый запуск
    bool res;                             // храним переменную для подключения
    res = wifiManager.autoConnect("PVK"); // подключение телефона к точке
    return res;
}