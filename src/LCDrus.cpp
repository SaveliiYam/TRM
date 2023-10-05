#include "LCDrus.h"

LCD::LCD()
{
}

void LCD::ini()
{
    // lcd.command(0b101010);
    lcd.init();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("\xA8"
              "p"
              "\xB8\xB3"
              "e"
              "\xBF");
    lcd.createChar(0, RightArrow);
}

void LCD::ClearAll() { lcd.clear(); }

void LCD::Point(const byte &parametr)
{
    lcd.setCursor(0, parametr);
    lcd.print(char(0));
}

void LCD::ConnectToWifi(const bool &connect)
{
    lcd.setCursor(0, 0);
    lcd.print("Xo"
              "\xbf\xB8\xBF"
              "e "
              "\xBB\xB8"
              " "
              "\xB3\xC3");
    lcd.setCursor(0, 1);
    lcd.print("\xBE"
              "o"
              "\xE3\xBA\xBB\xC6\xC0\xB8\xBF\xC4"
              "c"
              "\xC7"
              " \xBA"
              " wifi?");
    if (connect)
    {
        Point(2);
    }
    else
    {
        Point(3);
    }
    lcd.setCursor(1, 2);
    lcd.print(yes); // yes
    lcd.setCursor(1, 3);
    lcd.print(no); // no
}

void LCD::workProgramm(const int &setpointTemperature, const float &temperature, const byte &numberPause, const uint32_t &time, const char *timeParametr)
{
    lcd.setCursor(0, 0);
    lcd.print("\xA9\x63\xBF\x61\xB3\xBA\x61"
              ": "); // setpoint
    lcd.print(setpointTemperature);
    lcd.print(probel);
    lcd.setCursor(0, 1);
    lcd.print("\x54\x65\xBC\xBE\x65\x70\x61\xBF\x79\x70\x61"
              ": "); // Temperature
    lcd.print(temperature);
    lcd.print(probel);
    lcd.setCursor(0, 2);
    lcd.print("Bpe\xBC\xC7"
              ": "); // time
    lcd.print(time);
    lcd.print(timeParametr);
    lcd.setCursor(0, 3);
    lcd.print("\xA8\x61\x79\xB7\x61"
              ": "); // pause
    lcd.print(numberPause);
    lcd.print(probel);
}

void LCD::mainMenu(const int &temperature, const byte &number)
{
    lcd.setCursor(0, 0);
    lcd.print("Ho\xBC\x65\x70 \xBE\x70\x6F\xB4"
              ": "); // number prog
    lcd.print(number);
    lcd.print(probel);
    lcd.setCursor(0, 1);
    lcd.print("Te\xBC\xBE\x65\x70\x61\xBF\x79\x70\x61"
              ": "); // temperature
    lcd.print(temperature);
    lcd.print(probel);
}

void LCD::mainMenu(const int &temperature, const byte &number, const byte max, const byte &min, const bool &delay, const bool &set)
{
    lcd.setCursor(0, 0);
    lcd.print("Ho\xBC\x65\x70 \xBE\x70\x6F\xB4: "); // number prog
    lcd.print(number);
    lcd.print(probel);
    lcd.setCursor(0, 1);
    lcd.print("Te\xBC\xBE\x65\x70\x61\xBF\x79\x70\x61"
              ": "); // temperature
    lcd.print(temperature);
    lcd.print(probel);
    Clear(2);
    Clear(3);

    // lcd.setCursor(0, 2);
    // lcd.print("Max: "); lcd.print(max);
    // lcd.print(" Min: "); lcd.print(min);
    // lcd.setCursor(0, 3);
    // lcd.print("Set: "); lcd.print(set);
    // lcd.print(" Del: "); lcd.print(delay);
}

void LCD::stopProgramm()
{
    lcd.setCursor(0, 0);
    lcd.print("     \xA8\x70\x6F\xB4\x70\x61\xBC\xBC\x61");
    lcd.setCursor(0, 1);
    lcd.print("    O\x63\xBf\x61\xbd\x6f\xb3\xbb\x65\xBD\x61");
}

void LCD::startProgramm()
{
    lcd.setCursor(0, 0);
    lcd.print("     \xA8\x70\x6F\xB4\x70\x61\xBC\xBC\x61");
    lcd.setCursor(0, 1);
    lcd.print("      Ha\xC0\x61\xBB\x61\x63\xC4");
}

void LCD::BaseSettings(const bool &parametr)
{
    if (parametr)
        Point(3);
    else
        Point(2);
    lcd.setCursor(0, 0);
    // lcd.print("Return base setting");//Return base setting
    lcd.print("Bep\xBD\x79\xBF\xC4\x20\xB2\x61\xB7\x6F\xB3\xC3\x65");
    lcd.setCursor(0, 1);
    lcd.print("\xBD\x61\x63\xBF\x70\x6F\xB9\xBA\xB8?");
    lcd.setCursor(1, 2);
    lcd.print(no);
    lcd.setCursor(1, 3);
    lcd.print(yes);
}

void LCD::EnterTemperaturePause(byte numberProgramm, byte step, const bool &parametr, const byte &temperature, const uint16_t &time)
{
    NumberProg(numberProgramm);
    lcd.setCursor(0, 1);
    // lcd.print("Step: ");//step
    lcd.print("\xA8\x61\x79\xB7\x61: ");
    byte step_ = step;
    step_++;
    lcd.print(step_);
    if (!parametr)
        Point(2);
    else
        Point(3);
    lcd.setCursor(1, 2);
    // lcd.print("Set temp: ");
    lcd.print("\xA9\x63\xBF\x61\xB3\xBA\x61: ");
    lcd.print(temperature);
    lcd.setCursor(1, 3);
    // lcd.print("Set time: ");
    lcd.print("Bpe\xBC\xC7: ");
    lcd.print(time);
}
void LCD::Connecting()
{
    lcd.setCursor(0, 0);
    // lcd.print("Connect...");
    lcd.print("\xA8\x6F\xE3\xBA\xBB\xC6\xC0\x65\xBD\xB8\x65...");
}

void LCD::NumberProg(const byte &numberProgramm)
{
    lcd.setCursor(0, 0);
    lcd.print("Ho\xBC\x65\x70 \xBE\x70\x6F\xB4: ");
    lcd.print(numberProgramm);
    lcd.print(probel);
}

void LCD::TimeConcrete(const bool &parametr, const bool &value)
{
    if (parametr)
    {
        if (value)
        {
            Point(2);
            Serial.println("Point 1");
        }
        else if (!value)
        {
            Point(3);
            Serial.println("Point 2");
        }
        lcd.setCursor(0, 0);
        // lcd.print("min/sec");
        lcd.print("Hac\xBF\x70\x6F\xB9\xBA\xB8 \xB3\x70\x65\xBC\x65\xBD\xB8");
        lcd.setCursor(0, 1);
        lcd.print("O\xBF\x6F\xB2\x70\x61\xB6\x65\xBD\xB8\x65\x20\xB3\x70\x65\xBC\x65\xBD\xB8");
        lcd.setCursor(1, 2);
        lcd.print("C\x65\xBA\x79\xBD\xE3\xC3");
        // lcd.print("sec");
        lcd.setCursor(1, 3);
        // lcd.print("min");
        lcd.print("M\xB8\xBD\x79\xBF\xC3");
    }
    else
    {
        if (value)
        {
            Point(2);
        }
        else if (!value)
        {
            Point(3);
        }
        lcd.setCursor(0, 0);
        // lcd.print("time delay");
        lcd.print("Hac\xBF\x70\x6F\xB9\xBA\xB8 \xB3\x70\x65\xBC\x65\xBD\xB8");
        lcd.setCursor(0, 1);
        lcd.print("\xA4\x61\xE3\x65\x70\xB6\xBA\x61\x20\xB3\x70\x65\xBC\x65\xBD\xB8");
        lcd.setCursor(1, 2);
        // lcd.print("no delay");
        lcd.print("\xA0\x65\xB7\x20\xB7\x61\xE3\x65\x70\xB6\xBA\xB8");
        lcd.setCursor(1, 3);
        // lcd.print("delay");
        lcd.print("C \xB7\x61\xE3\x65\x70\xB6\xBA\x6F\xB9");
    }
}

void LCD::TimeSettings(const bool &parametr)
{
    if (parametr)
    {
        Point(1);
    }
    else
    {
        Point(2);
    }
    lcd.setCursor(0, 0);
    // lcd.print("Time Settings");
    lcd.print("Hac\xBF\x70\x6F\xB9\xBA\xB8 \xB3\x70\x65\xBC\x65\xBD\xB8");
    lcd.setCursor(1, 1);
    // lcd.print("min/sec");
    lcd.print("O\xBF\x6F\xB2\x70\x61\xB6\x65\xBD\xB8\x65\x20\xB3\x70\x65\xBC\x65\xBD\xB8");
    lcd.setCursor(1, 2);
    // lcd.print("delay time");
    lcd.print("\xA4\x61\xE3\x65\x70\xB6\xBA\x61\x20\xB3\x70\x65\xBC\x65\xBD\xB8");
}

void LCD::PowerLimits(const bool &parametr, const byte &minLimit, const byte &maxLimit)
{
    if (parametr)
    {
        Point(1);
    }
    else
    {
        Point(2);
    }
    lcd.setCursor(0, 0);
    // lcd.print("Power limits:");
    lcd.print("\xA8\x61\x70\x61\xBC\x65\xBF\x70\xC3\x20\xBC\x6F\xE6\xBD\x6F\x63\xBF\xB8");
    lcd.setCursor(1, 1);
    // lcd.print("minLimit: ");
    lcd.print("M\xB8\xBD\xB8\xBC\x79\xBC:  ");
    lcd.print(minLimit);
    lcd.print('%');
    lcd.setCursor(1, 2);
    // lcd.print("maxLimit: ");
    lcd.print("Ma\xBA\x63\xB8\xBC\x79\xBC: ");
    lcd.print(maxLimit);
    lcd.print('%');
}

void LCD::PrintLimitMenu(const bool &parametr, const byte &limit)
{
    lcd.setCursor(1, 0);
    if (parametr)
    {
        // lcd.print("MinLimit: ");
        lcd.print("M\xB8\xBD\xB8\xBC\x79\xBC:  ");
        lcd.print(limit);
    }
    else
    {
        // lcd.print("maxLimit: ");
        lcd.print("Ma\xBA\x63\xB8\xBC\x79\xBC: ");
        lcd.print(limit);
    }
    lcd.print('%');
}

void LCD::WiFi(const bool &parametr)
{
    if (parametr)
    {
        Point(1);
    }
    else
    {
        Point(2);
    }
    lcd.setCursor(1, 0);
    // lcd.print("WiFi on/off");
    lcd.print("WIFI \xB3\xBA\xBB/\xB3\xC3\xBA\xBB");
    lcd.setCursor(1, 1);
    lcd.print("\xB3\xBA\xBB");
    lcd.setCursor(1, 2);
    lcd.print("\xB3\xC3\xBA\xBB");
}

void LCD::settingsMainMenu(const byte &parametr)
{
    lcd.clear();
    Point(parametr);
    lcd.setCursor(1, 0);
    // lcd.print("Pause settings");
    lcd.print("Hac\xBF\x70\x6F\xB9\xBA\xB8\x20\xBE\x61\x79\xB7\xC3");
    lcd.setCursor(1, 1);
    // lcd.print("Other settings");
    lcd.print("\xE0\x6F\xBE. \xBD\x61\x63\xBF\x70\x6F\xB9\xBA\xB8");
    lcd.setCursor(1, 2);
    // lcd.print("Power limits");
    lcd.print("\xA8\x61\x70\x61\xBC\x65\xBF\x70\xC3\x20\xBC\x6F\xE6\xBD\x6F\x63\xBF\xB8");
    lcd.setCursor(1, 3);
    // lcd.print("sec/min settings");
    lcd.print("Hac\xBF\x70\x6F\xB9\xBA\xB8 \xB3\x70\x65\xBC\x65\xBD\xB8");
}

void LCD::WriteOnLcdPause(const Pair<byte, uint16_t> &pause, const byte &numberPause)
{
    static byte temperatureOld;
    static uint16_t timeOld;
    static byte numberPauseOld;
    if (numberPauseOld != numberPause)
    {
        numberPauseOld = numberPause;
        Clear(1);
    }
    if (temperatureOld != pause.first() || timeOld != pause.second())
    {
        temperatureOld = pause.first();
        timeOld = pause.second();
        Clear(2);
    }
    lcd.setCursor(0, 1);
    // lcd.print("Number pause: ");
    lcd.print("\xA8\x61\x79\xB7\x61: ");
    lcd.print(numberPauseOld);
    lcd.setCursor(0, 2);
    // lcd.print("NeedTemp");
    lcd.print("\xA9\x63\xBF\x61\xB3\xBA\x61");
    lcd.print(temperatureOld);
    // lcd.print(" Time: ");
    lcd.print(" Bpe\xBC\xC7: ");
    lcd.print(timeOld);
}

void LCD::WriteTemperature(const float &temperature)
{
    static float oldTemp;
    if (oldTemp != temperature)
    {
        oldTemp = temperature;
        Clear(0);
    }
    lcd.setCursor(0, 0);
    // lcd.print("Temperature: ");
    lcd.print("Te\xBC\xBE\x65\x70\x61\xBF\x79\x70\x61: ");
    lcd.print(temperature);
}

void LCD::Clear(const byte &curs)
{
    lcd.setCursor(0, curs);
    lcd.print("                    ");
}

void LCD::TuneBaseSettings(const byte &parametr)
{
    if (parametr == 1)
    {
        Point(1);
    }
    else if (parametr == 2)
    {
        Point(2);
    }
    else
    {
        Point(3);
    }
    lcd.setCursor(0, 0);
    // lcd.print("Other settings");
    lcd.print("\xE0\x6F\xBE. \xBD\x61\x63\xBF\x70\x6F\xB9\xBA\xB8");
    lcd.setCursor(1, 1);
    // lcd.print("Base koefficients");
    lcd.print("\xA0\x61\xB7\x6F\xB3\xC3\x65\x20\xBD\x61\x63\xBF\x70\x6F\xB9\xBA\xB8");
    lcd.setCursor(1, 2);
    // lcd.print("Tune Settings");
    lcd.print("H\x61\x63\xBF\x70\x6F\xB9\xBA\xB8\x20\xA8\xA5\xE0");
    lcd.setCursor(1, 3);
    lcd.print("Ka\xBB\xB8\xB2\x70\x6F\xB3\xBA\x61");
}
void LCD::TuneBaseSettings(const byte &parametr, const bool &choise)
{
    lcd.setCursor(0, 0);
    if (parametr)
    {
        // lcd.print("Base koefficients");
        lcd.print("Bep\xBD\x79\xBF\xC4");
        lcd.setCursor(0, 1);
        lcd.print("\xA0\x61\xB7\x6F\xB3\xC3\x65\x20\xBD\x61\x63\xBF\x70\x6F\xB9\xBA\xB8?"); // Базовые настройки
    }
    else
    {
        // lcd.print("Tune Settings");
        lcd.print("\xA8\x70\x6F\xB8\xB7\xB3\x65\x63\xBF\xB8");
        lcd.setCursor(0, 1);
        lcd.print("H\x61\x63\xBF\x70\x6F\xB9\xBA\x79\x20\xA8\xA5\xE0?");
    }
    if (choise)
    {
        Point(2);
    }
    else
    {
        Point(3);
    }
    lcd.setCursor(1, 2);
    lcd.print(yes);
    lcd.setCursor(1, 3);
    lcd.print(no);
}

void LCD::TuneBaseSettings(const byte &parametr, const float &calib){
    lcd.setCursor(0,0);
    lcd.print("Ka\xBB\xB8\xB2\x70\x6F\xB3\xBA\x61");
    lcd.setCursor(1,1);
    lcd.print("\xA4\xBD\x61\xC0\x65\xBD\xB8\x65: ");
    lcd.print(calib);
}

void LCD::Tuning(const byte &temperatureMax)
{
    lcd.setCursor(0, 0);
    // lcd.print("Enter max temp: ");
    lcd.print("B\xB3\x6F\xE3\x20\xBC\x61\xBA\x63\xB8\xBC\x61\xBB\xC4\xBD\x6F\xB9");
    lcd.setCursor(0, 1);
    lcd.print("Te\xBC\xBE\x65\x70\x61\xBF\x79\x70\xC3: ");
    lcd.print(temperatureMax);
    lcd.print(probel);
}
void LCD::TuningProcces(const byte &percent, const byte &temperatureNow)
{
    lcd.setCursor(0, 0);
    // lcd.print("Percent: ");
    lcd.print("\xA8\x70\x6F\xE5\x65\xBD\xBF: ");
    lcd.print(percent);
    lcd.print("%");
    lcd.print(probel);
    lcd.setCursor(0, 1);
    // lcd.print("Temperature: ");
    lcd.print("\x54\x65\xBC\xBE\x65\x70\x61\xBF\x79\x70\x61\x3A\x20");
    lcd.print(temperatureNow);
    lcd.print(probel);
}

void LCD::SuccessConection(const bool &status)
{
    lcd.setCursor(0, 1);
    if (status)
    {
        lcd.print("\xA9\x63\xBE\x65\x78");
    }
    else
    {
        lcd.print("He \xBe\x6F\xBB\x79\xC0\xB8\xBB\x6F\x63\xC4");
        lcd.setCursor(0, 2);
        lcd.print("\xA8\x65\x70\x65\xB7\x61\xB4\x70\x79\xB7\xBA\x61");
    }
}