#include "gamepad.h"
#include "common.h"

#include<QDebug>
#include <thread>
#include <chrono>
#include <stdlib.h>

Gamepad::Gamepad()
{
    if (!joystick.isFound())
    {
        qCritical( "Gamepad not found");
    }
}

void Gamepad::clearInput()
{
    for(uint8_t i = 0; i <= NUMBER_OF_BUTTONS; i++)
    {
        JoystickEvent event;
        joystick.sample(&event);
    }

    for(uint8_t i = 0; i <= NUMBER_OF_AXIS; i++)
    {
        JoystickEvent event;
        joystick.sample(&event);
    }
}

void Gamepad::handleInput()
{
    clearInput();

    while(true)
    {
        JoystickEvent event;

        std::this_thread::sleep_for(std::chrono::milliseconds(GAMEPAD_REFRESH_TIME));

        if(joystick.sample(&event))
        {
            if(event.isButton())
            {
                switch (event.number)
                {
                    case XBOX_BUTTON:
                        if(event.value == BUTTON_DOWN)
                        {
                            qInfo("XBOX_BUTTON Pressed");
                            exit(EXIT_BY_BUTTON);
                        }
                        break;

                    default:
                        qInfo() << "Uknown button :" << event.number << "value : " << event.value;
                        break;
                }
            }
            else if (event.isAxis())
            {
                switch (event.number)
                {
                    default:
                        qInfo() << "Uknown axis :" << event.number << "value : " << event.value;
                        break;
                }
            }
        }
    }
}


