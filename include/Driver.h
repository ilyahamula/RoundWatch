#ifndef DRIVERHEADER_H
#define  DRIVERHEADER_H

#include <Arduino.h>
#include "Stepper_28BYJ_48.h"

class Driver
{
public:
    enum class eType
    {
        eNone,
        eStepper_28BYJ_48,
        eDC_Motor
    };

    struct Settings
    {
        eType type = eType::eNone;
        uint8_t in1 = 0;
        uint8_t in2 = 0;
        uint8_t in3 = 0;
        uint8_t in4 = 0;
        uint8_t dcPin = 0;
        uint8_t divisionPin = 0;
    };

    static Driver* CreateDriver(const Settings& settings);

public:
    Driver(const uint8_t divPin)
        : m_divisionsPin(divPin) 
    { pinMode(m_divisionsPin, INPUT); };

public:
    virtual ~Driver();
    virtual void MoveStep(const bool forward = true) = 0;
    virtual void MoveForward() = 0;
    virtual void MoveBackward() = 0;

protected:
    uint8_t m_divisionsPin;
};

class DriverStepper : public Driver
{
public:
    DriverStepper::DriverStepper(const uint8_t in1, const uint8_t in2, 
            const uint8_t in3, const uint8_t in4, const uint8_t divPin)
        : Driver(divPin) , m_stepperMotor(in1, in2, in3, in4) {};

public:
    void MoveStep(const bool forward = true) override;
    void MoveForward() override;
    void MoveBackward() override;

private:
    Stepper_28BYJ_48 m_stepperMotor;
};

class DriverDCMotor : public Driver
{
public:
    DriverDCMotor(const uint8_t dcPin, const uint8_t divPin)
        : Driver(divPin), m_dcPin(dcPin) {};

public:
    void MoveStep(const bool forward = true) override;
    void MoveForward() override;
    void MoveBackward() override;

private:
    uint8_t m_dcPin;
};

#endif