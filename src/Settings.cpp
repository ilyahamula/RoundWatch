#include <Arduino.h>
#include <EEPROM.h>

#include "Settings.h"
#include "RoundWatch.h"

void SetupHoursDial(RoundWatch& watch)
{
	RoundWatch::Settings settings;
#ifdef USE_STEPPER_28BYJ_48
	settings.driver.type = Driver::eType::eStepper_28BYJ_48;
	settings.driver.in1 = HOURS_IN1;
	settings.driver.in2 = HOURS_IN2;
	settings.driver.in3 = HOURS_IN3;
	settings.driver.in4 = HOURS_IN4;
#else
#ifdef USE_DC_MOTTOR
	settings.driver.type = Driver::eType::eDC_Motor;
	settings.driver.dcPin = HOURS_IN1;
#endif
#endif
	settings.driver.divisionPin = HOURS_DIV;

	settings.ledPin = HOURS_LED;
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

void SetupMinutesDial(RoundWatch& watch)
{
	RoundWatch::Settings settings;
#ifdef USE_STEPPER_28BYJ_48
	settings.driver.type = Driver::eType::eStepper_28BYJ_48;
	settings.driver.in1 = MINUTES_IN1;
	settings.driver.in2 = MINUTES_IN2;
	settings.driver.in3 = MINUTES_IN3;
	settings.driver.in4 = MINUTES_IN4;
#else
#ifdef USE_DC_MOTTOR
	settings.driver.type = Driver::eType::eDC_Motor;
	settings.driver.dcPin = MINUTES_IN1;
#endif
#endif
	settings.driver.divisionPin = MINUTES_DIV;

	settings.ledPin = MINUTES_LED;
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

void SetupWatch(RoundWatch& watch)
{
    SetupHoursDial(watch);
	SetupMinutesDial(watch);
    watch.Setup();
}
