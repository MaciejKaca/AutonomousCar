#pragma once

class GamepadBase
{
    public:
        virtual void readGamepadInput() = 0;
        virtual ~GamepadBase() = 0;
};
