#pragma once

#include <inc/common.h>

class FileHandlingBase
{
public:
    virtual U8 getBrakeButtonId() = 0;
    virtual U8 getLeftBlinkerButtonId() = 0;
    virtual U8 getRightBlinkerButtonId() = 0;
    virtual U8 getExitButtonId() = 0;
    virtual float getAccelBiasX() = 0;
    virtual float getAccelBiasY() = 0;
    virtual float getAccelBiasZ() = 0;
    virtual float getGyroBiasX() = 0;
    virtual float getGyroBiasY() = 0;
    virtual float getGyroBiasZ() = 0;
    virtual float getMagBiasX() = 0;
    virtual float getMagBiasY() = 0;
    virtual float getMagBiasZ() = 0;
    virtual float getMagScaleX() = 0;
    virtual float getMagScaleY() = 0;
    virtual float getMagScaleZ() = 0;

    virtual void setBrakeButtonId(const S32& value) = 0;
    virtual void setLeftBlinkerButtonId(const S32& value) = 0;
    virtual void setRightBlinkerButtonId(const S32& value) = 0;
    virtual void setExitButtonId(const S32& value) = 0;
    virtual void setAccelBiasX(const float& value) = 0;
    virtual void setAccelBiasY(const float& value) = 0;
    virtual void setAccelBiasZ(const float& value) = 0;
    virtual void setGyroBiasX(const float& value) = 0;
    virtual void setGyroBiasY(const float& value) = 0;
    virtual void setGyroBiasZ(const float& value) = 0;
    virtual void setMagBiasX(const float& value) = 0;
    virtual void setMagBiasY(const float& value) = 0;
    virtual void setMagBiasZ(const float& value) = 0;
    virtual void setMagScaleX(const float& value) = 0;
    virtual void setMagScaleY(const float& value) = 0;
    virtual void setMagScaleZ(const float& value) = 0;
};
