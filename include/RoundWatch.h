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
        Driver::Settings driver;
        //Led
        uint8_t ledPin = 0;
        LED topLED;
        LED bottomLED;
    };

    struct WatchBlinker
    {
        WatchBlinker(RoundWatch& rw, const uint8_t r, const uint8_t g, const uint8_t b);
        ~WatchBlinker();
    private:
        RoundWatch& watchRef;
    };
    
public:
    RoundWatch();
    ~RoundWatch();

    void AdjustDialSettings(const DIAL dial, const Settings& settings);

    // RoundWatch interface
    void Setup();
    void SetRealTime(const uint8_t hour, const uint8_t min = 0, const uint8_t sec = 0);
    void AdjustBrightness(const int analogValue);

    // for calibration
    void MoveForward(const DIAL dial);
    void MoveBackward(const DIAL dial);
    void MoveOneDivForward(const DIAL dial);
    void MoveOneDivBackward(const DIAL dial);
    void CalibrateByIncorrectTime(const uint8_t hours, const uint8_t min);

private:
    // Dial`s interface
    void SetupDial(const DIAL dial, const Driver::Settings& driverSettings);

    // LED`s interface
    void SetLedPin(const DIAL dial, const uint8_t pin);
    void SetTopBrightness(const DIAL dial, const double brightness); // max 1.0
    void SetBottomBrightness(const DIAL dial, const double brightness); // max 1.0
    void SetTopColor(const DIAL dial, const uint8_t r, const uint8_t g, const uint8_t b);
    void SetBottomColor(const DIAL dial, const uint8_t r, const uint8_t g, const uint8_t b);
    void OffLed(const DIAL dial);

private:
    static const int num = INT(DIAL::NUM_DIALS);

private:
    RoundDial* m_dials[num];
    TimeLed* m_leds[num];

private:
    int16_t m_timeValue[num];
    int m_prevAnalogValue;
};

#endif  
