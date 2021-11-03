#ifndef TELEGRAMCMD_H
#define TELEGRAMCMD_H

class WiFiClientSecure;
class UniversalTelegramBot;

class TelegramCMD
{
    TelegramCMD();
    ~TelegramCMD();
    TelegramCMD(const TelegramCMD&) = delete;
    TelegramCMD& operator=(const TelegramCMD&) = delete;

public:
    static TelegramCMD& Instance();
    void Update();

private:
    void HandleNewMessages(int numNewMessages);

private:
    bool isIncorrectTimeMode;
    bool isTimeOffsetMode;
    bool isSetColorMode;

    WiFiClientSecure* m_client;
    UniversalTelegramBot* m_bot;
};

#endif