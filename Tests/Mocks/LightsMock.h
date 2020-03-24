#pragma once

#include <gmock/gmock.h>
#include <inc/lights.h>
#include <inc/common.h>

class LightsMock : public Lights
{
public:
    MOCK_METHOD(TurnSignalCommand, getTurnSignalStatus, ());
    MOCK_METHOD(HeadLightCommand, getHeadLightStatus, ());
    MOCK_METHOD(BrakeLightsCommand, getBrakeLightsStatus, ());
    MOCK_METHOD(ReverseLightCommand, getReverseLightStatus, ());
    MOCK_METHOD(BrakeLightsCommand, getBrakeLightsWhenOffStatus, ());

    MOCK_METHOD(void, setTurnSignalCommand, (const TurnSignalCommand &command));
    MOCK_METHOD(void, setHeadLight, (const HeadLightCommand &command));
    MOCK_METHOD(void, setBrakeLights, (const BrakeLightsCommand &command));
    MOCK_METHOD(void, setBrakeLightsWhenOff, (const BrakeLightsCommand &command));
    MOCK_METHOD(void, setReverseLight, (const ReverseLightCommand &command));
};
