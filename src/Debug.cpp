#include "Debug.h"
#include "DS1307.h"
#include "TimeLed.h"

#ifdef DEBUG
bool Debug::m_serialSetup = false;
#endif

int Debug::minute = 0;

void Debug::PrintTime(DS1307& clock)
{
#ifdef DEBUG
    if (!m_serialSetup)
    {
        Serial.begin(9600);
        m_serialSetup = true;
    }

	clock.getTime();
	if (clock.minute == minute)
		return;
	
	minute = clock.minute;
	Serial.println("");
	Serial.print(clock.hour);
	Serial.print(":");
	Serial.print(clock.minute, DEC);
	Serial.print(":");
	Serial.print(clock.second, DEC);
	Serial.print("	");
	Serial.print(clock.month, DEC);
	Serial.print("/");
	Serial.print(clock.dayOfMonth, DEC);
	Serial.print("/");
	Serial.print(clock.year+2000, DEC);
	Serial.print(" ");
	Serial.print(clock.dayOfMonth);
	Serial.print("*");
	switch (clock.dayOfWeek)// Friendly printout the weekday
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
#endif
}

void Debug::TestForLed(const uint16_t pin)
{
#ifdef DEBUG
    if (!m_serialSetup)
    {
        Serial.begin(9600);
        m_serialSetup = true;
    }
	Serial.println("Debug::TestForLed()");
	TimeLed led(pin);
	
	led.SetBottomColor(0, 0, 255);
	led.SetBottomBrightness(1.f);
	led.SetTopColor(0, 255, 0);
	led.SetTopBrightness(1.f);
	led.Show();

#endif
}
