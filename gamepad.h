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
    bool isGamepadConnected();
    const std::string gamepadPath = "/dev/input/js0";
};

#endif // GAMEPAD_H
