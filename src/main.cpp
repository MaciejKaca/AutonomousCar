#include <inc/gamepad.h>
#include <inc/serialport.h>
#include <inc/servo.h>
#include <inc/lights.h>

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
