#ifndef DEBUGHEADER_H
#define  DEBUGHEADER_H

#include <Arduino.h>
#include "Settings.h"
#define DEBUG

#define TEST_HOURS_DIAL Debug::TestForDialSystem(HOURS_IN1, HOURS_IN2, HOURS_IN3, HOURS_IN4, HOURS_LED, HOURS_DIV)
#define TEST_MINUTES_DIAL Debug::TestForDialSystem(MINUTES_IN1, MINUTES_IN2, MINUTES_IN3, MINUTES_IN4, MINUTES_LED, MINUTES_DIV)
#define TEST_HOURS_STEP_MOVING Debug::TestForMottorAndDivisions(DIAL::HOURS, HOURS_IN1, HOURS_IN2, HOURS_IN3, HOURS_IN4, HOURS_DIV)
#define TEST_MINUTES_STEP_MOVING Debug::TestForMottorAndDivisions(DIAL::MINUTES, MINUTES_IN1, MINUTES_IN2, MINUTES_IN3, MINUTES_IN4, MINUTES_DIV)

class DS1307;
enum class DIAL;

class Debug
{
    Debug() = delete;

#ifdef DEBUG
    static bool m_serialSetup;
#endif
public:
    template  <typename T>
    static void Print(T text)
    {
#ifdef DEBUG
        if (!m_serialSetup)
        {
            Serial.begin(9600);
            delay(300);
            m_serialSetup = true;
        }
        Serial.print(text);
#endif
    }

    static int minute;
    static void Print(String& str);
    static void PrintTime(DS1307& clock);
    static void TestForLed(const uint16_t pin);
    static void TestForDialDivider(const uint16_t ledPin, const uint16_t dividerPin);
    static void TestForMottor(const uint8_t in1, const uint8_t in2, const uint8_t in3, const uint8_t in4);
    static void TestForDialSystem(const uint8_t in1, const uint8_t in2, const uint8_t in3, const uint8_t in4, 
        const uint16_t ledPin, const uint16_t dividerPin);
    static void TestForMottorAndDivisions(const DIAL type, const uint8_t in1, const uint8_t in2, const uint8_t in3, const uint8_t in4, 
        const uint16_t dividerPin);
};

#endif