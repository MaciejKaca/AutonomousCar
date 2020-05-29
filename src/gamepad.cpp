#include <inc/common.h>
#include <inc/gamepad.h>

#include <thread>
#include <chrono>
#include <cstdlib>
#include <sys/stat.h>
#include <unistd.h>
#include <vector>

#include <inc/steppermotor.h>
#include <RS-232/rs232.h>

Gamepad::Gamepad(StepperMotorShell *_stepperMotor, Servo *_servo, Lights *_lights):
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

    buttonState[ButtonID::X_BUTTON] = BUTTON_UP;
    buttonState[ButtonID::EXIT_BUTTON] = BUTTON_UP;
    buttonState[ButtonID::LEFT_BUMPER] = BUTTON_UP;
    buttonState[ButtonID::RIGHT_BUMPER] = BUTTON_UP;

    axisState[AxisID::LEFT_X_AXIS] = 0;
    axisState[AxisID::ARROW_Y_AXIS] = 0;
    axisState[AxisID::LEFT_TRIGGER] = JoystickEvent::MIN_AXES_VALUE;
    axisState[AxisID::RIGHT_TRIGGER] = JoystickEvent::MIN_AXES_VALUE;

    isReadGamepadInputThreadActive = false;
    isHandleAxisInputsThreadActive = false;
    isHandleButtonInputsThreadActive = false;

    mustAxisReturnToZero.fill(false);
}

Gamepad::~Gamepad()
{
    qInfo("in Gamepad::~Gamepad, destructor called");
    delete joystick;
}

void Gamepad::startThread()
{
    qInfo("in Gamepad::startThread, starting thread");
    handleButtonInputsThread = std::async(std::launch::async, &Gamepad::handleButtonInput, this);
    handleAxisInputsThread = std::async(std::launch::async, &Gamepad::handleAxisInput, this);
    readGamepadInputThread = std::async(std::launch::async, &Gamepad::readGamepadInput, this);
}

void Gamepad::stopThread()
{
    qInfo("in Gamepad::stopThread, terminating thread");
    isReadGamepadInputThreadActive = false;
    readGamepadInputThread.get();

    auto status = handleAxisInputsThread.wait_for(std::chrono::microseconds(0));
    if (status == std::future_status::ready)
    {
        handleAxisInputsThread.get();
    }

    status = handleButtonInputsThread.wait_for(std::chrono::microseconds(0));
    if (status == std::future_status::ready)
    {
        handleButtonInputsThread.get();
    }
}

void Gamepad::waitForExitButton()
{
    qInfo("in Gamepad::waitForExitButton, waiting for exit");
    readGamepadInputThread.get();
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

void Gamepad::launchHandleAxisInputThread()
{
    auto status = handleAxisInputsThread.wait_for(std::chrono::microseconds(0));
    if (status == std::future_status::ready)
    {
        handleAxisInputsThread.get();
        handleAxisInputsThread = std::async(std::launch::async, &Gamepad::handleAxisInput, this);
    }
}

void Gamepad::launchHandleButtonInputThread()
{
    auto status = handleButtonInputsThread.wait_for(std::chrono::microseconds(0));
    if (status == std::future_status::ready)
    {
        handleButtonInputsThread.get();
        handleButtonInputsThread = std::async(std::launch::async, &Gamepad::handleButtonInput, this);
    }
}

void Gamepad::readGamepadInput()
{
    clearInput();
    isReadGamepadInputThreadActive = true;

    while(isReadGamepadInputThreadActive)
    {
        if (!isGamepadConnected())
        {
            qCritical( "Gamepad disconnected");
            throw EXIT_BY_MISSING_GAMEPAD;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(GAMEPAD_REFRESH_TIME));

        if(stepperMotor->getAllowedDirection().forward == false
                && getAxisValue(RIGHT_TRIGGER) > AXIS_MIN_TRIGGER_POSITION)
        {
            mustAxisReturnToZero[RIGHT_TRIGGER] = true;
        }
        if(stepperMotor->getAllowedDirection().backward == false
                && getAxisValue(LEFT_TRIGGER) > AXIS_MIN_TRIGGER_POSITION)
        {
            mustAxisReturnToZero[LEFT_TRIGGER] = true;
        }

        JoystickEvent event;
        if(joystick->sample(&event))
        {
            if(event.isButton())
            {
                buttonState[ButtonID(event.number)] = bool(event.value);
                buttonEvents.push_back({ButtonID(event.number), bool(event.value)});

                launchHandleButtonInputThread();

            }
            else if (event.isAxis())
            {
                axisState[AxisID(event.number)] = event.value;
                axisEvents.push_back({AxisID(event.number), event.value});

                launchHandleAxisInputThread();
            }
        }
    }
}

const bool &Gamepad::isButtonPressed(const ButtonID &buttonID)
{
    return buttonState.find(buttonID)->second;
}

const short &Gamepad::getAxisValue(const AxisID &axisID)
{
    return axisState.find(axisID)->second;
}

void Gamepad::handleButtonInput()
{
    while(buttonEvents.size() > 0 && isReadGamepadInputThreadActive)
    {
        ButtonEvent buttonEvent = *buttonEvents.begin();
        switch (buttonEvent.buttonID)
        {
            case EXIT_BUTTON:
                if(buttonEvent.buttonValue == BUTTON_DOWN)
                {
                    qInfo("in Gamepad::handleInput, exit pressed");
                    isReadGamepadInputThreadActive = false;
                }
                break;

            case X_BUTTON:
                if(buttonEvent.buttonValue == BUTTON_DOWN)
                {
                    stepperMotor->brake();
                    if(lights->getReverseLightStatus() == REVERSE_LIGHT_ON)
                    {
                        lights->setReverseLight(REVERSE_LIGHT_OFF);
                    }
                }
                else
                {
                    stepperMotor->switchOff();

                    if(getAxisValue(LEFT_TRIGGER) > AXIS_MIN_TRIGGER_POSITION)
                    {
                        axisEvents.push_back({LEFT_TRIGGER, getAxisValue(LEFT_TRIGGER)});
                        launchHandleAxisInputThread();
                    }
                    if(getAxisValue(RIGHT_TRIGGER) > AXIS_MIN_TRIGGER_POSITION)
                    {
                        axisEvents.push_back({RIGHT_TRIGGER, getAxisValue(RIGHT_TRIGGER)});
                        launchHandleAxisInputThread();
                    }
                }
                break;

            case RIGHT_BUMPER:
                if(buttonEvent.buttonValue == BUTTON_DOWN)
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
                if(buttonEvent.buttonValue == BUTTON_DOWN)
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
        buttonState[buttonEvent.buttonID] = buttonEvent.buttonValue;
        buttonEvents.erase(buttonEvents.begin());
    }
}

void Gamepad::handleAxisInput()
{
    while(axisEvents.size() > 0 && isReadGamepadInputThreadActive)
    {
        AxisEvent axisEvent = *axisEvents.begin();
        switch (axisEvent.axisID)
        {
        case RIGHT_TRIGGER:
            if(!isButtonPressed(X_BUTTON) && stepperMotor->getAllowedDirection().forward && !mustAxisReturnToZero[axisEvent.axisID])
            {
                if(axisEvent.axisValue > AXIS_MIN_TRIGGER_POSITION)
                {
                    if(getAxisValue(LEFT_TRIGGER) < AXIS_MIN_TRIGGER_POSITION)
                    {
                        stepperMotor->move(DIRECTION_FORWARD, axisToSpeed(axisEvent.axisValue), PRIORITY_MANUAL);
                    }
                    else
                    {
                        if(stepperMotor->getSpeed() > 0)
                        {
                            stepperMotor->switchOff();
                        }
                    }
                }
                else
                {
                    stepperMotor->switchOff();
                    if(getAxisValue(LEFT_TRIGGER) > AXIS_MIN_TRIGGER_POSITION)
                    {
                        axisEvents.push_back({LEFT_TRIGGER, getAxisValue(LEFT_TRIGGER)});
                    }
                }
            }
            else
            {
                if(axisEvent.axisValue <= AXIS_MIN_TRIGGER_POSITION)
                {
                    mustAxisReturnToZero[axisEvent.axisID] = false;
                }
            }
            break;

        case LEFT_TRIGGER:
            if(!isButtonPressed(X_BUTTON) && stepperMotor->getAllowedDirection().backward && !mustAxisReturnToZero[axisEvent.axisID])
            {
                if(axisEvent.axisValue > AXIS_MIN_TRIGGER_POSITION)
                {
                    if(getAxisValue(RIGHT_TRIGGER) < AXIS_MIN_TRIGGER_POSITION)
                    {

                        stepperMotor->move(DIRECTION_BACKWARD, axisToSpeed(axisEvent.axisValue), PRIORITY_MANUAL);
                    }
                    else
                    {
                        if(lights->getReverseLightStatus() == REVERSE_LIGHT_ON)
                        {
                            lights->setReverseLight(REVERSE_LIGHT_OFF);
                        }
                        if(stepperMotor->getSpeed() > 0)
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
                    if(stepperMotor->getSpeed() > 0)
                    {
                        stepperMotor->switchOff();
                    }
                    if(getAxisValue(RIGHT_TRIGGER) > AXIS_MIN_TRIGGER_POSITION)
                    {
                        axisEvents.push_back({RIGHT_TRIGGER, getAxisValue(RIGHT_TRIGGER)});
                    }
                }
            }
            else
            {
                if(axisEvent.axisValue <= AXIS_MIN_TRIGGER_POSITION)
                {
                    mustAxisReturnToZero[axisEvent.axisID] = false;
                }
            }
            break;

        case LEFT_X_AXIS:
            servo->turn(axisToDegrees(axisEvent.axisValue), PRIORITY_MANUAL);
            break;

        case ARROW_Y_AXIS:
            if(axisEvent.axisValue > 0) //Arrow DOWN
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
            else if(axisEvent.axisValue < 0) //Arrow UP
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
        axisState[axisEvent.axisID] = axisEvent.axisValue;
        axisEvents.erase(axisEvents.begin());
    }
}
