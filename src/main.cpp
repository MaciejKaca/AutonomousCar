#include <inc/gamepad.h>
#include <inc/serialport.h>
#include <inc/servo.h>
#include <inc/lights.h>
#include <inc/logutils.h>
#include <inc/DistanceSensors.h>
#include <inc/SafetySystem.h>
#include <inc/StepperMotorShell.h>
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
    StepperMotorShell* stepperMotorShell;
    DistanceSensors *distanceSensors;
    SafetySystem *safetySystems;

    try
    {
        lightsAndServoSerial = new SerialPort();
        distanceSensors = new DistanceSensors();
        lights = new Lights(lightsAndServoSerial);
        stepperMotorShell =  new StepperMotorShell(lights);
        servo = new Servo(lightsAndServoSerial, stepperMotorShell);
        safetySystems = new SafetySystem(servo, stepperMotorShell, distanceSensors);
        gamepad = new Gamepad(stepperMotorShell, servo, lights);
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
            delete stepperMotorShell;
            delete safetySystems;
            break;
        default:
            break;
        }
        return 0;
    }

    try
    {
        stepperMotorShell->setSafetySystem(safetySystems);
        gamepad->startThread();
        gamepad->waitForExitButton();
    }
    catch(ExitReason excp)
    {
        qCritical() << "Exception catched: " << excp;
        switch(excp)
        {
        default:
            delete safetySystems;
            delete distanceSensors;
            delete lights;
            delete servo;
            delete lightsAndServoSerial;
            delete stepperMotorShell;
            delete gamepad;
            break;
        }
        return excp;
    }

    delete safetySystems;
    delete lights;
    delete servo;
    delete lightsAndServoSerial;
    delete distanceSensors;
    delete stepperMotorShell;
    delete gamepad;

    return 0;
}
