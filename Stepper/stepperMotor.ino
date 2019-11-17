#include <stepperMotor.h>
#include <stepperInterface.h>

AccelStepper stepperMotor = AccelStepper(MOTOR_INTERFACE_TYPE, STEPPER_PIN, DIRECTION_PIN);
StepperMotorMsg message;

void setup()
{
   stepperMotor.setMaxSpeed(MAX_MOTOR_SPEED);
   stepperMotor.setSpeed(INITIAL_SPEED);	
}

void loop()
{
   if(isMessagePresent())
   {
      receiveMessage(message);
   }

   handleMessage(message, stepperMotor);
}
