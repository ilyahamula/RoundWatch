#ifndef COMMANDHEADER_H
#define  COMMANDHEADER_H

enum class eConcreteCommand
{
    eNone,
    eMoveForwardHour,
    eMoveBackwardHour,
    eMoveForwardMin,
    eMoveBackwardMin,
    eSetTime
};

class Command
{
private:
    Command();
    Command(const Command&) = delete;
    Command& operator=(const Command&) = delete;

public:
    Command& Instance();

    const eConcreteCommand GetCommand();

private:
    TaskHandle_t telegramBotTask;
};

#endif