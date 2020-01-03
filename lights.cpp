#include "lights.h"

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

void Lights::setTurnSignal(TurnSignalCommand command)
{
    LightsAndServoMsg message;
    message.device = TURN_SIGNAL;
    message.turnSignalCommand = command;
    serialPort->send( (U8 *) &message, sizeof(LightsAndServoMsg) );
}

void Lights::setHeadLight(HeadLightCommand command)
{
    LightsAndServoMsg message;
    message.device = HEADLIGHT;
    message.headLightCommand = command;
    serialPort->send( (U8 *) &message, sizeof(LightsAndServoMsg) );
}

void Lights::setBrakeLights(BrakeLightsCommand command)
{
    LightsAndServoMsg message;
    message.device = BRAKE_LIGHT;
    message.brakeLightsCommand = command;
    serialPort->send( (U8 *) &message, sizeof(LightsAndServoMsg) );
}

void Lights::setReverseLight(ReverseLightCommand command)
{
    LightsAndServoMsg message;
    message.device = REVERSE_LIGHT;
    message.reverseLightCommand = command;
    serialPort->send( (U8 *) &message, sizeof(LightsAndServoMsg) );
}
