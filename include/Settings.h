#ifndef SETTINGSHEADER_H
#define  SETTINGSHEADER_H

#define IS_SETUP_CLOCK_ADDR 2
#define IS_SETTUP 137

#define HOURS_IN1 12
#define HOURS_IN2 13
#define HOURS_IN3 14
#define HOURS_IN4 26
#define HOURS_LED 5
#define HOURS_DIV 16

#define MINUTES_IN1 27
#define MINUTES_IN2 33
#define MINUTES_IN3 25
#define MINUTES_IN4 35
#define MINUTES_LED 18
#define MINUTES_DIV 17

#define BOT_TOKEN ""
#define SSID ""
#define PASSWORD ""

class RoundWatch;
class DS1307;

void SetupHoursDial(RoundWatch& watch);
void SetupMinutesDial(RoundWatch& watch);
void SetupRTC(DS1307& myClock);

#endif