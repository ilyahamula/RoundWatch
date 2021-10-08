#ifndef ROUNWATCH_H
#define ROUNWATCH_H

#include "TimeLed.h"
#include "RoundDial.h"

#define INT(name) static_cast<int>(name)

class RoundWatch
{
public:
    struct LED
    {
        uint8_t red = 0;
        uint8_t green = 0;
        uint8_t blue = 0;
        double brightness = 0.0;
    };

    struct Settings
    {
        // Mottor's pins
        uint8_t in1 = 0;
        uint8_t in2 = 0;
        uint8_t in3 = 0;
        uint8_t in4 = 0;

        uint8_t divisionPin = 0;

        //Led
        uint8_t ledPin = 0;
        LED topLED;
        LED bottomLED;
    };
public:
    RoundWatch();
    ~RoundWatch();

    void AdjustDialSettings(const DIAL dial, const Settings& settings);

    // RoundWatch interface
    void Setup();
    void SetRealTime(const uint8_t hour, const uint8_t min = 0, const uint8_t sec = 0);
    void SetDisplayedTime(const uint8_t hour, const uint8_t min = 0, const uint8_t sec = 0);

    // for calibration
    void MoveForward(const DIAL dial);
    void MoveBackward(const DIAL dial);
    void MoveOneDivForward(const DIAL dial);
    void CalibrateByIncorrectTime(const uint8_t hours, const uint8_t min);

private:
    // Dial`s interface
    void SetMottorPins(const DIAL dial, const uint8_t in1, const uint8_t in2, const uint8_t in3, const uint8_t in4);
    void SetDivisionsPin(const DIAL dial, const uint8_t pin);

    // LED`s interface
    void SetLedPin(const DIAL dial, const uint8_t pin);
    void SetTopBrightness(const DIAL dial, const double brightness); // max 1.0
    void SetBottomBrightness(const DIAL dial, const double brightness); // max 1.0
    void SetTopColor(const DIAL dial, const uint8_t r, const uint8_t g, const uint8_t b);
    void SetBottomColor(const DIAL dial, const uint8_t r, const uint8_t g, const uint8_t b); 

private:
    static const int num = INT(DIAL::NUM_DIALS);

private:
    RoundDial* m_dials[num];
    TimeLed* m_leds[num];

private:
    int16_t m_timeValue[num];
};

#endif  
