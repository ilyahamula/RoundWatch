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
    friend void SetIncorrrectTime(const int hours, const int min);
    
private:
    Command();
    Command(const Command&) = delete;
    Command& operator=(const Command&) = delete;

public:
    static Command& Instance();
    const eConcreteCommand GetCommand();
    void GetIncorrectTime(int& hours, int& min);

private:
    TaskHandle_t m_telegramBotTask;
    TaskHandle_t m_serialTask;
    TaskHandle_t m_bluetoothTask;
    eConcreteCommand m_currCmd;
    int m_hours;
    int m_min;
};

#endif