#include <Arduino.h>
#include <WiFiUdp.h>
#include <NTPClient.h>

#include "Debug.h"
#include "Time.h"

const long utcOffsetInSeconds = 10800;
static WiFiUDP ntpUDP;
static NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

TimeManager::TimeManager()
{
    timeClient.begin();
}

TimeManager& TimeManager::Instance()
{
    static TimeManager timeMgr;
    return timeMgr;
}

TimeValues TimeManager::GetTime() const
{
    TimeValues result;
    if (timeClient.update())
    {
        result.hours = timeClient.getHours();
        result.minutes = timeClient.getMinutes();
        result.seconds = timeClient.getSeconds();
    }
    else
    {
        // TODO: notify if we get errors
    }
    return result;    
}

void TimeManager::SetTimeOffset(const int offset)
{
    timeClient.setTimeOffset(offset);
}
