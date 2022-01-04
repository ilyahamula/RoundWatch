#include "TimeLed.h"
#include "Debug.h"

#define LED_COUNT 9

namespace
{
  void SetSpecificPixels(Adafruit_NeoPixel& strip, uint8_t r, uint8_t g, uint8_t b, 
    double bright, uint8_t from = 0, uint8_t to = LED_COUNT)
  {
    const auto color = strip.Color(r * bright, g * bright, b * bright);
    for (uint16_t i = from; i < to; i++)
      strip.setPixelColor(i, color);
    strip.show();
  }
}

void RunBlinking(void* params)
{
    TimeLed* thisObj = reinterpret_cast<TimeLed*>(params);
    if (!thisObj)
        return;

    uint8_t r = 0, g = 0, b = 0;
    thisObj->GetBlinkingColor(r, g, b);
    Debug::Print("\nRunBlinking: the object of TimeLed is valid\n");
    while (true)
    {
        const double step = 0.1;
        const int delayStep = 65;
        for (double bright = 0.0; bright < 1.0; bright += step)
        {
            SetSpecificPixels(thisObj->strip, r, g, b, bright);
            delay(delayStep);
        }

        for (double bright = 1.0; bright > 0.0; bright -= step)
        {
            SetSpecificPixels(thisObj->strip, r, g, b, bright);
            delay(delayStep);
        }
        vTaskDelay(10);
    }
}

TimeLed::TimeLed(const uint8_t pin)
    : strip(LED_COUNT, pin, NEO_GRB + NEO_KHZ800)
    , m_topBrightness(0)
    , m_bottomBrightness(0)
    , m_isTopOff(false)
    , m_topColor()
    , m_bottomColor()
    , m_blinkingColor()
    , m_setupBlinkTask(nullptr)
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
  Debug::Print("\nTimeLed::Show()");
  
  SetSpecificPixels(strip, m_topColor.red, m_topColor.green, m_topColor.blue,
    m_topBrightness, 0, 6);

  SetSpecificPixels(strip, m_bottomColor.red, m_bottomColor.green, m_bottomColor.blue,
    m_bottomBrightness, 6);
}

void TimeLed::Off()
{
    Debug::Print("\nTimeLed::Off()");
    strip.clear();
    strip.show();
}

void TimeLed::OffTop()
{
  SetSpecificPixels(strip, 0, 0, 0, 0.0, 0, 6);
  m_isTopOff = true;
}

void TimeLed::OnTop()
{
  SetSpecificPixels(strip, m_topColor.red, m_topColor.green, m_topColor.blue,
    m_topBrightness, 0, 6);
  m_isTopOff = false;
}

void TimeLed::RunSetupBlinking(const uint8_t r, const uint8_t g, const uint8_t b)
{
  m_blinkingColor.red = r;
  m_blinkingColor.green = g;
  m_blinkingColor.blue = b;

  Debug::Print("\nBlinkSetupProces - true\n");
  xTaskCreatePinnedToCore(
        RunBlinking,   /* Task function. */
        "Setup Blink Task",     /* name of task. */
        10000,       /* Stack size of task */
        this,        /* parameter of the task */
        1,           /* priority of the task */
        &m_setupBlinkTask,      /* Task handle to keep track of created task */
        0);          /* pin task to core 0 */  
}

void TimeLed::StopSetupBlinking()
{
  if (m_setupBlinkTask)
  {
      Debug::Print("\nBlinkSetupProces - false\n");
      vTaskDelete(m_setupBlinkTask);
      Off();
      Show();
      m_setupBlinkTask = nullptr;
  }
}

void TimeLed::GetBlinkingColor(uint8_t& r, uint8_t& g, uint8_t& b)
{
  r = m_blinkingColor.red;
  g = m_blinkingColor.green;
  b = m_blinkingColor.blue;
}

void TimeLed::AdjustBrightness(const double value)
{
  m_topBrightness = value;
  m_bottomBrightness = value;

  if (!m_isTopOff)
  {
    SetSpecificPixels(strip, m_topColor.red, m_topColor.green, m_topColor.blue,
      m_topBrightness, 0, 6);

    SetSpecificPixels(strip, m_bottomColor.red, m_bottomColor.green, m_bottomColor.blue,
      m_bottomBrightness, 6);
  }
  else
  {
    SetSpecificPixels(strip, 0, 0, 0, 0.0, 0, 6);
    SetSpecificPixels(strip, m_bottomColor.red, m_bottomColor.green, m_bottomColor.blue,
      m_bottomBrightness, 6);
  }
}