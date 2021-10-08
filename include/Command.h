#ifndef COMMANDHEADER_H
#define  COMMANDHEADER_H

enum class eConcreteCommand
{
    eNone,
    eMoveForwardHour,
    eMoveBackwardHour,
    eMoveForwardMin,
    eMoveBackwardMin,
    eMoveFrwdStepHour,
    eMoveFrwdStepMin,
    eIncorrectTime,
};

class Command
{
    friend void SetCommand(const eConcreteCommand command);
    friend void SetIncorrrectTime(const uint8_t hours, const uint8_t min);
    
private:
    Command();
    Command(const Command&) = delete;
    Command& operator=(const Command&) = delete;

public:
    static Command& Instance();
    const eConcreteCommand GetCommand();
    void GetIncorrectTime(uint8_t& hours, uint8_t& min);

private:
    TaskHandle_t m_telegramBotTask;
    TaskHandle_t m_serialTask;
    TaskHandle_t m_bluetoothTask;
    eConcreteCommand m_currCmd;
    uint8_t m_hours;
    uint8_t m_min;
};

#endif