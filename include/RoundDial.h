#ifndef ROUNDIAL_H
#define  ROUNDIAL_H

#include <Arduino.h>
#include "Stepper_28BYJ_48.h"

class RoundDial
{
public:
    RoundDial(const uint8_t in1, const uint8_t in2, const uint8_t in3, const uint8_t in4);

    void SetNumDivisions(const uint8_t divisions);
    int GetNumDivisions() const;
    int GetCurrDiv() const;
    void Setup();
    void MoveToNextDiv();

public: // for clibration through WEB interface
    void MoveForward();
    void MoveBackward();

private:
    void SaveCurrDivToStorage();
    void UpdateCurrDivFromStorage();

private:
    Stepper_28BYJ_48 m_stepperMotor;

private:
    uint16_t m_numDivisions;
    uint16_t m_stepsPerDiv;
    uint16_t m_currDiv;
};

#endif  
