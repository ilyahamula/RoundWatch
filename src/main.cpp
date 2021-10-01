#include <Arduino.h>
#include <Wire.h>
#include "DS1307.h"

#include "Debug.h"
#include "Settings.h"
#include "RoundWatch.h"

RoundWatch watch; 
DS1307 myClock;

void setup()
{
	Debug::Print("--------Setup()\n");
	SetupHoursDial(watch);
	SetupMinutesDial(watch);
    watch.Setup();

	Debug::Print("--------end Setup()\n");                                                                         
}

void loop()
{
	if (Serial.available() > 0)
	{
		const uint8_t byte = Serial.read();
		Serial.println(byte);

		if (byte == 49) // 1
			watch.MoveForward(DIAL::HOURS);
		else if (byte == 50) // 2
			watch.MoveBackward(DIAL::HOURS);
		else if (byte == 51) //3
			watch.MoveForward(DIAL::MINUTES);
		else if (byte == 52) // 4
			watch.MoveBackward(DIAL::MINUTES);

		else if (byte == 53) // 5
			watch.MoveOneDivForward(DIAL::HOURS);
		else if (byte == 54) // 6
			watch.MoveOneDivForward(DIAL::MINUTES);

		Serial.flush();
	}

    Debug::PrintTime(myClock);

	myClock.getTime();
	watch.SetRealTime(myClock.hour, myClock.minute);
}