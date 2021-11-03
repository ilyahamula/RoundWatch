#include "TelegramCMD.h"
#include "Settings.h"
#include "Command.h"
#include "Debug.h"

#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

//commands
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
#define SET_TOP_COLOR_HOURS "/set_top_color_hours"
#define SET_BOTTOM_COLOR_HOURS "/set_bottom_color_hours"
#define SET_TOP_COLOR_MIN "/set_top_color_min"
#define SET_BOTTOM_COLOR_MIN "/set_bottom_color_min"

#define INCORRECT_VALUE "Incorrect value"

namespace
{
    bool isNumber(const String& str)
    {
        for (const auto ch : str)
            if (!isDigit(ch))
                return false;
        return true;
    };

    bool ParseTime(const String& text, int8_t& hours, int8_t& min)
    {
        const int pos = text.indexOf(':');
        if (pos == -1)
            return false;

        const String hoursStr = text.substring(0, pos);
        const String minutesStr = text.substring(pos + 1);
        Debug::Print("\nparse hr:" );
        Debug::Print(hoursStr);
        Debug::Print("\nparse min:" );
        Debug::Print(minutesStr);

        if (!isNumber(hoursStr) || !isNumber(minutesStr))
            return false;
        
        int hoursLocal = hoursStr.toInt();
        int minLocal = minutesStr.toInt();

        if (hoursLocal > 23 || hoursLocal < 0 ||
            minLocal > 59 || minLocal < 0)
            return false;
        hours = hoursLocal;
        min = minLocal; 
        return true;
    }

    bool ParseNumber(const String& text, uint32_t& offsetInSec)
    {
        if (!isNumber(text))
            return false;

        offsetInSec = text.toInt();
        return true;
    }

    String CreateWelcomeMessage(const String& userName)
    {
        String welcome = "Welcome, " + userName + ".\n";
        welcome += "Use the following commands to control RoundWatch.\n\n";
        welcome += MV_FRWD_HOURS;
        welcome += "  to move forward hours dial \n";
        welcome += MV_BKWD_HOURS;
        welcome += "  to move backward hours dial \n";
        welcome += MV_FRWD_MIN;
        welcome += "  to move forward minutes dial \n";
        welcome += MV_BKWD_MIN;
        welcome += "  to move backward minutes dial \n";
        welcome += MV_FRWD_STEP_HOURS;
        welcome += "  to move one step forward hours dial \n";
        welcome += MV_FRWD_STEP_MIN;
        welcome += "  to move one step forward minutes dial \n";
        welcome += MV_BKWD_STEP_HOURS;
        welcome += "  to move one step backward hours dial \n";
        welcome += MV_BKWD_STEP_MIN;
        welcome += "  to move one step backward minutes dial \n";
        welcome += INCORRECT_TIME;
        welcome += "  to tell watch that time is incorrect \n";
        welcome += CONNECTION_TEST;
        welcome += " to test the connection \n";
        return welcome;
    }
}

TelegramCMD::TelegramCMD()
    : isIncorrectTimeMode(false)
    , isTimeOffsetMode(false)
    , isSetColorMode(false)
{
    m_client = new WiFiClientSecure;
    m_client->setCertificate(TELEGRAM_CERTIFICATE_ROOT);
    m_bot = new UniversalTelegramBot(BOT_TOKEN, *m_client);
}

TelegramCMD::~TelegramCMD()
{
    delete m_client;
    delete m_bot;
}

TelegramCMD& TelegramCMD::Instance()
{
    static TelegramCMD inst;
    return inst;
}

void TelegramCMD::Update()
{
    int numNewMessages = m_bot->getUpdates(m_bot->last_message_received + 1);
    while(numNewMessages)
    {
        HandleNewMessages(numNewMessages);
        numNewMessages = m_bot->getUpdates(m_bot->last_message_received + 1);
    }
}

void TelegramCMD::HandleNewMessages(int numNewMessages) 
{
    Debug::Print("\nHandleNewMessages ");
    Debug::Print(String(numNewMessages));
    Debug::Print("\n");

    Command& cmdMgr = Command::Instance();
    for (int i = 0; i < numNewMessages; i++) 
    {
        // Chat id of the requester
        const String& chat_id = m_bot->messages[i].chat_id;
        if (chat_id != CHAT_ID)
        {
            m_bot->sendMessage(chat_id, "Unauthorized user");
            continue;
        }
        // Print the received message
        const String& text = m_bot->messages[i].text;
        Debug::Print(text + "\n");
        if (isIncorrectTimeMode)
        {
            Debug::Print("\nIncorrect time flag\n");
            int8_t hours = -1;
            int8_t min = -1;
            if (ParseTime(text, hours, min))
            {
                cmdMgr.SetIncorrrectTime(hours, min);
                cmdMgr.SetCommand(eConcreteCommand::eIncorrectTime);
                isIncorrectTimeMode = false;
                m_bot->sendMessage(chat_id, "Done!");
            }
            else
                m_bot->sendMessage(chat_id, "Incorrect time format! Print time in format hh:mm");
        }
        else if (isTimeOffsetMode)
        {
            Debug::Print("\nTime offset flag\n");
            uint32_t offsetInSec = 0;
            if (ParseNumber(text, offsetInSec))
            {
                cmdMgr.SetTimeOffset(offsetInSec);
                cmdMgr.SetCommand(eConcreteCommand::eTimeOffset);
                isTimeOffsetMode = false;
                m_bot->sendMessage(chat_id, "Done!");
            }
            else
                m_bot->sendMessage(chat_id, INCORRECT_VALUE);
        }
        else if (text == "/start")
            m_bot->sendMessage(chat_id, CreateWelcomeMessage(m_bot->messages[i].from_name));
        else if (text == MV_FRWD_HOURS)
    		cmdMgr.SetCommand(eConcreteCommand::eMoveForwardHour);
    	else if (text == MV_BKWD_HOURS)
    		cmdMgr.SetCommand(eConcreteCommand::eMoveBackwardHour);
    	else if (text == MV_FRWD_MIN)
    	    cmdMgr.SetCommand(eConcreteCommand::eMoveForwardMin);
    	else if (text == MV_BKWD_MIN)
    		cmdMgr.SetCommand(eConcreteCommand::eMoveBackwardMin);
    	else if (text == MV_FRWD_STEP_HOURS)
    		cmdMgr.SetCommand(eConcreteCommand::eMoveFrwdStepHour);
    	else if (text == MV_FRWD_STEP_MIN)
    		cmdMgr.SetCommand(eConcreteCommand::eMoveFrwdStepMin);
        else if (text == MV_BKWD_STEP_HOURS)
    		cmdMgr.SetCommand(eConcreteCommand::eMoveBackwardStepHour);
    	else if (text == MV_BKWD_STEP_MIN)
    		cmdMgr.SetCommand(eConcreteCommand::eMoveBackwardStepMin);
        else if (text == INCORRECT_TIME)
        {
            m_bot->sendMessage(chat_id, "print time in format hh:mm");
            isIncorrectTimeMode = true;
        }
        else if (text == TIME_OFFSET)
        {
            m_bot->sendMessage(chat_id, "print UTC time offset in seconds\n (Example: UTC+3 = 10800 sec)");
            isTimeOffsetMode = true;
        }
        else if (text == CONNECTION_TEST)
            m_bot->sendMessage(chat_id, "Connection is tested");
    }
}
