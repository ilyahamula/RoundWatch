#include "RoundWatch.h"
#include "Debug.h"

#define NOT_DEFINED 0

RoundWatch::RoundWatch()
    : m_dials()
    , m_leds()
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

void RoundWatch::AdjustDialSettings(const DIAL dial, const Settings& settings)
{
    SetMottorPins(dial, settings.in1, settings.in2, settings.in3, settings.in4);
    SetLedPin(dial, settings.ledPin);
    SetDivisionsPin(dial, settings.divisionPin);

    SetBottomBrightness(dial, settings.bottomLED.brightness);
    SetBottomColor(dial, settings.bottomLED.red,
        settings.bottomLED.green,
        settings.bottomLED.blue);

    SetTopBrightness(dial, settings.topLED.brightness);
    SetTopColor(dial, settings.topLED.red,
        settings.topLED.green,
        settings.topLED.blue);
}

void RoundWatch::SetMottorPins(const DIAL dial, const uint8_t in1, const uint8_t in2, const uint8_t in3,
    const uint8_t in4)
{
    if (!m_dials[INT(dial)])
        m_dials[INT(dial)] = RoundDial::CreateDial(dial, in1, in2, in3, in4);
}

void RoundWatch::SetDivisionsPin(const DIAL dial, const uint8_t pin)
{
    if (m_dials[INT(dial)])
        m_dials[INT(dial)]->SetDivisionsPin(pin);
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

void RoundWatch::MoveForward(const DIAL dial)
{
    RoundDial* dl = m_dials[INT(dial)];
    if (dl)
        dl->MoveForward();
}

void RoundWatch::MoveBackward(const DIAL dial)
{
    RoundDial* dl = m_dials[INT(dial)];
    if (dl)
        dl->MoveBackward();
}

void RoundWatch::MoveOneDivForward(const DIAL dial)
{
    RoundDial* dl = m_dials[INT(dial)];
    if (dl)
        dl->MoveOneDivForward();
}

void RoundWatch::CalibrateByIncorrectTime(const int hours, const int min)
{
    // TODO: implement
}

void RoundWatch::Setup()
{
    for (uint8_t i = 0; i < INT(DIAL::NUM_DIALS); ++i)
    {
        Debug::Print("\nSetup ");
        Debug::Print(i);
        Debug::Print(" dial");
        if (m_dials[i])
            m_dials[i]->Setup();
        if (m_leds[i])
            m_leds[i]->Show();
    }
    Debug::Print("\nSetup done!");
}

void RoundWatch::SetRealTime(const uint8_t hour, const uint8_t min, const uint8_t sec)
{

    RoundDial* rdHour = m_dials[INT(DIAL::HOURS)];
    if (rdHour)
    {
        rdHour->SetTimeValue(hour);
        if (rdHour->IsCurDivMajor())
        {
            //m_leds[INT(DIAL::HOURS)]->SetTopColor(r, g, b);
        }

    }

    RoundDial* rdMinutes = m_dials[INT(DIAL::MINUTES)];
    if (rdMinutes)
    {
        rdMinutes->SetTimeValue(min);
        if (rdHour->IsCurDivMajor())
        {
            //m_leds[INT(DIAL::MINUTES)]->SetTopColor(r, g, b);
        }
    }
}

void RoundWatch::SetDisplayedTime(const uint8_t hour, const uint8_t min, const uint8_t sec)
{
    RoundDial* rdHour = m_dials[INT(DIAL::HOURS)];
    if (rdHour)
    {
        rdHour->SetTimeValue(hour);
    }

    RoundDial* rdMinutes = m_dials[INT(DIAL::MINUTES)];
    if (rdMinutes)
    {
        rdMinutes->SetTimeValue(min);
    }
}