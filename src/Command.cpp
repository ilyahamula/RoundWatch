#include <Arduino.h>
#include "Command.h"

Command& Command::Instance()
{
    static Command inst;
    return inst;
}

const eConcreteCommand Command::GetCommand()
{
    return eConcreteCommand::eNone;
    // TODO: ask Serial and WEBServer for available input and procced them
}