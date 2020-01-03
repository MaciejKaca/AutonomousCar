#ifndef LIGHTS_H
#define LIGHTS_H

#include "common.h"
#include "serialport.h"

class Lights
{
public:
    Lights(SerialPort * _serialPort);
    TurnSignalCommand getTurnSignalStatus();
    HeadLightCommand getHeadLightStatus();
    BrakeLightsCommand getBrakeLightsStatus();
    ReverseLightCommand getReverseLightStatus();

    void setTurnSignal(TurnSignalCommand command);
    void setHeadLight(HeadLightCommand command);
    void setBrakeLights(BrakeLightsCommand command);
    void setReverseLight(ReverseLightCommand command);

private:
    SerialPort * serialPort;

    TurnSignalCommand turnSignalStatus;
    HeadLightCommand headLightStatus;
    BrakeLightsCommand brakeLightsStatus;
    ReverseLightCommand reverseLightStatus;
};

#endif // LIGHTS_H
