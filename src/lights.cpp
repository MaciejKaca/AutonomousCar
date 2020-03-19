#include <inc/lights.h>

Lights::Lights(SerialPort * _serialPort) : serialPort(_serialPort)
{
    turnSignalStatus = TURN_SIGNAL_OFF;
    headLightStatus = HEADLIGHT_OFF;
    brakeLightsStatus = BRAKE_LIGHT_OFF;
    reverseLightStatus = REVERSE_LIGHT_OFF;
}

BrakeLightsCommand Lights::getBrakeLightsStatus()
{
    return brakeLightsStatus;
}

HeadLightCommand Lights::getHeadLightStatus()
{
    return headLightStatus;
}

ReverseLightCommand Lights::getReverseLightStatus()
{
    return reverseLightStatus;
}

TurnSignalCommand Lights::getTurnSignalStatus()
{
    return turnSignalStatus;
}

void Lights::setTurnSignal(const TurnSignalCommand command)
{
    turnSignalStatus = command;

    LightsAndServoMsg message;
    message.device = TURN_SIGNAL;
    message.turnSignalCommand = turnSignalStatus;
    serialPort->send( (U8 *) &message, sizeof(LightsAndServoMsg) );
}

void Lights::setHeadLight(const HeadLightCommand command)
{
    headLightStatus = command;

    LightsAndServoMsg message;
    message.device = HEADLIGHT;
    message.headLightCommand = headLightStatus;
    serialPort->send( (U8 *) &message, sizeof(LightsAndServoMsg) );
}

void Lights::setBrakeLights(const BrakeLightsCommand command)
{
    brakeLightsStatus = command;

    if(command == BRAKE_LIGHT_OFF)
    {
        brakeLightsStatus = brakeLightsWhenOffStatus;
    }
    LightsAndServoMsg message;
    message.device = BRAKE_LIGHT;
    message.brakeLightsCommand = brakeLightsStatus;
    serialPort->send( (U8 *) &message, sizeof(LightsAndServoMsg) );
}

void Lights::setReverseLight(const ReverseLightCommand command)
{
    reverseLightStatus = command;

    LightsAndServoMsg message;
    message.device = REVERSE_LIGHT;
    message.reverseLightCommand = reverseLightStatus;
    serialPort->send( (U8 *) &message, sizeof(LightsAndServoMsg) );
}

void Lights::setBrakeLightsWhenOff(BrakeLightsCommand command)
{
    brakeLightsWhenOffStatus = command;

    setBrakeLights(brakeLightsWhenOffStatus);
}
