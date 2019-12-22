#include "gamepad.h"
#include "common.h"

#include <QDebug>
#include <thread>
#include <chrono>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

#include "steppermotor.h"

Gamepad::Gamepad()
{    
    if (!joystick.isFound())
    {
        qCritical( "Gamepad not found");
        exit(EXIT_BY_MISSING_GAMEPAD);
    }

    isBrakePressed = false;
    isLeftTriggerPressed = false;
    isRightTriggerPressed = false;
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

inline bool Gamepad::isGamepadConnected()
{
    struct stat buffer;
    return (stat (gamepadPath.c_str(), &buffer) == 0);
}

uint16_t Gamepad::axisToSpeed(int16_t axisValue)
{
    if(axisValue > MIN_AXIS_VALUE && axisValue < 0)
    {
        return MIN_SPEED + ( ceil( float( MAX_AXIS_VALUE - abs(axisValue) ) / AXIS_TO_SPEED_SCALE ) );
    }
    else if(axisValue >= 0)
    {
        return MIN_SPEED + ( ceil( float( MAX_AXIS_VALUE + axisValue) / AXIS_TO_SPEED_SCALE ) );
    }
    else
    {
        return 0;
    }
}

void Gamepad::handleInput()
{
    clearInput();

    while(true)
    {
        if (!isGamepadConnected())
        {
            qCritical( "Gamepad disconnected");
            exit(EXIT_BY_MISSING_GAMEPAD);
        }

        JoystickEvent event;

        std::this_thread::sleep_for(std::chrono::milliseconds(GAMEPAD_REFRESH_TIME));

        if(joystick.sample(&event))
        {
            if(event.isButton())
            {
                switch (event.number)
                {
                    case EXIT_BUTTON:
                        if(event.value == BUTTON_DOWN)
                        {
                            qInfo("Exit Pressed");
                            exit(EXIT_BY_BUTTON);
                        }
                        break;

                    case X_BUTTON:
                        if(event.value == BUTTON_DOWN)
                        {
                            stepperMotor.brake();
                            isBrakePressed = true;
                        }
                        else
                        {
                            stepperMotor.swithOff();
                            isBrakePressed = false;
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
                    case RIGHT_TRIGGER:
                        if(isBrakePressed == false)
                        {
                            if(event.value != MIN_AXIS_VALUE && isLeftTriggerPressed == false)
                            {
                                stepperMotor.move( FORWARD, axisToSpeed( event.value ) );
                                isRightTriggerPressed = true;
                            }
                            else
                            {
                                stepperMotor.swithOff();
                                isRightTriggerPressed = false;
                            }
                        }
                        break;

                case LEFT_TRIGGER:
                    if(isBrakePressed == false)
                    {
                        if(event.value != MIN_AXIS_VALUE && isRightTriggerPressed == false)
                        {
                            stepperMotor.move( BACKWARD, axisToSpeed( event.value ) );
                            isLeftTriggerPressed = true;
                        }
                        else
                        {
                            stepperMotor.swithOff();
                            isLeftTriggerPressed = false;
                        }
                    }
                    break;

                    default:
                        qInfo() << "Uknown axis :" << event.number << "value : " << event.value;
                        break;
                }
            }
        }
    }
}


