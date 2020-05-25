#ifndef ROUNDIAL_H
#define  ROUNDIAL_H

#include <Arduino.h>
#include <Stepper.h>

class RoundDial
{
public:
    RoundDial(const uint8_t in1, const uint8_t in2, const uint8_t in3, const uint8_t in4);

    void SetNumDivisions(const uint8_t divisions);
    void SetStopperPin(const uint8_t pin);
    void Setup();
    void MakeStep();

private:
    void RunToZero();

private:
    uint16_t numDivision;
    uint16_t m_stepsPerDiv;
    uint8_t m_stopperPin;
    Stepper m_stepperMotor;
};

#endif  
