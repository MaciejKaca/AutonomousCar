#include <QApplication>
#include <wiringPi.h>

#include <inc/gamepad.h>
#include <inc/serialport.h>
#include <inc/servo.h>
#include <inc/lights.h>
#include <inc/logutils.h>
#include <inc/DistanceSensors.h>
#include <inc/SafetySystem.h>
#include <inc/StepperMotorShell.h>
#include <inc/SettingsWindow.h>
#include <inc/FileHandling.h>
#include <inc/IMU.h>

int main(int argc, char *argv[])
{
    LOGUTILS::initLogging(false);

    QApplication settingsApp(argc, argv);
    SettingsWindow settingsWindow;
    settingsWindow.show();
    settingsApp.exec();

    wiringPiSetup();

    SerialPort* lightsAndServoSerial;
    SerialPort* imuSerial;
    Servo* servo;
    Lights* lights;
    Gamepad* gamepad;
    StepperMotorShell* stepperMotorShell;
    DistanceSensors *distanceSensors;
    SafetySystem *safetySystems;
    FileHandling *fileHandling;

    try
    {
        fileHandling = new FileHandling();
        lightsAndServoSerial = new SerialPort(LIGHTS_AND_SERVO_PORT);
        imuSerial = new SerialPort(IMU_PORT);
        distanceSensors = new DistanceSensors();
        lights = new Lights(lightsAndServoSerial);
        stepperMotorShell =  new StepperMotorShell(lights);
        servo = new Servo(lightsAndServoSerial, stepperMotorShell);
        safetySystems = new SafetySystem(servo, stepperMotorShell, distanceSensors);
        gamepad = new Gamepad(stepperMotorShell, servo, lights, fileHandling);
    }
    catch(ExitReason excp)
    {
        switch(excp)
        {
        case EXIT_BY_MISSING_GAMEPAD:
            delete imuSerial;
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
            delete imuSerial;
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

    delete imuSerial;
    delete safetySystems;
    delete lights;
    delete servo;
    delete lightsAndServoSerial;
    delete distanceSensors;
    delete stepperMotorShell;
    delete gamepad;

    return 0;
}
