#include <Arduino.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
//#include <BluetoothSerial.h>

#include "Debug.h"
#include "Command.h"
#include "Settings.h"

// Friend functions
void SetCommand(const eConcreteCommand command)
{
    Command::Instance().m_currCmd = command;
    Debug::Print("command setted\n");
}

void SetIncorrrectTime(const int8_t hours, const int8_t min)
{
    Command::Instance().m_hours = hours;
    Command::Instance().m_min = min;
    Debug::Print("\nincorrect time setted\n");
}

void SetTimeOffset(const uint32_t offset)
{
    Command::Instance().m_offset = offset;
    Debug::Print("\ntime offset setted\n");
}

namespace
{
    namespace FlagManager
    {
        static bool isIncorrectTimeMode = false;
        static bool isTimeOffsetMode = false;
    };

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

    void HandleNewMessages(int numNewMessages, UniversalTelegramBot& bot) 
    {
        Debug::Print("\nHandleNewMessages ");
        Debug::Print(String(numNewMessages));
        Debug::Print("\n");

        for (int i = 0; i < numNewMessages; i++) 
        {
            // Chat id of the requester
            const String& chat_id = bot.messages[i].chat_id;
            if (chat_id != CHAT_ID)
            {
                bot.sendMessage(chat_id, "Unauthorized user");
                continue;
            }

            // Print the received message
            const String& text = bot.messages[i].text;
            Debug::Print(text + "\n");
            if (FlagManager::isIncorrectTimeMode)
            {
                Debug::Print("\nIncorrect time flag\n");

                int8_t hours = -1;
                int8_t min = -1;
                if (ParseTime(text, hours, min))
                {
                    SetIncorrrectTime(hours, min);
                    SetCommand(eConcreteCommand::eIncorrectTime);
                    FlagManager::isIncorrectTimeMode = false;

                    String msg = "Done!";
                    bot.sendMessage(chat_id, msg);
                }
                else
                {
                    String msg = "Incorrect time format! Print time in format hh:mm";
                    bot.sendMessage(chat_id, msg);
                }
            }
            else if (FlagManager::isTimeOffsetMode)
            {
                Debug::Print("\nTime offset flag\n");
                uint32_t offsetInSec = 0;
                if (ParseNumber(text, offsetInSec))
                {
                    SetTimeOffset(offsetInSec);
                    SetCommand(eConcreteCommand::eTimeOffset);
                    FlagManager::isTimeOffsetMode = false;

                    String msg = "Done!";
                    bot.sendMessage(chat_id, msg);
                }
                else
                {
                    String msg = "Incorrect value";
                    bot.sendMessage(chat_id, msg);
                }
            }
            else if (text == "/start")
            {
                String welcome = "Welcome, " + bot.messages[i].from_name + ".\n";
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
                bot.sendMessage(chat_id, welcome, "");
            }
            else if (text == MV_FRWD_HOURS)
	    		SetCommand(eConcreteCommand::eMoveForwardHour);
	    	else if (text == MV_BKWD_HOURS)
	    		SetCommand(eConcreteCommand::eMoveBackwardHour);
	    	else if (text == MV_FRWD_MIN)
	    		SetCommand(eConcreteCommand::eMoveForwardMin);
	    	else if (text == MV_BKWD_MIN)
	    		SetCommand(eConcreteCommand::eMoveBackwardMin);
	    	else if (text == MV_FRWD_STEP_HOURS)
	    		SetCommand(eConcreteCommand::eMoveFrwdStepHour);
	    	else if (text == MV_FRWD_STEP_MIN)
	    		SetCommand(eConcreteCommand::eMoveFrwdStepMin);
            else if (text == MV_BKWD_STEP_HOURS)
	    		SetCommand(eConcreteCommand::eMoveBackwardStepHour);
	    	else if (text == MV_BKWD_STEP_MIN)
	    		SetCommand(eConcreteCommand::eMoveBackwardStepMin);
            else if (text == INCORRECT_TIME)
            {
                String msg = "print time in format hh:mm";
                bot.sendMessage(chat_id, msg, "");
                FlagManager::isIncorrectTimeMode = true;
            }
            else if (text == TIME_OFFSET)
            {
                String msg = "print UTC time offset in seconds\n (Example: UTC+3 = 10800 sec)\n";
                bot.sendMessage(chat_id, msg, "");
                FlagManager::isTimeOffsetMode = true;
            }
            else if (text == CONNECTION_TEST)
            {
                String msg = "Connection is tested";
                bot.sendMessage(chat_id, msg, "");
            }
        }
    }

    void RunTelegramBot(void* parametrs)
    {
        WiFiClientSecure client;
        client.setCACert(TELEGRAM_CERTIFICATE_ROOT);
        UniversalTelegramBot bot(BOT_TOKEN, client);

        int botRequestDelay = 1000;
        unsigned long lastTimeBotRan = 0;

        while (true)
        {
            if (millis() > lastTimeBotRan + botRequestDelay)  
            {
                int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
                while(numNewMessages) 
                {
                    HandleNewMessages(numNewMessages, bot);
                    numNewMessages = bot.getUpdates(bot.last_message_received + 1);
                }
                lastTimeBotRan = millis();
            }
            vTaskDelay(10);
        }
    }

    // void RunBluetooth(void* parameters)
    // {
    //     BluetoothSerial SerialBT;
    //     if (!SerialBT.begin("RoundWatch"))
    //         Debug::Print("Bluetooth is not working!");

    //     while(true)
    //     {
    //         if (SerialBT.available() > 0)
	//         {
	//         	const uint8_t byte = SerialBT.read();
	//         	Serial.println(byte);

	//         	if (byte == 49) // 1
	//         		SetCommand(eConcreteCommand::eMoveForwardHour);
	//         	else if (byte == 50) // 2
	//         		SetCommand(eConcreteCommand::eMoveBackwardHour);
	//         	else if (byte == 51) //3
	//         		SetCommand(eConcreteCommand::eMoveForwardMin);
	//         	else if (byte == 52) // 4
	//         		SetCommand(eConcreteCommand::eMoveBackwardMin);
	//         	else if (byte == 53) // 5
	//         		SetCommand(eConcreteCommand::eMoveFrwdStepHour);
	//         	else if (byte == 54) // 6
	//         		SetCommand(eConcreteCommand::eMoveFrwdStepMin);

	//         	Serial.flush();
	//         }
    //         vTaskDelay(10);
    //     }
    // }
}

// class Command
Command::Command()
    : m_telegramBotTask()
    , m_serialTask()
    , m_bluetoothTask()
    , m_currCmd(eConcreteCommand::eNone)
    , m_hours(-1)
    , m_min(-1)
    , m_offset(0)
{
    xTaskCreatePinnedToCore(
                    RunTelegramBot,   /* Task function. */
                    "Telegram Bot",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &m_telegramBotTask,      /* Task handle to keep track of created task */
                    0);          /* pin task to core 0 */                  

    // xTaskCreatePinnedToCore(
    //                 RunBluetooth,   /* Task function. */
    //                 "Bluetooth",     /* name of task. */
    //                 10000,       /* Stack size of task */
    //                 NULL,        /* parameter of the task */
    //                 1,           /* priority of the task */
    //                 &m_bluetoothTask,      /* Task handle to keep track of created task */
    //                 0);          /* pin task to core 0 */ 
}

Command& Command::Instance()
{
    static Command inst;
    return inst;
}

const eConcreteCommand Command::GetCommand()
{
    eConcreteCommand tempCmd = m_currCmd;
    m_currCmd = eConcreteCommand::eNone;
    return tempCmd;
}

void Command::GetIncorrectTime(int8_t& hours, int8_t& min)
{
    hours = m_hours;
    min = m_min;

    m_hours = -1;
    m_min = -1;
}

uint32_t Command::GetTimeOffset()
{
    uint32_t tempOffset = m_offset;
    m_offset = 0;
    return tempOffset;
}
