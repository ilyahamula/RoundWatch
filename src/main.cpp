#include <Arduino.h>
#include <WiFi.h>

#include "Debug.h"
#include "Settings.h"
#include "Command.h"
#include "RoundWatch.h"

RoundWatch watch; 

void setup()
{
	Debug::Print("--------Setup()\n");
	WiFi.mode(WIFI_STA);
  	WiFi.begin(SSID, PASSWORD);
 
  	while (WiFi.status() != WL_CONNECTED) 
	  {
    	delay(1000);
    	Debug::Print("Connecting to WiFi..\n");
  	}
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

	//watch.SetRealTime(myClock.hour, myClock.minute);
}