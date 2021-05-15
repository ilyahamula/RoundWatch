#include <Arduino.h>
#include <Wire.h>
#include <EEPROM.h>
#include "DS1307.h"

#include "Debug.h"
#include "RoundWatch.h"

#define IS_SETUP_CLOCK_ADDR 2
#define IS_SETTUP 137

RoundWatch watch; 
DS1307 myClock;//define a object of DS1307 class

void setup()
{
    watch.SetMottorPins(DIAL::HOURS, 12, 13, 14, 26);
    watch.SetLedPin(DIAL::HOURS, 5);
    watch.SetBottomBrightness(DIAL::HOURS, 1.0);
    watch.SetBottomColor(DIAL::HOURS, 0, 0, 255);
    watch.SetTopBrightness(DIAL::HOURS, 1.0);
    watch.SetTopColor(DIAL::HOURS, 0, 0, 255);

	watch.SetMottorPins(DIAL::MINUTES, 27, 33, 25, 35);
    watch.SetLedPin(DIAL::MINUTES, 18);
    watch.SetBottomBrightness(DIAL::MINUTES, 1.0);
    watch.SetBottomColor(DIAL::MINUTES, 0, 255, 0);
    watch.SetTopBrightness(DIAL::MINUTES, 1.0);
    watch.SetTopColor(DIAL::MINUTES, 255, 0, 0);

    watch.Setup();

    myClock.begin(21, 22);
    const uint8_t isSetup = EEPROM.read(IS_SETUP_CLOCK_ADDR);
	if (isSetup != IS_SETTUP)
	{
		myClock.fillByYMD(2020,10,17);//Jan 19,2013
		myClock.fillByHMS(17,11,00);//15:28 30"
		myClock.fillDayOfWeek(SAT);//Saturday
		myClock.setTime();//write time to the RTC chip

		EEPROM.write(IS_SETUP_CLOCK_ADDR, IS_SETTUP);
    	EEPROM.commit();
	}
	/*
	myClock.fillByYMD(2020,10,17);//Jan 19,2013
	myClock.fillByHMS(18,23,00);//15:28 30"
	myClock.fillDayOfWeek(SAT);//Saturday
	myClock.setTime();//write time to the RTC chip
	*/
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