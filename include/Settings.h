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

#define ESP32_SDA 21
#define ESP32_SCL 22

// credentials
#define BOT_TOKEN ""
#define SSID_DEF ""
#define PASSWORD ""
#define CHAT_ID ""

class RoundWatch;

void SetupWatch(RoundWatch& watch);

#endif