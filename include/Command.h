#ifndef COMMANDHEADER_H
#define  COMMANDHEADER_H

class Command
{
private:
    Command() = default;
    Command(const Command&) = delete;
    Command& operator=(const Command&) = delete;

public:
    enum class eConcreteCommand
    {
        eNone,
        eMoveForwardHour,
        eMoveBackwardHour,
        eMoveForwardMin,
        eMoveBackwardMin,
        eSetTime
    };

public:
    Command& Instance();

    const eConcreteCommand GetCommand();
};

#endif