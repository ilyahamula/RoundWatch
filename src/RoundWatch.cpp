#include "RoundWatch.h"
#include "Debug.h"

#define NOT_DEFINED 0

RoundWatch::RoundWatch()
    : m_dials()
    , m_leds()
    , m_majorDiv()
    , m_timeValue()
{
    m_timeValue[INT(DIAL::HOURS)] = 0;
    m_timeValue[INT(DIAL::MINUTES)] = 0;

    m_dials[INT(DIAL::HOURS)] = nullptr;
    m_dials[INT(DIAL::MINUTES)] = nullptr;
    m_leds[INT(DIAL::HOURS)] = nullptr;
    m_leds[INT(DIAL::MINUTES)] = nullptr;
}

RoundWatch::~RoundWatch()
{
    if (!m_dials[INT(DIAL::HOURS)])
        delete m_dials[INT(DIAL::MINUTES)];
    if (!m_dials[INT(DIAL::MINUTES)])
        delete m_dials[INT(DIAL::HOURS)];
    if (!m_leds[INT(DIAL::HOURS)])
        delete m_leds[INT(DIAL::HOURS)];
    if (!m_leds[INT(DIAL::MINUTES)])
        delete m_leds[INT(DIAL::MINUTES)];
}

void RoundWatch::SetMottorPins(const DIAL dial, const uint8_t in1, const uint8_t in2, const uint8_t in3, const uint8_t in4)
{
    if (!m_dials[INT(dial)])
        m_dials[INT(dial)] = new RoundDial(in1, in2, in3, in4);
}

void RoundWatch::SetNumDivisions(const DIAL dial, const uint8_t divisions)
{
    if (m_dials[INT(dial)])
        m_dials[INT(dial)]->SetNumDivisions(divisions);
    
    m_majorDiv[INT(dial)] = new bool[divisions];
    for (uint16_t i = 0; i < divisions; ++i)
        m_majorDiv[INT(dial)][i] = false;
}

void RoundWatch::SetMajorDivisions(const DIAL dial, uint16_t n, uint16_t* array)
{
   if (!m_majorDiv[INT(dial)])
    return;
    
   for (uint16_t i = 0; i < n; ++i)
    m_majorDiv[INT(dial)][array[i]] = true;
}

void RoundWatch::SetStopperPin(const DIAL dial, const uint8_t pin)
{
    if (m_dials[INT(dial)])
        m_dials[INT(dial)]->SetStopperPin(pin);
}

void RoundWatch::SetLedPin(const DIAL dial, const uint8_t pin)
{
    if (!m_leds[INT(dial)])
        m_leds[INT(dial)] = new TimeLed(pin);
}

void RoundWatch::SetTopBrightness(const DIAL dial, const double brightness)
{
    if (m_leds[INT(dial)])
        m_leds[INT(dial)]->SetTopBrightness(brightness);
}

void RoundWatch::SetBottomBrightness(const DIAL dial, const double brightness)
{
    if (m_leds[INT(dial)])
        m_leds[INT(dial)]->SetBottomBrightness(brightness);
}

void RoundWatch::SetTopColor(const DIAL dial, const uint8_t r, const uint8_t g, const uint8_t b)
{
    if (m_leds[INT(dial)])
        m_leds[INT(dial)]->SetTopColor(r, g, b);
}

void RoundWatch::SetBottomColor(const DIAL dial, const uint8_t r, const uint8_t g, const uint8_t b)
{
    if (m_leds[INT(dial)])
        m_leds[INT(dial)]->SetBottomColor(r, g, b);
}

void RoundWatch::Setup()
{
    // probably better to use threads
    for (uint8_t i = 0; i < INT(DIAL::NUM_DIALS); ++i)
    {
        if (m_dials[i])
            m_dials[i]->Setup();
    }
}

void RoundWatch::SetTime(const int16_t hour, const int16_t min, const int16_t sec)
{
    
}
