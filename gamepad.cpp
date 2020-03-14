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

Gamepad::Gamepad(StepperMotor *  _stepperMotor, Servo * _servo, Lights * _lights)
{    
    if (!joystick.isFound())
    {
        qCritical( "Gamepad not found");
        exit(EXIT_BY_MISSING_GAMEPAD);
    }

    stepperMotor = _stepperMotor;
    servo = _servo;
    lights = _lights;
}

void Gamepad::clearInput()
{
    for(U8 i = 0; i <= NUMBER_OF_BUTTONS; i++)
    {
        JoystickEvent event;
        joystick.sample(&event);
    }

    for(U8 i = 0; i <= NUMBER_OF_AXIS; i++)
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

U16 Gamepad::axisToSpeed(const S16 axisValue)
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

S8 Gamepad::axisToDegrees(const S16 axisValue)
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

bool Gamepad::isButtonPressed(const ButtonID buttonID)
{
    return buttonState.find(buttonID)->second;
}

short Gamepad::getAxisValue(const AxisID axisID)
{
    return axisState.find(axisID)->second;
}

void Gamepad::handleInput(const ButtonID buttonID)
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
            if(isButtonPressed(X_BUTTON) )
            {
                lights->setBrakeLights(STOP);
                stepperMotor->brake();
            }
            else
            {
                lights->setBrakeLights(BRAKE_LIGHT_OFF);
                stepperMotor->swithOff();
            }
            break;

        case RIGHT_BUMPER:
            if(isButtonPressed(RIGHT_BUMPER))
            {
                if(! isButtonPressed(LEFT_BUMPER))
                {
                    if(lights->getTurnSignalStatus() != RIGHT && lights->getTurnSignalStatus() != HAZARD_LIGHTS)
                    {
                        lights->setTurnSignal(RIGHT);
                    }
                    else
                    {
                        lights->setTurnSignal(TURN_SIGNAL_OFF);
                    }
                }
                else
                {
                    if(lights->getTurnSignalStatus() != HAZARD_LIGHTS)
                    {
                        lights->setTurnSignal(HAZARD_LIGHTS);
                    }
                }
            }
            break;

        case LEFT_BUMPER:
        if(isButtonPressed(LEFT_BUMPER))
        {
            if(! isButtonPressed(RIGHT_BUMPER))
            {
                if(lights->getTurnSignalStatus() != LEFT && lights->getTurnSignalStatus() != HAZARD_LIGHTS)
                {
                    lights->setTurnSignal(LEFT);
                }
                else
                {
                    lights->setTurnSignal(TURN_SIGNAL_OFF);
                }
            }
            else
            {
                if(lights->getTurnSignalStatus() != HAZARD_LIGHTS)
                {
                    lights->setTurnSignal(HAZARD_LIGHTS);
                }
            }
        }
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
                        if( getAxisValue(LEFT_TRIGGER) > MIN_AXIS_VALUE + AXIS_TO_SPEED_SCALE)
                        {
                            lights->setReverseLight(ON);
                        }
                        else
                        {
                            lights->setReverseLight(REVERSE_LIGHT_OFF);
                        }

                        stepperMotor->move( BACKWARD, axisToSpeed( getAxisValue(LEFT_TRIGGER) ) );
                    }
                    else
                    {
                        lights->setReverseLight(REVERSE_LIGHT_OFF);
                        stepperMotor->swithOff();
                    }
            }
            else
            {
                lights->setReverseLight(REVERSE_LIGHT_OFF);
                stepperMotor->brake();
            }
            break;

        case LEFT_X_AXIS:
            servo->turn( axisToDegrees( getAxisValue(LEFT_X_AXIS) ) );
            break;

        case ARROW_Y_AXIS:
            if(getAxisValue(ARROW_Y_AXIS) > 0) //Arrow UP
            {
                if(lights->getBrakeLightsStatus() != BRAKE_LIGHT_DAYTIME)
                {
                    lights->setBrakeLightsWhenOff(BRAKE_LIGHT_DAYTIME);
                }
                else
                {
                    lights->setBrakeLightsWhenOff(BRAKE_LIGHT_OFF);
                }
            }
            else if(getAxisValue(ARROW_Y_AXIS) < 0) //Arrow DOWN
            {
                if(lights->getHeadLightStatus() == HEADLIGHT_OFF)
                {
                    lights->setHeadLight(HEADLIGHT_DAYTIME);
                }
                else if(lights->getHeadLightStatus() == HEADLIGHT_DAYTIME)
                {
                    lights->setHeadLight(HIGH_BEAM);
                }
                else if(lights->getHeadLightStatus() == HIGH_BEAM)
                {
                    lights->setHeadLight(HEADLIGHT_OFF);
                }
            }
            break;

        default:
            qInfo() << "Uknown axis :" << axisID;
            break;
    }
}
