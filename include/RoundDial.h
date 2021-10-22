#ifndef ROUNDIAL_H
#define  ROUNDIAL_H

#include <Arduino.h>
#include "Stepper_28BYJ_48.h"

#define HOURS_DIV_NUM 24
#define MIN_DIV_NUM 24
#define MINUTES_NUM 60


enum class DIAL
{
    HOURS = 0,
    MINUTES = 1,

    NUM_DIALS = 2
};

class RoundDial
{
    friend class Debug;
public:
    static RoundDial* CreateDial(const DIAL type, const uint8_t in1, const uint8_t in2, const uint8_t in3, const uint8_t in4);

public:
    RoundDial(const uint8_t in1, const uint8_t in2, const uint8_t in3, const uint8_t in4);
    virtual ~RoundDial();

    virtual DIAL GetType() const = 0;
    virtual void SetTimeValue(const uint8_t value) = 0;
    virtual bool IsCurDivMajor() const = 0;
    virtual void Setup() = 0;
    virtual void CalibrateByIncorrectTime(const uint8_t realTime, const uint8_t shownTime) = 0;

    void SetDivisionsPin(const uint8_t pin);

public: // for calibration through WEB interface
    void MoveForward();
    void MoveBackward();
    void MoveOneDivForward();
    void MoveOneDivBackward();
    
protected:
    void MoveStep(const bool forward = true);
    void MoveToNextDiv();
    void MoveToPrevDiv();
    void MoveByDifferense(const int8_t diff);
    virtual void SaveCurrDivToStorage() = 0;

protected:
    Stepper_28BYJ_48 m_stepperMotor;

protected:
    uint8_t m_numDivisions;
    uint16_t m_stepsPerDiv;
    uint8_t m_currDiv;
    uint8_t m_divisionsPin;
};

class RoundDialHours : public RoundDial
{
public:
    RoundDialHours(const uint8_t in1, const uint8_t in2, const uint8_t in3, const uint8_t in4);

    DIAL GetType() const override;
    void SetTimeValue(const uint8_t value) override;
    bool IsCurDivMajor() const override;
    void Setup() override;
    void CalibrateByIncorrectTime(const uint8_t realTime, const uint8_t shownTime) override;

protected:
    void SaveCurrDivToStorage() override;

private:
    static bool m_majorDiv[HOURS_DIV_NUM];
};

class RoundDialMinutes : public RoundDial
{
public:
    RoundDialMinutes(const uint8_t in1, const uint8_t in2, const uint8_t in3, const uint8_t in4);

    DIAL GetType() const override;
    void SetTimeValue(const uint8_t value) override;
    bool IsCurDivMajor() const override;
    void Setup() override;
    void CalibrateByIncorrectTime(const uint8_t realTime, const uint8_t shownTime) override;

protected:
    void SaveCurrDivToStorage() override;

private:
    static bool m_majorDiv[MIN_DIV_NUM];
    static int m_minToDiv[MINUTES_NUM];
};

#endif  
