#pragma once

class GamepadBase
{
    public:
        virtual bool readGamepadInput() = 0;
        virtual ~GamepadBase() = 0;
};
