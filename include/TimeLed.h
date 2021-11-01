#ifndef TIMELED_H
#define  TIMELED_H

#include <Adafruit_NeoPixel.h>

class TimeLed
{
    friend void RunBlinking(void* params);

public:
    TimeLed(const uint8_t pin);

    void SetTopBrightness(const double brightness); // max 1.0
    void SetBottomBrightness(const double brightness); // max 1.0
    void SetTopColor(const uint8_t r, const uint8_t g, const uint8_t b);
    void SetBottomColor(const uint8_t r, const uint8_t g, const uint8_t b);
    void Show();
    void Off();
    void OffTop();
    void OnTop();

    void RunSetupBlinking(const uint8_t r, const uint8_t g, const uint8_t b);
    void StopSetupBlinking();
    void GetBlinkingColor(uint8_t& r, uint8_t& g, uint8_t& b);

private:

private:
    struct sRGB
    {
        uint8_t red = 0;
        uint8_t green = 0;
        uint8_t blue = 0;
    };

private:
    Adafruit_NeoPixel strip;

private:
    double m_topBrightness;
    double m_bottomBrightness;

    sRGB m_topColor;
    sRGB m_bottomColor;
    sRGB m_blinkingColor;

    TaskHandle_t m_setupBlinkTask;
};

#endif  
