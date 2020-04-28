#ifndef TIMELED_H
#define  TIMELED_H

#include <Adafruit_NeoPixel.h>

class TimeLed
{
public:
    TimeLed(const uint8_t pin);

    //void SetBrightness(const uint8_t brightness);
    void SetTopColor(const uint8_t r, const uint8_t g, const uint8_t b);
    void SetBottomColor(const uint8_t r, const uint8_t g, const uint8_t b);
    void Show();
private:
    Adafruit_NeoPixel strip;
};

#endif  
