#include <inc/common.h>
#include <inc/gamepad.h>

#include <thread>
#include <chrono>
#include <cstdlib>
#include <sys/stat.h>
#include <unistd.h>

#include <inc/steppermotor.h>
#include <RS-232/rs232.h>

Gamepad::Gamepad(StepperMotor *_stepperMotor, Servo *_servo, Lights *_lights):
      AXIS_TO_SPEED_SCALE(static_cast<float>(std::abs(JoystickEvent::MIN_AXES_VALUE)+JoystickEvent::MAX_AXES_VALUE+1)/
                          static_cast<float>(_stepperMotor->MAX_SPEED - StepperMotor::MIN_SPEED)),
      AXIS_TO_ANGLE_SCALE(static_cast<float>(std::abs(JoystickEvent::MIN_AXES_VALUE)+JoystickEvent::MAX_AXES_VALUE+1)/
                          static_cast<float>(std::abs(servo->MIN_ANGLE)+servo->MAX_ANGLE)),
      AXIS_MIN_TRIGGER_POSITION(JoystickEvent::MIN_AXES_VALUE + static_cast<S16>(AXIS_TO_SPEED_SCALE))
{
    qInfo("in Gamepad, initializing constructor");
    joystick = new Joystick();
    if (!joystick->isFound())
    {
        qCritical("in Gamepad::Gamepad, joystick not found");
        throw EXIT_BY_MISSING_GAMEPAD;
    }
    else
    {
        qInfo("in Gamepad::Gamepad, joystick found");
    }

    stepperMotor = _stepperMotor;
    servo = _servo;
    lights = _lights;

    buttonState[ButtonID::X_BUTTON] = false;
    buttonState[ButtonID::EXIT_BUTTON] = false;
    buttonState[ButtonID::LEFT_BUMPER] = false;
    buttonState[ButtonID::RIGHT_BUMPER] = false;

    axisState[AxisID::LEFT_X_AXIS] = 0;
    axisState[AxisID::ARROW_Y_AXIS] = 0;
    axisState[AxisID::LEFT_TRIGGER] = JoystickEvent::MIN_AXES_VALUE;
    axisState[AxisID::RIGHT_TRIGGER] = JoystickEvent::MIN_AXES_VALUE;

    isThreadActive = false;
}

Gamepad::~Gamepad()
{
    qInfo("in Gamepad::~Gamepad, destructor called");
    delete joystick;
}

GamepadBase::~GamepadBase(){}

void Gamepad::startThread()
{
    qInfo("in Gamepad::startThread, starting thread");
    gamepadThread = std::async(std::launch::async, &Gamepad::readGamepadInput, this);
}

void Gamepad::stopThread()
{
    qInfo("in Gamepad::stopThread, terminating thread");
    if(isThreadActive == true)
    {
        isThreadActive = false;
        gamepadThread.get();
    }
}

void Gamepad::waitForExitButton()
{
    qInfo("in Gamepad::waitForExitButton, waiting for exit");
    gamepadThread.get();
}

void Gamepad::clearInput()
{
    qInfo("in Gamepad::clearInput, clearing input");
    for(U8 i = 0; i < NUMBER_OF_BUTTONS*2; i++)
    {
        JoystickEvent event;
        joystick->sample(&event);
    }

    for(U8 i = 0; i < NUMBER_OF_AXIS*2; i++)
    {
        JoystickEvent event;
        joystick->sample(&event);
    }
}

inline bool Gamepad::isGamepadConnected()
{
    struct stat buffer;
    return (stat (gamepadPath.c_str(), &buffer) == 0);
}

U16 Gamepad::axisToSpeed(const S16 &axisValue)
{
    if(axisValue > (JoystickEvent::MIN_AXES_VALUE + AXIS_TO_SPEED_SCALE) && axisValue < 0)
    {
        return StepperMotor::MIN_SPEED+(std::ceil(float(JoystickEvent::MAX_AXES_VALUE - std::abs(axisValue))/AXIS_TO_SPEED_SCALE));
    }
    else if(axisValue >= 0)
    {
        return StepperMotor::MIN_SPEED+(std::ceil(float(JoystickEvent::MAX_AXES_VALUE+axisValue)/ AXIS_TO_SPEED_SCALE));
    }
    else
    {
        return 0;
    }
}

S8 Gamepad::axisToDegrees(const S16 &axisValue)
{
    return  S8(ceil(axisValue/AXIS_TO_ANGLE_SCALE));
}

bool Gamepad::readGamepadInput()
{
    clearInput();
    isThreadActive = true;

    while(isThreadActive)
    {
        if (!isGamepadConnected())
        {
            qCritical( "Gamepad disconnected");
            throw EXIT_BY_MISSING_GAMEPAD;
        }

        JoystickEvent event;

        std::this_thread::sleep_for(std::chrono::milliseconds(GAMEPAD_REFRESH_TIME));

        if(joystick->sample(&event))
        {
            if(event.isButton())
            {
                buttonState[ButtonID(event.number)] = bool(event.value);

                handleInput(ButtonID(event.number));
            }
            else if (event.isAxis())
            {
                axisState[AxisID(event.number)] = event.value;

                handleInput(AxisID(event.number));
            }
        }
    }

    return true;
}

const bool &Gamepad::isButtonPressed(const ButtonID &buttonID)
{
    return buttonState.find(buttonID)->second;
}

const short &Gamepad::getAxisValue(const AxisID &axisID)
{
    return axisState.find(axisID)->second;
}

void Gamepad::handleInput(const ButtonID &buttonID)
{
    switch (buttonID)
    {
        case EXIT_BUTTON:
            if(isButtonPressed(buttonID))
            {
                qInfo("in Gamepad::handleInput, exit pressed");
                isThreadActive = false;
            }
            break;

        case X_BUTTON:
            if(isButtonPressed(X_BUTTON))
            {
                lights->setBrakeLights(BRAKE_LIGHT_STOP);
                stepperMotor->brake();
            }
            else
            {
                lights->setBrakeLights(BRAKE_LIGHT_OFF);
                stepperMotor->switchOff();
            }
            break;

        case RIGHT_BUMPER:
            if(isButtonPressed(RIGHT_BUMPER))
            {
                if(!isButtonPressed(LEFT_BUMPER))
                {
                    if(lights->getTurnSignalStatus() != TURN_SIGNAL_RIGHT && lights->getTurnSignalStatus() != HAZARD_LIGHTS)
                    {
                        lights->setTurnSignal(TURN_SIGNAL_RIGHT);
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
                if(!isButtonPressed(RIGHT_BUMPER))
                {
                    if(lights->getTurnSignalStatus() != TURN_SIGNAL_LEFT && lights->getTurnSignalStatus() != HAZARD_LIGHTS)
                    {
                        lights->setTurnSignal(TURN_SIGNAL_LEFT);
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
            break;
    }
}

void Gamepad::handleInput(const AxisID &axisID)
{
    switch (axisID)
    {
        case RIGHT_TRIGGER:
            if(!isButtonPressed(X_BUTTON))
            {
                    if(getAxisValue(RIGHT_TRIGGER) > AXIS_MIN_TRIGGER_POSITION &&
                            getAxisValue(LEFT_TRIGGER) < AXIS_MIN_TRIGGER_POSITION)
                    {
                        stepperMotor->move(FORWARD, axisToSpeed(getAxisValue(RIGHT_TRIGGER)));
                    }
                    else
                    {
                        if(stepperMotor->getSpeed() != 0)
                        {
                            stepperMotor->switchOff();
                        }
                    }
            }
            break;

        case LEFT_TRIGGER:
            if(!isButtonPressed(X_BUTTON))
            {
                    if(getAxisValue(LEFT_TRIGGER) > AXIS_MIN_TRIGGER_POSITION &&
                            getAxisValue(RIGHT_TRIGGER) < AXIS_MIN_TRIGGER_POSITION)
                    {
                        if(lights->getReverseLightStatus() == REVERSE_LIGHT_OFF)
                        {
                            lights->setReverseLight(REVERSE_LIGHT_ON);
                        }

                        stepperMotor->move(BACKWARD, axisToSpeed( getAxisValue(LEFT_TRIGGER)));
                    }
                    else
                    {
                        if(lights->getReverseLightStatus() == REVERSE_LIGHT_ON)
                        {
                            lights->setReverseLight(REVERSE_LIGHT_OFF);
                        }

                        if(stepperMotor->getSpeed() != 0)
                        {
                            stepperMotor->switchOff();
                        }
                    }
            }
            else
            {
                if(lights->getReverseLightStatus() == REVERSE_LIGHT_ON)
                {
                    lights->setReverseLight(REVERSE_LIGHT_OFF);
                }
            }
            break;

        case LEFT_X_AXIS:
            servo->turn(axisToDegrees(getAxisValue(LEFT_X_AXIS)));
            break;

        case ARROW_Y_AXIS:
            if(getAxisValue(ARROW_Y_AXIS) > 0) //Arrow DOWN
            {
                if(lights->getBrakeLightsWhenOffStatus() != BRAKE_LIGHT_DAYTIME)
                {
                    lights->setBrakeLightsWhenOff(BRAKE_LIGHT_DAYTIME);
                }
                else
                {
                    lights->setBrakeLightsWhenOff(BRAKE_LIGHT_OFF);
                }
            }
            else if(getAxisValue(ARROW_Y_AXIS) < 0) //Arrow UP
            {
                if(lights->getHeadLightStatus() == HEADLIGHT_OFF)
                {
                    lights->setHeadLight(HEADLIGHT_DAYTIME);
                }
                else if(lights->getHeadLightStatus() == HEADLIGHT_DAYTIME)
                {
                    lights->setHeadLight(HEADLIGHT_HIGH_BEAM);
                }
                else if(lights->getHeadLightStatus() == HEADLIGHT_HIGH_BEAM)
                {
                    lights->setHeadLight(HEADLIGHT_OFF);
                }
            }
            break;

        default:
            break;
    }
}
