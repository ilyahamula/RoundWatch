#include "RoundDial.h"
#include "Debug.h"

#define NOT_DEFINED 0

RoundDial::RoundDial(const uint8_t in1, const uint8_t in2, const uint8_t in3, const uint8_t in4)
    : m_stepperMotor(in1, in2, in3, in4)
    , m_numDivisions(NOT_DEFINED)
    , m_stepsPerDiv(NOT_DEFINED)
    , m_stopperPin(NOT_DEFINED)
{
}

void RoundDial::SetNumDivisions(const uint8_t divisions)
{
    m_numDivisions = divisions;
}

void RoundDial::SetStopperPin(const uint8_t pin)
{
    m_stopperPin = pin;
    pinMode(m_stopperPin, INPUT_PULLDOWN);
}

void RoundDial::Setup()
{
    RunToZero();
    bool madeFullRound = false;
    while (digitalRead(m_stopperPin) && !madeFullRound)
    {
        m_stepperMotor.step(1);
        while (!digitalRead(m_stopperPin))
        {
            m_stepperMotor.step(1);
            madeFullRound = true;
        }
    }
}

void RoundDial::MoveToNextDiv()
{

}

void RoundDial::RunToZero()
{
    Debug::Print("Start run to zero\n");
    while (!digitalRead(m_stopperPin))
        m_stepperMotor.step(1);

    Debug::Print("End run to zero\n");
}