#ifndef SETTINGSHEADER_H
#define  SETTINGSHEADER_H

// EEPROM
#define IS_SETUP_CLOCK_ADDR 2
#define IS_SETTUP 137

// pins
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

#define ESP32_SDA 21
#define ESP32_SCL 22

// credentials
#define BOT_TOKEN ""
#define SSID ""
#define PASSWORD ""
#define CHAT_ID ""

//commands
#define MV_FRWD_HOURS "/mv_frwd_hours"
#define MV_BKWD_HOURS "/mv_bkwd_hours"
#define MV_FRWD_MIN "/mv_frwd_min"
#define MV_BKWD_MIN "/mv_bkwd_min"
#define MV_FRWD_STEP_HOURS "/mv_frwd_step_hours"
#define MV_FRWD_STEP_MIN "/mv_frwd_step_min"

class RoundWatch;
class DS1307;

void SetupWatch(RoundWatch& watch);
void SetupRTC(DS1307& clock);

#endif