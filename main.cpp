#include "gamepad.h"

#include <future>

Gamepad gamepad;

int main()
{
    auto gamepadThread = std::async(std::launch::async, &Gamepad::handleInput, &gamepad);
}
