#include "Debug.h"
#include "TimeLed.h"
#include "Stepper_28BYJ_48.h"
#include "RoundDial.h"

namespace
{
	void DialDividerAndLedTest(const uint16_t dividerPin, TimeLed& led)
	{
		if (digitalRead(dividerPin))
		{
			Serial.println("ON---");
			led.Show();
		}
		else
		{
			Serial.println("OFF");
			led.Off();
		}
	}
}

#ifdef DEBUG
bool Debug::m_serialSetup = false;
#endif

int Debug::minute = 0;

void Debug::Print(String& str)
{
#ifdef DEBUG
    if (!m_serialSetup)
    {
        Serial.begin(9600);
		delay(1000);
        m_serialSetup = true;
    }
	Serial.print(str);
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

void Debug::TestForDialDivider(const uint16_t ledPin, const uint16_t dividerPin)
{
#ifdef DEBUG
    if (!m_serialSetup)
    {
        Serial.begin(9600);
        m_serialSetup = true;
    }
	Serial.println("Debug::Test For Dial");

	TimeLed led(ledPin);
	led.SetBottomColor(0, 0, 255);
	led.SetBottomBrightness(1.f);
	led.SetTopColor(0, 255, 0);
	led.SetTopBrightness(1.f);

	pinMode(dividerPin, INPUT);

	while(true)
	{
		DialDividerAndLedTest(dividerPin, led);
	}

#endif
}

void Debug::TestForMottor(const uint8_t in1, const uint8_t in2, const uint8_t in3, const uint8_t in4)
{
#ifdef DEBUG
    if (!m_serialSetup)
    {
        Serial.begin(9600);
        m_serialSetup = true;
    }
	Serial.println("Debug::Test For Mottor");

	Stepper_28BYJ_48 m_stepperMotor(in1, in2, in3, in4);

	while(true)
	{
		m_stepperMotor.step(10);
	}
#endif
}

void Debug::TestForDialSystem(const uint8_t in1, const uint8_t in2, const uint8_t in3, const uint8_t in4, 
    const uint16_t ledPin, const uint16_t dividerPin)
{
#ifdef DEBUG
    if (!m_serialSetup)
    {
        Serial.begin(9600);
        m_serialSetup = true;
    }
	Serial.println("Debug::Test Dial System");

	Stepper_28BYJ_48 m_stepperMotor(in1, in2, in3, in4);
	TimeLed led(ledPin);
	led.SetBottomColor(0, 0, 255);
	led.SetBottomBrightness(1.f);
	led.SetTopColor(0, 255, 0);
	led.SetTopBrightness(1.f);

	while(true)
	{
		DialDividerAndLedTest(dividerPin, led);
		m_stepperMotor.step(10);
	}
#endif
}

void Debug::TestForMottorAndDivisions(const DIAL type, const uint8_t in1, const uint8_t in2, const uint8_t in3, const uint8_t in4, 
    const uint16_t dividerPin)
{
#ifdef DEBUG
    if (!m_serialSetup)
    {
        Serial.begin(9600);
        m_serialSetup = true;
    }
	Serial.println("Debug::Test Mottor and Divisions");

	RoundDial* testDial = RoundDial::CreateDial(type, in1, in2, in3, in4);
	testDial->SetDivisionsPin(dividerPin);

	while(true)
	{
		Serial.println("-STEP-");
		testDial->MoveStep();
		delay(2000);
	}
#endif
}

void Debug::PrintTime(const int8_t hours, const int8_t min)
{
#ifdef DEBUG
    if (!m_serialSetup)
    {
        Serial.begin(9600);
        m_serialSetup = true;
    }

	if (min != minute)
	{
		minute = min;
		Serial.print("\n");
		Serial.print(hours);
		Serial.print(" : ");
		Serial.println(min);
	}
#endif
}

