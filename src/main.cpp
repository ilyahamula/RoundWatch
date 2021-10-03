#include <Arduino.h>
#include <Wire.h>
#include "DS1307.h"

#include "Debug.h"
#include "Settings.h"
#include "Command.h"
#include "RoundWatch.h"

RoundWatch watch; 
DS1307 myClock;

void setup()
{
	Debug::Print("--------Setup()\n");
	SetupWatch(watch);
	Debug::Print("--------end Setup()\n");                                                                         
}

void loop()
{
	const auto cmd = Command::Instance().GetCommand();
	if (cmd == eConcreteCommand::eMoveForwardHour)
		watch.MoveForward(DIAL::HOURS);
	else if (cmd == eConcreteCommand::eMoveBackwardHour)
		watch.MoveBackward(DIAL::HOURS);
	else if (cmd == eConcreteCommand::eMoveForwardMin)
		watch.MoveForward(DIAL::MINUTES);
	else if (cmd == eConcreteCommand::eMoveBackwardMin)
		watch.MoveBackward(DIAL::MINUTES);
	else if (cmd == eConcreteCommand::eMoveFrwdStepHour)
		watch.MoveOneDivForward(DIAL::HOURS);
	else if (cmd == eConcreteCommand::eMoveFrwdStepMin)
		watch.MoveOneDivForward(DIAL::MINUTES);

    Debug::PrintTime(myClock);
	myClock.getTime();
	watch.SetRealTime(myClock.hour, myClock.minute);
}