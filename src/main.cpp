#include <inc/gamepad.h>
#include <inc/serialport.h>
#include <inc/servo.h>
#include <inc/lights.h>
#include <inc/logutils.h>

#include <future>

int main()
{
    LOGUTILS::initLogging(true);

    SerialPort* lightsAndServoSerial;
    Servo* servo;
    Lights* lights;
    Gamepad* gamepad;
    StepperMotor* stepperMotor;

    try
    {
        lightsAndServoSerial = new SerialPort();
        servo = new Servo(lightsAndServoSerial);
        lights = new Lights(lightsAndServoSerial);
        stepperMotor =  new StepperMotor();
        gamepad = new Gamepad(stepperMotor, servo, lights);
    }
    catch(ExitReason excp)
    {
        switch(excp)
        {
        case EXIT_BY_FAILED_GPIO:
            delete lights;
            delete servo;
            delete lightsAndServoSerial;
            break;
        case EXIT_BY_MISSING_GAMEPAD:
            delete lights;
            delete servo;
            delete lightsAndServoSerial;
            delete stepperMotor;
            break;
        default:
            break;
        }
        return 0;
    }

    try
    {
        gamepad->startThread();
        gamepad->waitForExitButton();
    }
    catch(ExitReason excp)
    {
        qCritical() << "Exception catched: " << excp;
        switch(excp)
        {
        default:
            delete lights;
            delete servo;
            delete lightsAndServoSerial;
            delete stepperMotor;
            delete gamepad;
            break;
        }
        return excp;
    }

    delete lights;
    delete servo;
    delete lightsAndServoSerial;
    delete stepperMotor;
    delete gamepad;

    return 0;
}
