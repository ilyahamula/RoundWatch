#ifndef DEBUGHEADER_H
#define  DEBUGHEADER_H

#include <Arduino.h>

#define DEBUG

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
            delay(1000);
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