#include "PIDRegulator.h"

int PIDRegulator::getValuePID(const float &temperatureNow)
{
    regulator.input = temperatureNow;
    return regulator.getResult();
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

        return true;
    }
    return false;
}