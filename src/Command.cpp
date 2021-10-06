#include <Arduino.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
//#include <BluetoothSerial.h>

#include "Debug.h"
#include "Command.h"
#include "Settings.h"

void SetCommand(const eConcreteCommand command)
{
    Command::Instance().m_currCmd = command;
    Debug::Print("command setted\n");
}

void SetIncorrrectTime(const int hours, const int min)
{
    Command::Instance().m_hours = hours;
    Command::Instance().m_min = min;
    Debug::Print("incorrect time setted\n");
}

namespace
{
    bool ParseTime(const String& text, int& hours, int& min)
    {
        return false;
    }

    void HandleNewMessages(int numNewMessages, UniversalTelegramBot& bot) 
    {
        Debug::Print("HandleNewMessages\n");
        Debug::Print(String(numNewMessages));

        bool isIncorrectTimeMode = false;
        for (int i = 0; i < numNewMessages; i++) 
        {
            // Chat id of the requester
            const String& chat_id = bot.messages[i].chat_id;
            // if (chat_id != CHAT_ID)
            // {
            //     bot.sendMessage(chat_id, "Unauthorized user", "");
            //     continue;
            // }
    
            // Print the received message
            const String& text = bot.messages[i].text;
            Debug::Print(text + "\n");
            if (isIncorrectTimeMode)
            {
                int hours = -1;
                int min = -1;
                if (ParseTime(text, hours, min))
                {
                    SetIncorrrectTime(hours, min);
                    SetCommand(eConcreteCommand::eIncorrectTime);
                    isIncorrectTimeMode = false;
                }
                else
                {
                    String msg = "Incorrect timeformat! Print time in format hh:mm";
                    bot.sendMessage(chat_id, msg, "");
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
                welcome += INCORRECT_TIME;
                welcome += "  to tell watch that time is incorrect \n";
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
            else if (text == INCORRECT_TIME)
            {
                String msg = "print time in format hh:mm";
                bot.sendMessage(chat_id, msg, "");
                isIncorrectTimeMode = true;
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
                    Debug::Print("got response\n");
                    HandleNewMessages(numNewMessages, bot);
                    numNewMessages = bot.getUpdates(bot.last_message_received + 1);
                }
                lastTimeBotRan = millis();
            }
            vTaskDelay(10);
        }
    }

#ifdef DEBUG
    void RunSerial(void* parameters)
    {
        while(true)
        {
            if (Serial.available() > 0)
	        {
	        	const uint8_t byte = Serial.read();
	        	Serial.println(byte);

	        	if (byte == 49) // 1
	        		SetCommand(eConcreteCommand::eMoveForwardHour);
	        	else if (byte == 50) // 2
	        		SetCommand(eConcreteCommand::eMoveBackwardHour);
	        	else if (byte == 51) //3
	        		SetCommand(eConcreteCommand::eMoveForwardMin);
	        	else if (byte == 52) // 4
	        		SetCommand(eConcreteCommand::eMoveBackwardMin);
	        	else if (byte == 53) // 5
	        		SetCommand(eConcreteCommand::eMoveFrwdStepHour);
	        	else if (byte == 54) // 6
	        		SetCommand(eConcreteCommand::eMoveFrwdStepMin);

	        	Serial.flush();
	        }
            vTaskDelay(10);
        }
    }
#endif

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

Command::Command()
    : m_telegramBotTask()
    , m_serialTask()
    , m_bluetoothTask()
    , m_currCmd(eConcreteCommand::eNone)
    , m_hours(-1)
    , m_min(-1)
{
    xTaskCreatePinnedToCore(
                    RunTelegramBot,   /* Task function. */
                    "Telegram Bot",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &m_telegramBotTask,      /* Task handle to keep track of created task */
                    0);          /* pin task to core 0 */                  
#ifdef DEBUG
    xTaskCreatePinnedToCore(
                    RunSerial,   /* Task function. */
                    "Serial port",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &m_serialTask,      /* Task handle to keep track of created task */
                    0);          /* pin task to core 0 */ 
#endif
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

void Command::GetIncorrectTime(int& hours, int& min)
{
    hours = m_hours;
    min = m_min;

    m_hours = -1;
    m_min = -1;
}
