#include <EEPROM.h>

#include "RoundDial.h"
#include "Debug.h"

#define NOT_DEFINED 0
#define EEPROM_SIZE 2
#define HOURS_ADDRESS 0
#define MIN_ADDRESS 1
#define MIN_STEP 3
#define USER_STEP 10

//#define OLD_MOVE_DIVISION

namespace
{
    int8_t DivisionsShortestDist(const int8_t real, const int8_t shown)
    {
        if (real > shown)
        {
            const int16_t diff = real - shown;
            if (diff > 12)
                return (24 - diff) * -1;
            return diff;
        }
        else if (real < shown)
        {
            const int16_t diff = (real - shown) * -1;
            if (diff < -12)
                return (24 + diff);
            return diff;
        }
        return 0;
    }
}

RoundDial* RoundDial::CreateDial(const DIAL type, const uint8_t in1, const uint8_t in2, const uint8_t in3, const uint8_t in4)
{
    static bool isFlashInit = false;
    if (!isFlashInit)
    {
        EEPROM.begin(EEPROM_SIZE);
        isFlashInit = true;
    }

    if (type == DIAL::HOURS)
        return new RoundDialHours(in1, in2, in3, in4);
    if (type == DIAL::MINUTES)
        return new RoundDialMinutes(in1, in2, in3, in4);

    return nullptr;
}

RoundDial::RoundDial(const uint8_t in1, const uint8_t in2, const uint8_t in3, const uint8_t in4)
    : m_stepperMotor(in1, in2, in3, in4)
    , m_numDivisions(NOT_DEFINED)
    , m_stepsPerDiv(NOT_DEFINED)
    , m_currDiv(0)
    , m_divisionsPin(NOT_DEFINED)
{
}

RoundDial::~RoundDial()
{
}

void RoundDial::SetDivisionsPin(const uint8_t pin)
{
    Debug::Print("\nSetDivisionsPin()");
    m_divisionsPin = pin;
    pinMode(m_divisionsPin, INPUT); // division button should be connected to HIGH level
}

void RoundDial::MoveStep(const bool forward)
{
#ifdef OLD_MOVE_DIVISION
    const int stepsDirection = (forward ? m_stepsPerDiv : (-1 * m_stepsPerDiv));
    m_stepperMotor.step(stepsDirection); // old realization
#else
    const int stepsDirection = (forward ? MIN_STEP : (-1 * MIN_STEP));
    while(!digitalRead(m_divisionsPin))
        m_stepperMotor.step(stepsDirection);

    while(digitalRead(m_divisionsPin))
        m_stepperMotor.step(stepsDirection);
#endif
}

void RoundDial::MoveToNextDiv()
{
    Debug::Print("\nMoveToNextDiv()");
    MoveStep();
    m_currDiv++;
    if (m_currDiv == m_numDivisions)
        m_currDiv = 0;
    SaveCurrDivToStorage();
}

void RoundDial::MoveToPrevDiv()
{
    Debug::Print("\nMoveToPrevDiv()");
    MoveStep(false);
    m_currDiv--;
    if (m_currDiv < 0)
        m_currDiv = 0;
    SaveCurrDivToStorage();
}

void RoundDial::MoveByDifferense(const int8_t diff)
{
    if (diff > 0)
    {
        for (int8_t i = diff; i > 0; --i)
            MoveStep();
    }
    else if (diff < 0)
    {
        for (int8_t i = diff; i < 0; ++i)
            MoveStep(false);
    }
}

void RoundDial::MoveForward()
{
    m_stepperMotor.step(USER_STEP);
}

void RoundDial::MoveBackward()
{
    m_stepperMotor.step(-1 * USER_STEP);
}

void RoundDial::MoveOneDivForward()
{
    MoveStep();
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
    if (value >= HOURS_DIV_NUM)
        return;
        
    while (value > m_currDiv || (m_currDiv == 23 && value == 0))
    {
        MoveToNextDiv();
        Debug::Print("\n(Hours) Moved to: ");
        Debug::Print(m_currDiv);
    }
    while (value < m_currDiv)
    {
        MoveToPrevDiv();
        Debug::Print("\n(Hours) Moved to: ");
        Debug::Print(m_currDiv);
    }
}

bool RoundDialHours::IsCurDivMajor() const
{
    return m_majorDiv[m_currDiv];
}

void RoundDialHours::Setup()
{
    m_currDiv = EEPROM.read(HOURS_ADDRESS);
    if (m_currDiv > m_numDivisions)
        m_currDiv = 0;
    Debug::Print("\n(Hours) updated actual division: ");
    Debug::Print(m_currDiv);
}

void RoundDialHours::SaveCurrDivToStorage()
{
    EEPROM.write(HOURS_ADDRESS, m_currDiv);
    EEPROM.commit();
    Debug::Print("\n(Hours) saved actual division: ");
    Debug::Print(m_currDiv);
}

void RoundDialHours::CalibrateByIncorrectTime(const uint8_t realTime, const uint8_t shownTime)
{
    const int8_t diff = DivisionsShortestDist(realTime, shownTime);
    Debug::Print("HOURS DIAL: CalibrateByIncorrectTime(), diff in div = ");
    Debug::Print(diff);
    Debug::Print("\n");

    MoveByDifferense(diff);
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
    if (value >= MINUTES_NUM)
        return;
        
    int actualDiv = m_minToDiv[value];
    while (actualDiv > m_currDiv || (m_currDiv == 23 && actualDiv == 0))
    {
        MoveToNextDiv();
        Debug::Print("\n(Minutes) Moved to: ");
        Debug::Print(m_currDiv);
    }
    while (actualDiv < m_currDiv)
    {
        MoveToPrevDiv();
        Debug::Print("\n(Minutes) Moved to: ");
        Debug::Print(m_currDiv);
    }
}

bool RoundDialMinutes::IsCurDivMajor() const
{
    return m_majorDiv[m_currDiv];
}

void RoundDialMinutes::Setup()
{
    m_currDiv = EEPROM.read(MIN_ADDRESS);
    if (m_currDiv > m_numDivisions)
        m_currDiv = 0;
    Debug::Print("\n(Minutes) updated actual division: ");
    Debug::Print(m_currDiv);
}

void RoundDialMinutes::SaveCurrDivToStorage()
{
    EEPROM.write(MIN_ADDRESS, m_currDiv);
    EEPROM.commit();
    Debug::Print("\n(Minutes) saved actual division: ");
    Debug::Print(m_currDiv);
}

void RoundDialMinutes::CalibrateByIncorrectTime(const uint8_t realTime, const uint8_t shownTime)
{
    const int8_t realDiv = m_minToDiv[realTime];
    const int8_t shownDiv = m_minToDiv[shownTime];

    const int8_t diff = DivisionsShortestDist(realDiv, shownDiv);
    Debug::Print("MINUTES DIAL: CalibrateByIncorrectTime(), diff in div = ");
    Debug::Print(diff);
    Debug::Print("\n");

    MoveByDifferense(diff);
}
