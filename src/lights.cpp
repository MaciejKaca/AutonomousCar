#include <inc/lights.h>

Lights::Lights(SerialPort * _serialPort) : serialPort(_serialPort)
{
    qInfo("in Lights, initializing constructor");
    brakeLightsWhenOffStatus = BRAKE_LIGHT_OFF;
    turnSignalStatus = TURN_SIGNAL_OFF;
    headLightStatus = HEADLIGHT_OFF;
    brakeLightsStatus = BRAKE_LIGHT_OFF;
    reverseLightStatus = REVERSE_LIGHT_OFF;

    this->setTurnSignal(turnSignalStatus);
    this->setHeadLight(headLightStatus);
    this->setBrakeLights(brakeLightsStatus);
    this->setReverseLight(reverseLightStatus);
}

const BrakeLightsCommand &Lights::getBrakeLightsStatus()
{
    return brakeLightsStatus;
}

const HeadLightCommand &Lights::getHeadLightStatus()
{
    return headLightStatus;
}

const ReverseLightCommand &Lights::getReverseLightStatus()
{
    return reverseLightStatus;
}

const TurnSignalCommand &Lights::getTurnSignalStatus()
{
    return turnSignalStatus;
}

const BrakeLightsCommand &Lights::getBrakeLightsWhenOffStatus()
{
    return brakeLightsWhenOffStatus;
}

void Lights::setTurnSignal(const TurnSignalCommand &command)
{
    turnSignalStatus = command;

    LightsAndServoMsg message;
    message.device = TURN_SIGNAL;
    message.turnSignalCommand = turnSignalStatus;

    qInfo() << "in Lights::setTurnSignal():" << turnSignalStatus;
    serialPort->send((U8 *)&message, sizeof(LightsAndServoMsg));
}

void Lights::setHeadLight(const HeadLightCommand &command)
{
    headLightStatus = command;

    LightsAndServoMsg message;
    message.device = HEADLIGHT;
    message.headLightCommand = headLightStatus;

    qInfo() << "in Lights::setHeadLight():" << headLightStatus;
    serialPort->send((U8 *) &message, sizeof(LightsAndServoMsg));
}

void Lights::setBrakeLights(const BrakeLightsCommand &command)
{
    brakeLightsStatus = command;

    if(command == BRAKE_LIGHT_OFF)
    {
        brakeLightsStatus = brakeLightsWhenOffStatus;
    }
    LightsAndServoMsg message;
    message.device = BRAKE_LIGHT;
    message.brakeLightsCommand = brakeLightsStatus;

    qInfo() << "in Lights::setBrakeLights():" << brakeLightsWhenOffStatus;
    serialPort->send((U8 *)&message, sizeof(LightsAndServoMsg));
}

void Lights::setReverseLight(const ReverseLightCommand &command)
{
    reverseLightStatus = command;

    LightsAndServoMsg message;
    message.device = REVERSE_LIGHT;
    message.reverseLightCommand = reverseLightStatus;

    qInfo() << "in Lights::setReverseLight():" << reverseLightStatus;
    serialPort->send((U8 *)&message, sizeof(LightsAndServoMsg));
}

void Lights::setBrakeLightsWhenOff(const BrakeLightsCommand &command)
{
    brakeLightsWhenOffStatus = command;
    qInfo() << "in Lights::setBrakeLightsWhenOff(): setting default status when brake lights OFF = "
            << brakeLightsWhenOffStatus;
    setBrakeLights(brakeLightsWhenOffStatus);
}
