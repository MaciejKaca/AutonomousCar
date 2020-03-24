#include <inc/gamepad.h>
#include <inc/serialport.h>
#include <inc/servo.h>
#include <inc/lights.h>
#include <inc/logutils.h>

#include <future>

int main()
{
    LOGUTILS::initLogging();

    SerialPort lightsAndServoSerial;
    Servo servo(&lightsAndServoSerial);
    Lights lights(&lightsAndServoSerial);
    StepperMotor stepperMotor;
    static Gamepad gamepad(&stepperMotor, &servo, &lights);

    auto gamepadThread = std::async(std::launch::async, &Gamepad::readGamepadInput, &gamepad);
    return 0;
}
