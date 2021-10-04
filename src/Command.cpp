#include <Arduino.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

#include "Debug.h"
#include "Command.h"
#include "Settings.h"

void SetCommand(const eConcreteCommand command)
{
    Command::Instance().m_currCmd = command;
}

namespace
{
    void HandleNewMessages(int numNewMessages, UniversalTelegramBot& bot) 
    {
        Debug::Print("HandleNewMessages\n");
        Debug::Print(String(numNewMessages));

        for (int i = 0; i < numNewMessages; i++) 
        {
            // Chat id of the requester
            String chat_id = String(bot.messages[i].chat_id);
            // if (chat_id != CHAT_ID)
            // {
            //     bot.sendMessage(chat_id, "Unauthorized user", "");
            //     continue;
            // }
    
            // Print the received message
            String text = bot.messages[i].text;
            Debug::Print(text);

            String from_name = bot.messages[i].from_name;

            if (text == "/start") 
            {
                String welcome = "Welcome, " + from_name + ".\n";
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
                bot.sendMessage(chat_id, welcome, "");
            }

            if (text == MV_FRWD_HOURS)
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
        }
    }

    void RunSerial(void* parameters)
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
    }
}

Command::Command()
    : telegramBotTask()
    , serialTask()
    , m_currCmd(eConcreteCommand::eNone)
{
    xTaskCreatePinnedToCore(
                    RunTelegramBot,   /* Task function. */
                    "Telegram Bot",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &telegramBotTask,      /* Task handle to keep track of created task */
                    0);          /* pin task to core 0 */                  
    delay(500); 
    xTaskCreatePinnedToCore(
                    RunSerial,   /* Task function. */
                    "Serial port",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &serialTask,      /* Task handle to keep track of created task */
                    0);          /* pin task to core 0 */ 
    delay(500);
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