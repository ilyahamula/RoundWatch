#include "RoundDial.h"
#include "Debug.h"

#define NOT_DEFINED 0

RoundDial* RoundDial::CreateDial(const DIAL type, const uint8_t in1, const uint8_t in2, const uint8_t in3, const uint8_t in4)
{
    if (type == DIAL::HOURS)
        return new RoundDialHours(in1, in2, in3, in4);
    if (type == DIAL::MINUTES)
        return new RoundDialHours(in1, in2, in3, in4);

    return nullptr;
}

RoundDial::RoundDial(const uint8_t in1, const uint8_t in2, const uint8_t in3, const uint8_t in4)
    : m_stepperMotor(in1, in2, in3, in4)
    , m_numDivisions(NOT_DEFINED)
    , m_stepsPerDiv(NOT_DEFINED)
    , m_currDiv(0)
{
}

RoundDial::~RoundDial()
{
}

void RoundDial::Setup()
{
    Debug::Print("Setup of Dial\n");
    UpdateCurrDivFromStorage();
}

void RoundDial::MoveToNextDiv()
{
    m_stepperMotor.step(m_stepsPerDiv);
    m_currDiv++;
    if (m_currDiv == m_numDivisions)
        m_currDiv = 0;
    SaveCurrDivToStorage();
}

void RoundDial::MoveForward()
{
    // Save to EEPROOM
    //m_currDiv;
}

void RoundDial::MoveBackward()
{
    // Save to EEPROOM
    //m_currDiv;
}

void RoundDial::SaveCurrDivToStorage()
{
    // Save to EEPROOM
    //m_currDiv;
}

void RoundDial::UpdateCurrDivFromStorage()
{
    m_currDiv = 0;// Read from EEPROOM
}

//--------------------Dial of Hours---------------------------------------------------------------------------------------------------------
bool RoundDialHours::m_majorDiv[HOURS_DIV_NUM] = { true, false, false, true, false, false, true, false, false, 
true, false, false, true, false, false, true, false, false, true, false, false, true, false, false };

RoundDialHours::RoundDialHours(const uint8_t in1, const uint8_t in2, const uint8_t in3, const uint8_t in4)
    : RoundDial(in1, in2, in3, in4)
{
    m_numDivisions = HOURS_DIV_NUM;
    m_stepsPerDiv = 100;
    m_currDiv = 0;
}

DIAL RoundDialHours::GetType() const
{
    return DIAL::HOURS;
}

void RoundDialHours::SetTimeValue(const uint8_t value)
{
    while (value > m_currDiv || (m_currDiv == 23 && value == 0))
    {
        MoveToNextDiv();
        Debug::Print("Hours Moved to: ");
        Debug::Print(m_currDiv);
    }
}

bool RoundDialHours::IsCurDivMajor() const
{
    return m_majorDiv[m_currDiv];
}

//--------------------Dial of Minutes-------------------------------------------------------------------------------------------------------
bool RoundDialMinutes::m_majorDiv[MIN_DIV_NUM] = { true, false, false, false, true, false, false, false, true, 
false, false, false, true, false, false, false, true, false, false, false, true, false, false, false };

int RoundDialMinutes::m_minToDiv[MINUTES_NUM] = 
{ //0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29   // min
    0, 0, 1, 1, 2, 2, 2, 3, 3, 4, 4, 4, 5, 5, 6, 6, 6, 7, 7, 8, 8, 8, 9, 9, 10,10,10,11,11,12,  // div

  //30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59   // min
    12,12,13,13,14,14,14,15,15,16,16,16,17,17,18,18,18,19,19,20,20,20,21,21,22,22,22,23,23,23   // div
};

RoundDialMinutes::RoundDialMinutes(const uint8_t in1, const uint8_t in2, const uint8_t in3, const uint8_t in4)
    : RoundDial(in1, in2, in3, in4)
{
    m_numDivisions = MIN_DIV_NUM;
    m_stepsPerDiv = 100;
    m_currDiv = 0;
}

DIAL RoundDialMinutes::GetType() const
{
    return DIAL::MINUTES;
}

void RoundDialMinutes::SetTimeValue(const uint8_t value)
{
    int actualDiv = m_minToDiv[value];
    while (actualDiv > m_currDiv || (m_currDiv == 23 && actualDiv == 0))
    {
        MoveToNextDiv();
        Debug::Print("Minutes Moved to: ");
        Debug::Print(m_currDiv);
    }
}

bool RoundDialMinutes::IsCurDivMajor() const
{
    return m_majorDiv[m_currDiv];
}
