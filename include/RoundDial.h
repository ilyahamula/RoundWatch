#ifndef ROUNDIAL_H
#define ROUNDIAL_H

#include <Arduino.h>
#include "Driver.h"

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
    static RoundDial* CreateDial(const DIAL type, const Driver::Settings& settings);

public:
    RoundDial(const Driver::Settings& settings);
    virtual ~RoundDial();

    virtual DIAL GetType() const = 0;
    virtual void SetTimeValue(const uint8_t value) = 0;
    virtual bool IsCurDivMajor() const = 0;
    virtual void Setup() = 0;
    virtual void CalibrateByIncorrectTime(const uint8_t realTime, const uint8_t shownTime) = 0;

public: // for calibration through WEB interface
    void MoveForward();
    void MoveBackward();
    void MoveOneDivForward();
    void MoveOneDivBackward();
    
protected:
    void MoveToNextDiv();
    void MoveToPrevDiv();
    void MoveByDifferense(const int8_t diff);
    virtual void SaveCurrDivToStorage() = 0;

protected:
    Driver* m_driver;

protected:
    uint8_t m_numDivisions;
    uint16_t m_stepsPerDiv;
    uint8_t m_currDiv;
};

class RoundDialHours : public RoundDial
{
public:
    RoundDialHours(const Driver::Settings& settings);

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
    RoundDialMinutes(const Driver::Settings& settings);

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
