#pragma once

#include <gmock/gmock.h>
#include <inc/Base/FileHandlingBase.h>

class FileHandlingMock : public FileHandlingBase
{
public:
    MOCK_METHOD(U8, getBrakeButtonId, (), (override));
    MOCK_METHOD(U8, getLeftBlinkerButtonId, (), (override));
    MOCK_METHOD(U8, getRightBlinkerButtonId, (), (override));
    MOCK_METHOD(U8, getExitButtonId, (), (override));

    MOCK_METHOD(float, getAccelBiasX, (), (override));
    MOCK_METHOD(float, getAccelBiasY, (), (override));
    MOCK_METHOD(float, getAccelBiasZ, (), (override));
    MOCK_METHOD(float, getGyroBiasX, (), (override));
    MOCK_METHOD(float, getGyroBiasY, (), (override));
    MOCK_METHOD(float, getGyroBiasZ, (), (override));
    MOCK_METHOD(float, getMagBiasX, (), (override));
    MOCK_METHOD(float, getMagBiasY, (), (override));
    MOCK_METHOD(float, getMagBiasZ, (), (override));
    MOCK_METHOD(float, getMagScaleX, (), (override));
    MOCK_METHOD(float, getMagScaleY, (), (override));
    MOCK_METHOD(float, getMagScaleZ, (), (override));

    MOCK_METHOD(void, setBrakeButtonId, (const S32& value), (override));
    MOCK_METHOD(void, setLeftBlinkerButtonId, (const S32& value), (override));
    MOCK_METHOD(void, setRightBlinkerButtonId, (const S32& value), (override));
    MOCK_METHOD(void, setExitButtonId, (const S32& value), (override));
    MOCK_METHOD(void, setAccelBiasX, (const float& value), (override));
    MOCK_METHOD(void, setAccelBiasY, (const float& value), (override));
    MOCK_METHOD(void, setAccelBiasZ, (const float& value), (override));
    MOCK_METHOD(void, setGyroBiasX, (const float& value), (override));
    MOCK_METHOD(void, setGyroBiasY, (const float& value), (override));
    MOCK_METHOD(void, setGyroBiasZ, (const float& value), (override));
    MOCK_METHOD(void, setMagBiasX, (const float& value), (override));
    MOCK_METHOD(void, setMagBiasY, (const float& value), (override));
    MOCK_METHOD(void, setMagBiasZ, (const float& value), (override));
    MOCK_METHOD(void, setMagScaleX, (const float& value), (override));
    MOCK_METHOD(void, setMagScaleY, (const float& value), (override));
    MOCK_METHOD(void, setMagScaleZ, (const float& value), (override));
};
