#include <AccelStepper.h>

#include "../Interface/stepperInterface.h"
#include "stepper.h"

AccelStepper stepperMotor = AccelStepper(MOTOR_INTERFACE_TYPE, STEPPER_PIN, DIRECTION_PIN);
StepperMotorMsg message;

void setup()
{
   stepper.setMaxSpeed(MAX_MOTOR_SPEED);
   stepper.setSpeed(INITIAL_SPEED);	
}

void loop()
{
   if(isMessagePresent())
   {
      receiveMessage(message)
      {
         handleMessage(message);
      }
   }
}