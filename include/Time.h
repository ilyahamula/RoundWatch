#ifndef TIMEHEADER_H
#define TIMEHEADER_H

struct TimeValues
{
    int8_t hours = -1;
    int8_t minutes = -1;
    int8_t seconds = -1;
};

class TimeManager
{
private:
    TimeManager();
    TimeManager(const TimeManager&) = delete;
    TimeManager& operator=(const TimeManager&) = delete;

public:
    static TimeManager& Instance();
    TimeValues GetTime() const;
    void SetTimeOffset(const int offset);
};

#endif