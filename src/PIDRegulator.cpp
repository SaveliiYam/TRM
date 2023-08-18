#include "PIDRegulator.h"

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

void PIDRegulator::putTemperature(const int& setTemperature){
    regulator.setpoint = setTemperature;
}

void PIDRegulator::tuneInitialization(const float &temperatureNeed)
{
    tuner.setParameters(NORMAL, temperatureNeed, 5, 5000, 2, 15000, 400);
}

void PIDRegulator::enterPIDKoefficients(const Koefficients &koefficients)
{
    regulator.Kp = koefficients.P;
    regulator.Ki = koefficients.I;
    regulator.Kd = koefficients.D;
}

bool PIDRegulator::tunePID(const float &temperatureNow)
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

        enterPIDKoefficients(koefficients);
        saveKoefficients();
        return true;
    }
    return false;
}

void PIDRegulator::loadKoefficients(){EEPROM.get(200, koefficients);}
void PIDRegulator::saveKoefficients(){EEPROM.put(200, koefficients);}
void PIDRegulator::baseKoefficients(){
        Koefficients base;
        koefficients = base;
        saveKoefficients();
    }