#include "gamepad.h"
#include "serialport.h"
#include "servo.h"
#include "lights.h"

#include <future>

SerialPort lightsAndServoSerial(LIGHTS_AND_SERVO_PORT);
Servo servo(&lightsAndServoSerial);
Lights lights(&lightsAndServoSerial);
StepperMotor stepperMotor;
Gamepad gamepad(&stepperMotor, &servo, &lights);


int main()
{
    auto gamepadThread = std::async(std::launch::async, &Gamepad::readGamepadInput, &gamepad);
}
