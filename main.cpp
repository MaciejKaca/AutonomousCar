#include "gamepad.h"
#include "serialport.h"

#include <future>

SerialPort lightsAndServoSerial(LIGHTS_AND_SERVO_PORT);
StepperMotor stepperMotor;
Gamepad gamepad(&stepperMotor, &lightsAndServoSerial);


int main()
{
    auto gamepadThread = std::async(std::launch::async, &Gamepad::handleInput, &gamepad);
}
