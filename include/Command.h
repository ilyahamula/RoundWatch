#ifndef COMMANDHEADER_H
#define COMMANDHEADER_H

#include <Arduino.h>

enum class eConcreteCommand
{
    eNone,
    eMoveForwardHour,
    eMoveBackwardHour,
    eMoveForwardMin,
    eMoveBackwardMin,
    eMoveFrwdStepHour,
    eMoveFrwdStepMin,
    eMoveBackwardStepHour,
    eMoveBackwardStepMin,
    eIncorrectTime,
    eTimeOffset,
    eSetTopColorHour,
    eSetBottomColorHour,
    eSetTopColorMin,
    eSetBottomColorMin,
};

class Command
{
    friend class TelegramCMD;
    
private:
    Command();
    Command(const Command&) = delete;
    Command& operator=(const Command&) = delete;

private:
    void SetCommand(const eConcreteCommand command);
    void SetIncorrrectTime(const int8_t hours, const int8_t min);
    void SetTimeOffset(const uint32_t offset);

public:
    static Command& Instance();
    const eConcreteCommand GetCommand();
    void GetIncorrectTime(int8_t& hours, int8_t& min);
    uint32_t GetTimeOffset();

private:
    TaskHandle_t m_telegramBotTask;
    TaskHandle_t m_serialTask;
    TaskHandle_t m_bluetoothTask;
    eConcreteCommand m_currCmd;
    int8_t m_hours;
    int8_t m_min;
    uint32_t m_offset;
};

#endif