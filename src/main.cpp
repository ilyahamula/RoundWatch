#include <Arduino.h>
#include <Wire.h>
#include <EEPROM.h>
#include "DS1307.h"
#include "RoundWatch.h"

#define IS_SETUP_CLOCK_ADDR 2
#define IS_SETTUP 137

RoundWatch watch; 
DS1307 myClock;//define a object of DS1307 class
int minute = 0;

void printTime()
{
	myClock.getTime();
	if (myClock.minute == minute)
		return;
	
	minute = myClock.minute;
	Serial.println("");
	Serial.print(myClock.hour);
	Serial.print(":");
	Serial.print(myClock.minute, DEC);
	Serial.print(":");
	Serial.print(myClock.second, DEC);
	Serial.print("	");
	Serial.print(myClock.month, DEC);
	Serial.print("/");
	Serial.print(myClock.dayOfMonth, DEC);
	Serial.print("/");
	Serial.print(myClock.year+2000, DEC);
	Serial.print(" ");
	Serial.print(myClock.dayOfMonth);
	Serial.print("*");
	switch (myClock.dayOfWeek)// Friendly printout the weekday
	{
		case MON:
		  Serial.print("MON");
		  break;
		case TUE:
		  Serial.print("TUE");
		  break;
		case WED:
		  Serial.print("WED");
		  break;
		case THU:
		  Serial.print("THU");
		  break;
		case FRI:
		  Serial.print("FRI");
		  break;
		case SAT:
		  Serial.print("SAT");
		  break;
		case SUN:
		  Serial.print("SUN");
		  break;
	}
	Serial.println(" ");
}

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
	/*
    const uint8_t isSetup = EEPROM.read(IS_SETUP_CLOCK_ADDR);
	//if (isSetup != IS_SETTUP)
	{
		myClock.fillByYMD(2020,10,17);//Jan 19,2013
		myClock.fillByHMS(17,11,00);//15:28 30"
		myClock.fillDayOfWeek(SAT);//Saturday
		myClock.setTime();//write time to the RTC chip

		//EEPROM.write(IS_SETUP_CLOCK_ADDR, IS_SETTUP);
    	//EEPROM.commit();
	}
	*/
		myClock.fillByYMD(2020,10,17);//Jan 19,2013
		myClock.fillByHMS(18,23,00);//15:28 30"
		myClock.fillDayOfWeek(SAT);//Saturday
		myClock.setTime();//write time to the RTC chip
}

int cnt = 0;

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

    printTime();
	myClock.getTime();
	watch.SetRealTime(myClock.hour, myClock.minute);
}