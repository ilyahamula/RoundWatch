#include <Arduino.h>
#include <EEPROM.h>

#include "Settings.h"
#include "RoundWatch.h"

void SetupHoursDial(RoundWatch& watch)
{
	RoundWatch::Settings settings;
	settings.in1 = HOURS_IN1;
	settings.in2 = HOURS_IN2;
	settings.in3 = HOURS_IN3;
	settings.in4 = HOURS_IN4;
	settings.ledPin = HOURS_LED;
	settings.divisionPin = HOURS_DIV;
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
	settings.in1 = MINUTES_IN1;
	settings.in2 = MINUTES_IN2;
	settings.in3 = MINUTES_IN3;
	settings.in4 = MINUTES_IN4;
	settings.ledPin = MINUTES_LED;
	settings.divisionPin = MINUTES_DIV;
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
