#include "RoundDial.h"
#include "Debug.h"

#define NOT_DEFINED 0
#define STEP_PER_DEG 20

RoundDial::RoundDial(const uint8_t in1, const uint8_t in2, const uint8_t in3, const uint8_t in4)
    : m_stepperMotor(in1, in2, in3, in4)
    , m_numDivisions(NOT_DEFINED)
    , m_stepsPerDiv(NOT_DEFINED)
    , m_currDiv(0)
{
}

void RoundDial::SetNumDivisions(const uint8_t divisions)
{
    m_numDivisions = divisions;
}

int RoundDial::GetNumDivisions() const
{
    return m_numDivisions;
}

int RoundDial::GetCurrDiv() const
{
    return m_currDiv;
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

void RoundDial::SaveCurrDivToStorage()
{
    // Save to EEPROOM
    //m_currDiv;
}

void RoundDial::UpdateCurrDivFromStorage()
{
    m_currDiv = 0;// Read from EEPROOM
}
