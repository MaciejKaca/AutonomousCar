#pragma once

#include <Arduino.h>
#include <AccelStepper.h>
#include <stepperInterface.h>

const byte DIRECTION_PIN = 7;
const byte STEPPER_PIN = 8;
const byte MOTOR_INTERFACE_TYPE = 1;
const byte STEPPER_DRIVER_POWER = 2;

const int MAX_MOTOR_SPEED = 1000;
const int INITIAL_SPEED = 0;

bool isMessagePresent()
{
    if(Serial.available() )
    {
        return true;
    }
    return false;
}

void handleMessage(StepperMotorMsg message, AccelStepper &stepperMotor)
{
    switch (message.command)
    {
    case RUN:
        if(message.direction == BACKWARD)
        {
            stepperMotor.setSpeed(message.speed * -1);
        }
        else
        {
            stepperMotor.setSpeed(message.speed);
        }
            
        digitalWrite(STEPPER_DRIVER_POWER, HIGH);
        break;
        
    case BRAKE:
        stepperMotor.setSpeed(0);
        break;

    case SWICTH_OFF:
        digitalWrite(STEPPER_DRIVER_POWER, LOW);
        stepperMotor.setSpeed(0);
        break;

    case MOVE_DISTANCE:
        //TODO
        break;
        
    }
}

void receiveMessage(StepperMotorMsg &message)
{
     Serial.readBytes((char*)&message, sizeof(StepperMotorMsg));
}
