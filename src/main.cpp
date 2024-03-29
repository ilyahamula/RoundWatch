#include <Arduino.h>
#include <WiFi.h>

#include "Debug.h"
#include "Settings.h"
#include "Command.h"
#include "Time.h"
#include "RoundWatch.h"

RoundWatch watch; 

void setup()
{
	Debug::Print("--------Setup()\n");
	SetupWatch(watch);
	RoundWatch::WatchBlinker adjuster(watch, 255, 255, 255);
	
	WiFi.mode(WIFI_STA);
	auto connectWiFi = []()
	{
		WiFi.begin(SSID_DEF, PASSWORD);
		while (WiFi.status() != WL_CONNECTED)
		{
    		delay(1000);
    		Debug::Print("Connecting to WiFi..\n");
  		}
	};

	connectWiFi();

	WiFi.onEvent([&connectWiFi](WiFiEvent_t event, WiFiEventInfo_t info){
		RoundWatch::WatchBlinker adjuster(watch, 255, 0, 0);
  		Debug::Print("\nWiFi lost connection. Trying to Reconnect");
  		WiFi.disconnect();
		connectWiFi();
	}, SYSTEM_EVENT_STA_DISCONNECTED);

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
	else if (cmd == eConcreteCommand::eMoveBackwardStepHour)
		watch.MoveOneDivBackward(DIAL::HOURS);
	else if (cmd == eConcreteCommand::eMoveBackwardStepMin)
		watch.MoveOneDivBackward(DIAL::MINUTES);
	else if (cmd == eConcreteCommand::eIncorrectTime)
	{ 
		int8_t hours = -1;
		int8_t min = -1;
		Command::Instance().GetIncorrectTime(hours, min); 
		if (hours != -1 && min != -1)
			watch.CalibrateByIncorrectTime(hours, min);
	}
	else if (cmd == eConcreteCommand::eTimeOffset)
	{ 
		const uint32_t offset = Command::Instance().GetTimeOffset(); 
		TimeManager::Instance().SetTimeOffset(offset);
	}

	const auto timeStruct = TimeManager::Instance().GetTime();
	if (timeStruct.hours != -1)
	{
		Debug::PrintTime(timeStruct.hours, timeStruct.minutes);
		watch.SetRealTime(timeStruct.hours, timeStruct.minutes);
	}

	watch.AdjustBrightness(analogRead(LIGHT_SENSOR_PIN));
}
