#include <inc/gamepad.h>
#include <inc/serialport.h>
#include <inc/servo.h>
#include <inc/lights.h>
#include <inc/logutils.h>
#include <inc/DistanceSensors.h>
#include <wiringPi.h>

#include <future>

int main()
{
    LOGUTILS::initLogging(false);
    wiringPiSetup();

    SerialPort* lightsAndServoSerial;
    Servo* servo;
    Lights* lights;
    Gamepad* gamepad;
    StepperMotor* stepperMotor;
    DistanceSensors *distanceSensors;

    try
    {
        lightsAndServoSerial = new SerialPort();
        stepperMotor =  new StepperMotor();
        distanceSensors = new DistanceSensors();
        servo = new Servo(lightsAndServoSerial, stepperMotor);
        lights = new Lights(lightsAndServoSerial);
        gamepad = new Gamepad(stepperMotor, servo, lights);
    }
    catch(ExitReason excp)
    {
        switch(excp)
        {
        case EXIT_BY_MISSING_GAMEPAD:
            delete distanceSensors;
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
        distanceSensors->startThread();
        gamepad->startThread();
        gamepad->waitForExitButton();
    }
    catch(ExitReason excp)
    {
        qCritical() << "Exception catched: " << excp;
        switch(excp)
        {
        default:
            delete distanceSensors;
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
    delete distanceSensors;
    delete stepperMotor;
    delete gamepad;

    return 0;
}
