#include <Arduino.h>
#include "BasicStepperDriver.h"


//_____________________________________________________________________________________
//                        STEPPER DEFININITIONS
//_____________________________________________________________________________________

#define LIM_SW 10        //Limit switch pin
#define DIR 4            //Direction pin
#define STEP 3           //Step pin

#define MOTOR_STEPS 400  //Steps per revolution
#define RPM 120

#define MICROSTEPS 1
//_____________________________________________________________________________________
//                        STEPPER GLOBALS
//_____________________________________________________________________________________
const double GEAR_RATIO = 70.5882;  //for every 70.5882 rotations of the stepper motor, the gantry will rotate once
const double HOME_POSITION = -30.5; //degrees from straight ahead

double gantryPosition = 0; //current position of the gantry (degrees) 
//_____________________________________________________________________________________

BasicStepperDriver stepper(MOTOR_STEPS, DIR, STEP);

void setup() 
{
 pinMode(LIM_SW, INPUT_PULLUP);
 stepper.begin(RPM, MICROSTEPS);   

 homeStepper();
}

void loop() {
  // put your main code here, to run repeatedly:

}

int initStepper(void)
{
  
  return 0;
}

int homeStepper(void)
{
  bool isHome = false;

  stepper.startRotate(360 * 100); //start moving the gantry towards the limit switch (degrees)
  
  while (!isHome)
  {
    if (digitalRead(LIM_SW) == LOW)
    {
        stepper.stop();
        isHome = true;
        Serial.println("LIMIT SWITCH REACHED");
    }
    //unsigned wait_time_micros = stepper.nextAction();
/*
    // motor control loop - send pulse and return how long to wait until next pulse
    unsigned wait_time_micros = stepper.nextAction();

    // 0 wait time indicates the motor has stopped
    if (wait_time_micros <= 0) 
    {
        stepper.disable();       // comment out to keep motor powered
        delay(3600000);
    }
*/
  }
 
  gantryPosition = HOME_POSITION;
  return 0;
}

int rotateCannonTo(double newGantryPosition)
{
  double movementAmount = (newGantryPosition - gantryPosition) * GEAR_RATIO;
  stepper.rotate(movementAmount);
  gantryPosition = newGantryPosition;
  return 0;
}
