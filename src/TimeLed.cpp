#include "TimeLed.h"
#include "Debug.h"

#define LED_COUNT 9

void RunBlinking(void* params)
{
    TimeLed* thisObj = reinterpret_cast<TimeLed*>(params);
    if (!thisObj)
        return;

    Debug::Print("\nRunBlinking: the object of TimeLed is valid\n");

    while (true)
    {
        const double step = 0.1;
        const int delayStep = 80;
        for (double bright = 0.0; bright < 1.0; bright += step)
        {
            for (uint16_t i = 0; i < LED_COUNT; i++)
            {
              const uint32_t color = thisObj->strip.Color(255 * bright, 
                                              255 * bright, 
                                              255 * bright);
              thisObj->strip.setPixelColor(i, color);
            }
            thisObj->strip.show();
            delay(delayStep);
        }

        for (double bright = 1.0; bright > 0.0; bright -= step)
        {
            for (uint16_t i = 0; i < LED_COUNT; i++)
            {
              const uint32_t color = thisObj->strip.Color(255 * bright, 
                                              255 * bright, 
                                              255 * bright);
              thisObj->strip.setPixelColor(i, color);
            }
            thisObj->strip.show();
            delay(delayStep);
        }
        vTaskDelay(10);
    }
}

TimeLed::TimeLed(const uint8_t pin)
    : strip(LED_COUNT, pin, NEO_GRB + NEO_KHZ800)
    , m_topBrightness(0)
    , m_bottomBrightness(0)
    , m_topColor()
    , m_bottomColor()
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

void TimeLed::Off()
{
    Debug::Print("\nTimeLed::Off()");
    strip.clear();
    strip.show();
}

void TimeLed::RunSetupBlinking()
{
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
    