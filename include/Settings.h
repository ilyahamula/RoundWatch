#ifndef SETTINGSHEADER_H
#define SETTINGSHEADER_H

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

#define LIGHT_SENSOR_PIN 34

// credentials
#define BOT_TOKEN ""
#define SSID_DEF "" 
#define PASSWORD ""
#define CHAT_ID ""

//commands
#define HELP "/help"
#define MV_FRWD_HOURS "/mv_frwd_hours"
#define MV_BKWD_HOURS "/mv_bkwd_hours"
#define MV_FRWD_MIN "/mv_frwd_min"
#define MV_BKWD_MIN "/mv_bkwd_min"
#define MV_FRWD_STEP_HOURS "/mv_frwd_step_hours"
#define MV_FRWD_STEP_MIN "/mv_frwd_step_min"
#define MV_BKWD_STEP_HOURS "/mv_bkwd_step_hours"
#define MV_BKWD_STEP_MIN "/mv_bkwd_step_min"
#define INCORRECT_TIME "/calibrate_by_incorrect_time"
#define TIME_OFFSET "/time_offset"
#define CONNECTION_TEST "/connection_test"

class RoundWatch;

void SetupWatch(RoundWatch& watch);

#endif