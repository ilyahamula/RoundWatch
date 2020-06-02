#ifndef DEBUGHEADER_H
#define  DEBUGHEADER_H

#include <Arduino.h>

#define DEBUG

class Debug
{
    Debug() = delete;

#ifdef DEBUG
    static bool m_serialSetup;
#endif
public:
    static void Print(String text);
};

#endif