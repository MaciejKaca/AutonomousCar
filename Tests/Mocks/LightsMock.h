#pragma once

#include <gmock/gmock.h>
#include <inc/Base/LightsBase.h>
#include <inc/common.h>

class LightsMock : public LightsBase
{
public:
    MOCK_METHOD(const TurnSignalCommand&, getTurnSignalStatus, (), (override));
    MOCK_METHOD(const HeadLightCommand&, getHeadLightStatus, (), (override));
    MOCK_METHOD(const BrakeLightsCommand&, getBrakeLightsStatus, (), (override));
    MOCK_METHOD(const ReverseLightCommand&, getReverseLightStatus, (), (override));
    MOCK_METHOD(const BrakeLightsCommand&, getBrakeLightsWhenOffStatus, (), (override));

    MOCK_METHOD(void, setTurnSignal, (const TurnSignalCommand &command), (override));
    MOCK_METHOD(void, setHeadLight, (const HeadLightCommand &command), (override));
    MOCK_METHOD(void, setBrakeLights, (const BrakeLightsCommand &command), (override));
    MOCK_METHOD(void, setBrakeLightsWhenOff, (const BrakeLightsCommand &command), (override));
    MOCK_METHOD(void, setReverseLight, (const ReverseLightCommand &command), (override));
};
