#include "Debug.h"

bool Debug::m_serialSetup = false;

void Debug::Print(String text)
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