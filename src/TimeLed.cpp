#include "TimeLed.h"

#define LED_COUNT 9

TimeLed::TimeLed(const uint8_t pin)
    : strip(LED_COUNT, pin, NEO_GRB + NEO_KHZ400)
    , m_topBrightness(0)
    , m_bottomBrightness(0)
    , m_topColor()
    , m_bottomColor()
{
  strip.begin();           
  strip.show();      
  strip.setBrightness(255);
}

void TimeLed::SetTopBrightness(const double brightness)
{
  m_topBrightness = brightness;
}

void TimeLed::SetBottomBrightness(const double brightness)
{
  m_bottomBrightness = brightness;
}

void TimeLed::SetTopColor(const uint8_t r, const uint8_t g, const uint8_t b)
{
  m_topColor.red = r;
  m_topColor.green = g;
  m_topColor.blue = b;
}

void TimeLed::SetBottomColor(const uint8_t r, const uint8_t g, const uint8_t b)
{
  m_bottomColor.red = r;
  m_bottomColor.green = g;
  m_bottomColor.blue = b;
}

void TimeLed::Show()
{

  const auto topColor = strip.Color(m_topColor.red * m_topBrightness, 
                                    m_topColor.green * m_topBrightness, 
                                    m_topColor.blue * m_topBrightness);
  for (uint16_t i = 0; i < 6; i++)
    strip.setPixelColor(i, topColor);


  const auto bottomColor = strip.Color(m_bottomColor.red * m_bottomBrightness,
                                      m_bottomColor.green * m_bottomBrightness,
                                      m_bottomColor.blue * m_bottomBrightness);
  for (uint16_t i = 6; i < LED_COUNT; i++)
    strip.setPixelColor(i, bottomColor);

  strip.show();
}