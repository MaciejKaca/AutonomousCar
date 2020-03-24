#pragma once

#include <inc/common.h>
#include <inc/serialport.h>

class LightsBase
{

public:
    virtual const TurnSignalCommand &getTurnSignalStatus() = 0;
    virtual const HeadLightCommand &getHeadLightStatus() = 0;
    virtual const BrakeLightsCommand &getBrakeLightsStatus() = 0;
    virtual const ReverseLightCommand &getReverseLightStatus() = 0;
    virtual const BrakeLightsCommand &getBrakeLightsWhenOffStatus() = 0;

    virtual void setTurnSignal(const TurnSignalCommand &command) = 0;
    virtual void setHeadLight(const HeadLightCommand &command) = 0;
    virtual void setBrakeLights(const BrakeLightsCommand &command) = 0;
    virtual void setBrakeLightsWhenOff(const BrakeLightsCommand &command) = 0;
    virtual void setReverseLight(const ReverseLightCommand &command) = 0;
};
