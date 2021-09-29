#include <Arduino.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

#include "Debug.h"
#include "Command.h"

namespace
{
#define BOT_TOKEN "2001676253:AAEOBv89TyzE1NGuyS5RD-Dgz3Z6ozrc1eI"

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

            if (text == "/start") {
              String welcome = "Welcome, " + from_name + ".\n";
              welcome += "Use the following commands to control your outputs.\n\n";
              welcome += "/led_on to turn GPIO ON \n";
              welcome += "/led_off to turn GPIO OFF \n";
              welcome += "/state to request current GPIO state \n";
              bot.sendMessage(chat_id, welcome, "");
            }

            if (text == "/led_on")
            {
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
                    Debug::Print("got response");
                    HandleNewMessages(numNewMessages, bot);
                    numNewMessages = bot.getUpdates(bot.last_message_received + 1);
                }
                lastTimeBotRan = millis();
            }
        }
    }
}

Command::Command()
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
}

Command& Command::Instance()
{
    static Command inst;
    return inst;
}

const eConcreteCommand Command::GetCommand()
{
    return eConcreteCommand::eNone;
    // TODO: ask Serial and WEBServer for available input and procced them
}