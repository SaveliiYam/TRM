#include "PIDRegulator.h"
#include "Pair.h"

PIDRegulator::PIDRegulator()
{ // Нужно из EEprom грузить коэффициенты и писать их в регулятор
    enterPIDKoefficients(koefficients);
    regulator.setDt(400);
    regulator.setDirection(NORMAL);
    regulator.setLimits(0, 255);
}

int PIDRegulator::getValuePID(const int &temperatureNow)
{
    // Serial.print("Temperature: ");
    // Serial.println(temperatureNow);
    // Serial.print("regulator.Kp");
    // Serial.println(regulator.Kp);

    regulator.input = temperatureNow;
    return regulator.getResultTimer();
}

void PIDRegulator::putTemperature(const int &setTemperature)
{
    regulator.setpoint = setTemperature;
}

void PIDRegulator::tuneInitialization(const float &temperatureNeed)
{
    tune = true;
    Serial.print("Ini");
    Serial.println(temperatureNeed);
    tuner.setParameters(NORMAL, temperatureNeed, 10, 100, 5, 100, 400);
}

Third<bool, byte, byte> PIDRegulator::tunePID(const int &temperatureNow)
{
    tuner.setInput(temperatureNow);
    tuner.compute();
    // Если точность больше 95, то стоп
    if (tuner.getAccuracy() > 95)
    {
        koefficients.P = tuner.getPID_p();
        koefficients.I = tuner.getPID_i();
        koefficients.D = tuner.getPID_d();
        // надо записать в EEPROM

        Serial.print("Accuracy: ");
        Serial.println(tuner.getAccuracy());
        Serial.println(tuner.getPID_p());
        Serial.println(tuner.getPID_i());
        Serial.println(tuner.getPID_d());

        enterPIDKoefficients(koefficients);
        saveKoefficients();
        return Third<bool, byte, byte>{true, 0, 100};
    }
    tuner.debugText();
    return Third<bool, byte, byte>{false, tuner.getOutput(), tuner.getAccuracy()};
}
Pair<byte, byte> PIDRegulator::GetPIDValueTune(const int &temperatureNow)
{
    Third<bool, byte, byte> result = tunePID(temperatureNow);
    if (result.first())
    {
        tune = false;
        Pair<byte, byte> res{0, 100};
        parametrsForLCD = res;
        return parametrsForLCD;
    }
    else
    {
        Pair<byte, byte> res{result.second(), result.third()};
        parametrsForLCD = res;
        return parametrsForLCD;
    }
}
Pair<byte, byte> PIDRegulator::GetParametrsLCD() const
{
    return parametrsForLCD;
}

bool PIDRegulator::getTuneInfo() const { return tune; }
void PIDRegulator::enterPIDKoefficients(const Koefficients &koefficients)
{
    regulator.Kp = koefficients.P;
    regulator.Ki = koefficients.I;
    regulator.Kd = koefficients.D;
}
void PIDRegulator::loadKoefficients()
{
    EEPROM.get(500, koefficients);
    Serial.print("P: ");
    Serial.println(koefficients.P);
    Serial.print("I: ");
    Serial.println(koefficients.I);
    Serial.print("D: ");
    Serial.println(koefficients.D);
    enterPIDKoefficients(koefficients);
}
void PIDRegulator::saveKoefficients()
{
    EEPROM.put(500, koefficients);
    EEPROM.commit();
}
void PIDRegulator::baseKoefficients()
{
    Koefficients base;
    base.P = 1.0;
    base.I = 1.0;
    base.D = 1.0;
    koefficients = base;
    EEPROM.put(500, base);
    EEPROM.commit();
}