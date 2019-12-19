#ifndef GAMEPAD_H
#define GAMEPAD_H

#include <joystick/joystick.hh>

class Gamepad
{
public:
    Gamepad();
    void handleInput();

private:
    Joystick joystick;
    void clearInput();
};

#endif // GAMEPAD_H
