#include <Arduino.h>
#include <Wire.h>
#include <EEPROM.h>
#include "DS1307.h"

#define TEST_HOURS_DIAL Debug::TestForDialSystem(12, 13, 14, 26, 5, 16)
#define TEST_MINUTES_DIAL Debug::TestForDialSystem(27, 33, 25, 35, 18, 17)
#define TEST_HOURS_STEP_MOVING Debug::TestForMottorAndDivisions(DIAL::HOURS, 12, 13, 14, 26, 16)
#define TEST_MINUTES_STEP_MOVING Debug::TestForMottorAndDivisions(DIAL::MINUTES, 27, 33, 25, 35, 17)

#include "Debug.h"
#include "RoundWatch.h"

#define IS_SETUP_CLOCK_ADDR 2
#define IS_SETTUP 137

RoundWatch watch; 
DS1307 myClock;//define a object of DS1307 class

void SetupHoursDial()
{
	RoundWatch::Settings settings;
	settings.in1 = 12;
	settings.in2 = 13;
	settings.in3 = 14;
	settings.in4 = 26;
	settings.ledPin = 5;
	settings.divisionPin = 16;
	settings.topLED.red = 0;
	settings.topLED.green = 0;
	settings.topLED.blue = 255;
	settings.topLED.brightness = 1.0;
	settings.bottomLED.red = 0;
	settings.bottomLED.green = 255;
	settings.bottomLED.blue = 0;
	settings.bottomLED.brightness = 1.0;

	watch.AdjustDialSettings(DIAL::HOURS, settings);
}

void SetupMinutesDial()
{
	RoundWatch::Settings settings;
	settings.in1 = 27;
	settings.in2 = 33;
	settings.in3 = 25;
	settings.in4 = 35;
	settings.ledPin = 18;
	settings.divisionPin = 17;
	settings.topLED.red = 0;
	settings.topLED.green = 0;
	settings.topLED.blue = 255;
	settings.topLED.brightness = 1.0;
	settings.bottomLED.red = 0;
	settings.bottomLED.green = 255;
	settings.bottomLED.blue = 0;
	settings.bottomLED.brightness = 1.0;

	watch.AdjustDialSettings(DIAL::MINUTES, settings);
}

void setup()
{
	Debug::Print("--------Setup()\n");
	SetupHoursDial();
	SetupMinutesDial();
    watch.Setup();

    myClock.begin(21, 22);
	/*
    const uint8_t isSetup = EEPROM.read(IS_SETUP_CLOCK_ADDR);
	if (isSetup != IS_SETTUP)
	{
		Debug::Print("Setup clock!\n");

		myClock.fillByYMD(2021, 9, 9);//Jan 19,2013
		myClock.fillByHMS(0, 15, 0);//15:28 30"
		myClock.fillDayOfWeek(THU);//Saturday
		myClock.setTime();//write time to the RTC chip

		EEPROM.write(IS_SETUP_CLOCK_ADDR, IS_SETTUP);
    	EEPROM.commit();
	}
	*/
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