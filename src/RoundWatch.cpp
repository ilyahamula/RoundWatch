#include "RoundWatch.h"
#include "Debug.h"

#define NOT_DEFINED -1

namespace
{
    double CalculateBrightness(const int val)
    {
        if (val == 3)
            return 0.1;
        if (val == 2)
            return 0.5;
        if (val == 1)
            return 1.0;
        return 0.5;
    }
}

RoundWatch::RoundWatch()
    : m_dials()
    , m_leds()
    , m_timeValue()
    , m_prevAnalogValue(0)
{
    m_timeValue[INT(DIAL::HOURS)] = NOT_DEFINED;
    m_timeValue[INT(DIAL::MINUTES)] = NOT_DEFINED;

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
    {
        m_leds[INT(dial)]->Off();
        m_leds[INT(dial)]->SetTopBrightness(brightness);
        m_leds[INT(dial)]->Show();
    }
}

void RoundWatch::SetBottomBrightness(const DIAL dial, const double brightness)
{
    if (m_leds[INT(dial)])
    {
        m_leds[INT(dial)]->Off();
        m_leds[INT(dial)]->SetBottomBrightness(brightness);
        m_leds[INT(dial)]->Show();
    }
}

void RoundWatch::SetTopColor(const DIAL dial, const uint8_t r, const uint8_t g, const uint8_t b)
{
    if (m_leds[INT(dial)])
    {
        m_leds[INT(dial)]->Off();
        m_leds[INT(dial)]->SetTopColor(r, g, b);
        m_leds[INT(dial)]->Show();
    }
}

void RoundWatch::SetBottomColor(const DIAL dial, const uint8_t r, const uint8_t g, const uint8_t b)
{
    if (m_leds[INT(dial)])
    {
        m_leds[INT(dial)]->Off();
        m_leds[INT(dial)]->SetBottomColor(r, g, b);
        m_leds[INT(dial)]->Show();
    }
}

void RoundWatch::OffLed(const DIAL dial)
{
    if (m_leds[INT(dial)])
        m_leds[INT(dial)]->Off();
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

void RoundWatch::MoveOneDivBackward(const DIAL dial)
{
    RoundDial* dl = m_dials[INT(dial)];
    if (dl)
        dl->MoveOneDivBackward();
}

void RoundWatch::CalibrateByIncorrectTime(const uint8_t hours, const uint8_t min)
{
    Debug::Print("\nRoundWatch::CalibrateByIncorrectTime\n");
    RoundDial* hoursDl = m_dials[INT(DIAL::HOURS)];
    RoundDial* minDl = m_dials[INT(DIAL::MINUTES)];
    if (!(hoursDl && minDl) || 
        m_timeValue[INT(DIAL::HOURS)] == NOT_DEFINED ||
        m_timeValue[INT(DIAL::MINUTES)] == NOT_DEFINED)
        return;
    
    hoursDl->CalibrateByIncorrectTime(m_timeValue[INT(DIAL::HOURS)], hours);
    minDl->CalibrateByIncorrectTime(m_timeValue[INT(DIAL::MINUTES)], min);
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
    }
    Debug::Print("\nSetup done!");
}

void RoundWatch::SetRealTime(const uint8_t hour, const uint8_t min, const uint8_t sec)
{
    auto setTimeForWatch = [this](const DIAL dial, const uint8_t value)
    {
        RoundDial* rdHour = m_dials[INT(dial)];
        TimeLed* ledHour = m_leds[INT(dial)];
        if (rdHour && ledHour)
        {
            rdHour->SetTimeValue(value);
            if (rdHour->IsCurDivMajor())
                ledHour->OnTop();
            else
                ledHour->OffTop();
            m_timeValue[INT(dial)] = value;
        }
    };

    setTimeForWatch(DIAL::HOURS, hour);
    setTimeForWatch(DIAL::MINUTES, min);
}

void RoundWatch::AdjustBrightness(const int analogValue)
{
    int actual = static_cast<int>(analogValue / 1000);
    if (actual != m_prevAnalogValue)
    {
        m_prevAnalogValue = actual;
        const double newBrighness = CalculateBrightness(m_prevAnalogValue);
        
        if (m_leds[INT(DIAL::HOURS)])
            m_leds[INT(DIAL::HOURS)]->AdjustBrightness(newBrighness);
        if (m_leds[INT(DIAL::MINUTES)])
            m_leds[INT(DIAL::MINUTES)]->AdjustBrightness(newBrighness);

        Debug::Print("AnalogValue = ");
		Debug::Print(m_prevAnalogValue);
		Debug::Print("\n");
    }
}

RoundWatch::WatchBlinker::WatchBlinker(RoundWatch& rw, const uint8_t r, const uint8_t g, const uint8_t b)
    : watchRef(rw)
{
    auto hoursLed = watchRef.m_leds[INT(DIAL::HOURS)];
    auto minLed = watchRef.m_leds[INT(DIAL::MINUTES)];
    if (hoursLed)
        hoursLed->RunSetupBlinking(r, g, b);
    if (minLed)
        minLed->RunSetupBlinking(r, g, b);
}

RoundWatch::WatchBlinker::~WatchBlinker()
{
    auto hoursLed = watchRef.m_leds[INT(DIAL::HOURS)];
    auto minLed = watchRef.m_leds[INT(DIAL::MINUTES)];
    if (hoursLed)
        hoursLed->StopSetupBlinking();
    if (minLed)
        minLed->StopSetupBlinking();
}
