#pragma once

#include "common.h"
#include "serialport.h"
#include "Base/LightsBase.h"

class Lights : LightsBase
{

public:
    Lights(SerialPort *_serialPort);
    const TurnSignalCommand &getTurnSignalStatus() override;
    const HeadLightCommand &getHeadLightStatus() override;
    const BrakeLightsCommand &getBrakeLightsStatus() override;
    const ReverseLightCommand &getReverseLightStatus() override;
    const BrakeLightsCommand &getBrakeLightsWhenOffStatus() override;

    void setTurnSignal(const TurnSignalCommand &command) override;
    void setHeadLight(const HeadLightCommand &command) override;
    void setBrakeLights(const BrakeLightsCommand &command) override;
    void setBrakeLightsWhenOff(const BrakeLightsCommand &command) override;
    void setReverseLight(const ReverseLightCommand &command) override;

private:
    SerialPort * serialPort;

    TurnSignalCommand turnSignalStatus;
    HeadLightCommand headLightStatus;
    BrakeLightsCommand brakeLightsStatus;
    BrakeLightsCommand brakeLightsWhenOffStatus;
    ReverseLightCommand reverseLightStatus;
};
