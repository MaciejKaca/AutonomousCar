#pragma once

#include "common.h"
#include "serialport.h"

class Lights
{

public:
    Lights(SerialPort *_serialPort);
    const TurnSignalCommand &getTurnSignalStatus();
    const HeadLightCommand &getHeadLightStatus();
    const BrakeLightsCommand &getBrakeLightsStatus();
    const ReverseLightCommand &getReverseLightStatus();
    const BrakeLightsCommand &getBrakeLightsWhenOffStatus();

    void setTurnSignal(const TurnSignalCommand &command);
    void setHeadLight(const HeadLightCommand &command);
    void setBrakeLights(const BrakeLightsCommand &command);
    void setBrakeLightsWhenOff(const BrakeLightsCommand &command);
    void setReverseLight(const ReverseLightCommand &command);

private:
    SerialPort * serialPort;

    TurnSignalCommand turnSignalStatus;
    HeadLightCommand headLightStatus;
    BrakeLightsCommand brakeLightsStatus;
    BrakeLightsCommand brakeLightsWhenOffStatus;
    ReverseLightCommand reverseLightStatus;
};
