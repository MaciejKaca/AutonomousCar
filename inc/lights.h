#pragma once

#include "common.h"
#include "serialport.h"

class Lights
{

public:
    Lights(SerialPort *_serialPort);
    TurnSignalCommand getTurnSignalStatus();
    HeadLightCommand getHeadLightStatus();
    BrakeLightsCommand getBrakeLightsStatus();
    ReverseLightCommand getReverseLightStatus();

    void setTurnSignal(const TurnSignalCommand command);
    void setHeadLight(const HeadLightCommand command);
    void setBrakeLights(const BrakeLightsCommand command);
    void setBrakeLightsWhenOff(const BrakeLightsCommand command);
    void setReverseLight(const ReverseLightCommand command);

private:
    SerialPort * serialPort;

    TurnSignalCommand turnSignalStatus;
    HeadLightCommand headLightStatus;
    BrakeLightsCommand brakeLightsStatus;
    BrakeLightsCommand brakeLightsWhenOffStatus;
    ReverseLightCommand reverseLightStatus;
};
