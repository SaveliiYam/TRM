#pragma once
#include <Arduino.h>

class TimerUs {
  public:
    bool ready() {
      if (state && micros() - tmr >= prd) {
        tmr = micros();
        return true;
      }
      return false;
    }
    void stop() {
      state = false;
    }
    void restart() {
      tmr = micros();
      state = true;
    }
    void setPeriod(uint32_t period) {
      restart();
      prd = period;
      if (prd == 0) prd++;
    }

  private:
    bool state = 0;
    uint32_t tmr = 0, prd = 0;
};