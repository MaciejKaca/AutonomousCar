#include "gamepad.h"
#include "common.h"

#include <QDebug>
#include <thread>
#include <chrono>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

#include "steppermotor.h"
#include "RS-232/rs232.h"

Gamepad::Gamepad(StepperMotor * _stepperMotor, SerialPort * _lightsAndServoSerial) : lightsAndServoSerial(_lightsAndServoSerial)
{    
    if (!joystick.isFound())
    {
        qCritical( "Gamepad not found");
        exit(EXIT_BY_MISSING_GAMEPAD);
    }

    isBrakePressed = false;
    isLeftTriggerPressed = false;
    isRightTriggerPressed = false;

    stepperMotor = _stepperMotor;
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

U16 Gamepad::axisToSpeed(S16 axisValue)
{
    if(axisValue > ( MIN_AXIS_VALUE + AXIS_TO_SPEED_SCALE ) && axisValue < 0)
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

S8 Gamepad::axisToDegrees(S16 axisValue)
{
    return  S8( axisValue / AXIS_TO_DEEGRES_SCALE );
}

void Gamepad::readGamepadInput()
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
                buttonState[ ButtonID (event.number) ] = bool (event.value);

                handleInput( ButtonID (event.number) );
            }
            else if (event.isAxis())
            {
                axisState[ AxisID (event.number) ] = event.value;

                handleInput( AxisID (event.number) );
            }
        }
    }
}

bool Gamepad::isButtonPressed(ButtonID buttonID)
{
    return buttonState.find(buttonID)->second;
}

short Gamepad::getAxisValue(AxisID axisID)
{
    return axisState.find(axisID)->second;
}

void Gamepad::handleInput(ButtonID buttonID)
{
    switch (buttonID)
    {
        case EXIT_BUTTON:
            if(isButtonPressed(buttonID) )
            {
                qInfo("Exit Pressed");
                stepperMotor->swithOff();
                exit(EXIT_BY_BUTTON);
            }
            break;

        case X_BUTTON:
            if(isButtonPressed(buttonID) )
            {
                stepperMotor->brake();
            }
            else
            {
                stepperMotor->swithOff();
            }
            break;

        case RIGHT_BUMPER:
            //TODO
            break;

        case LEFT_BUMPER:
            //TODO
            break;

        default:
            qInfo() << "Uknown button :" << buttonID;
            break;
    }
}


void Gamepad::handleInput(AxisID axisID)
{
    switch (axisID)
    {
        case RIGHT_TRIGGER:
            if( ! isButtonPressed(X_BUTTON) )
            {
                    if( getAxisValue(LEFT_TRIGGER) < MIN_AXIS_VALUE + AXIS_TO_SPEED_SCALE)
                    {
                        stepperMotor->move( FORWARD, axisToSpeed( getAxisValue(RIGHT_TRIGGER) ) );
                    }
                    else
                    {
                        stepperMotor->swithOff();
                    }
            }
            else
            {
                stepperMotor->brake();
            }
            break;

        case LEFT_TRIGGER:
            if( ! isButtonPressed(X_BUTTON) )
            {
                    if( getAxisValue(RIGHT_TRIGGER) < MIN_AXIS_VALUE + AXIS_TO_SPEED_SCALE)
                    {
                        stepperMotor->move( BACKWARD, axisToSpeed( getAxisValue(LEFT_TRIGGER) ) );
                    }
                    else
                    {
                        stepperMotor->swithOff();
                    }
            }
            else
            {
                stepperMotor->brake();
            }
            break;

        case LEFT_X_AXIS:
            //TODO
            break;

        default:
            qInfo() << "Uknown axis :" << axisID;
            break;
    }
}
