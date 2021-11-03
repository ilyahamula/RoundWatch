#include <Arduino.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
//#include <BluetoothSerial.h>

#include "Debug.h"
#include "Command.h"
#include "TelegramCMD.h"

namespace
{
    
    void RunTelegramBot(void* parametrs)
    {
        int botRequestDelay = 1000;
        unsigned long lastTimeBotRan = 0;
        while (true)
        {
            if (millis() > lastTimeBotRan + botRequestDelay)  
            {
                TelegramCMD::Instance().Update();
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

void Command::SetCommand(const eConcreteCommand command)
{
    m_currCmd = command;
    Debug::Print("command setted\n");
}

void Command::SetIncorrrectTime(const int8_t hours, const int8_t min)
{
    m_hours = hours;
    m_min = min;
    Debug::Print("\nincorrect time setted\n");
}

void Command::SetTimeOffset(const uint32_t offset)
{
    m_offset = offset;
    Debug::Print("\ntime offset setted\n");
}
