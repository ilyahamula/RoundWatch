#ifndef DEBUGHEADER_H
#define  DEBUGHEADER_H

#include <Arduino.h>

#define DEBUG

class DS1307;

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
            m_serialSetup = true;
        }
        Serial.print(text);
#endif
    }

    static int minute;
    static void PrintTime(DS1307& clock);
    static void TestForLed(const uint16_t pin);
};

#endif