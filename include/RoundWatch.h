#ifndef ROUNWATCH_H
#define  ROUNWATCH_H

#include "TimeLed.h"
#include "RoundDial.h"

#define INT(name) static_cast<int>(name)

class RoundWatch
{
public:
    RoundWatch();
    ~RoundWatch();

    // Dial`s interface
    void SetMottorPins(const DIAL dial, const uint8_t in1, const uint8_t in2, const uint8_t in3, const uint8_t in4);

    // LED`s interface
    void SetLedPin(const DIAL dial, const uint8_t pin);
    void SetTopBrightness(const DIAL dial, const double brightness); // max 1.0
    void SetBottomBrightness(const DIAL dial, const double brightness); // max 1.0
    void SetTopColor(const DIAL dial, const uint8_t r, const uint8_t g, const uint8_t b);
    void SetBottomColor(const DIAL dial, const uint8_t r, const uint8_t g, const uint8_t b); 

    // RoundWatch interface
    void Setup();
    void SetTime(const int16_t hour, const int16_t min = -1, const int16_t sec = -1);

private:
    static const int num = INT(DIAL::NUM_DIALS);

private:
    RoundDial* m_dials[num];
    TimeLed* m_leds[num];

private:
    uint16_t m_timeValue[num];
};

#endif  
