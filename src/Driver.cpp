#include "Driver.h"

#define MIN_STEP 2
#define USER_STEP 10

Driver* Driver::CreateDriver(const Driver::Settings& settings)
{
    if (settings.type == Driver::eType::eStepper_28BYJ_48)
        return new DriverStepper(settings.in1, settings.in2,
            settings.in3, settings.in4, settings.divisionPin);
    if (settings.type == Driver::eType::eDC_Motor)
        return new DriverDCMotor(settings.dcPin, settings.divisionPin);
}

void DriverStepper::MoveStep(const bool forward)
{
#ifdef OLD_MOVE_DIVISION
    const int stepsDirection = (forward ? m_stepsPerDiv : (-1 * m_stepsPerDiv));
    m_stepperMotor.step(stepsDirection); // old realization
#else
    const int stepsDirection = (forward ? MIN_STEP : (-1 * MIN_STEP));
    while (forward && !digitalRead(m_divisionsPin))
        m_stepperMotor.step(stepsDirection);

    while(digitalRead(m_divisionsPin))
        m_stepperMotor.step(stepsDirection);

    while(!digitalRead(m_divisionsPin))
        m_stepperMotor.step(stepsDirection);

    while (!forward && digitalRead(m_divisionsPin))
        m_stepperMotor.step(stepsDirection);
#endif
}

void DriverStepper::MoveForward()
{
    m_stepperMotor.step(USER_STEP);
}

void DriverStepper::MoveBackward()
{
    m_stepperMotor.step(-1 * USER_STEP);
}

void DriverDCMotor::MoveStep(const bool forward)
{

}

void DriverDCMotor::MoveForward()
{

}

void DriverDCMotor::MoveBackward()
{

}
