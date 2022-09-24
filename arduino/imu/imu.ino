#include <Wire.h>
#include <SPI.h>
#include <Adafruit_LSM9DS1.h>
#include <Adafruit_Sensor.h>  // not used in this demo but required!
#include <math.h>
#include <BasicStepperDriver.h>
#include <Adafruit_MotorShield.h>

//_____________________________________________________________________________________
//                        STEPPER DEFININITIONS
//_____________________________________________________________________________________

#define LIM_SW 10        //Limit switch pin
#define DIR 4            //Direction pin
#define STEP 3           //Step pin

#define MOTOR_STEPS 400  //Steps per revolution
#define RPM 120

#define MAX_ELEVATION 50
#define MIN_ELEVATION -40

//_____________________________________________________________________________________
//                        MOTOR GLOBALS
//_____________________________________________________________________________________

const double GEAR_RATIO = 70.5882;  //for every 70.5882 rotations of the stepper motor, the gantry will rotate once
const double HOME_POSITION = -48; //degrees from straight ahead

double gantryPosition = 0; //current position of the gantry (degrees) 

BasicStepperDriver stepper(MOTOR_STEPS, DIR, STEP);

Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *yMotor = AFMS.getMotor(1);

double tilt_offset = 0;
double error = 3;

//_____________________________________________________________________________________
//                        CANNON DEFININITIONS
//_____________________________________________________________________________________

#define CANNON_PIN 8

//_____________________________________________________________________________________
//                        IMU GLOBALS
//_____________________________________________________________________________________
Adafruit_LSM9DS1 lsm = Adafruit_LSM9DS1();


typedef struct _movement_angles
{
  double tilt_angle; 
  double pan_angle;
}Movement_Angles;



typedef enum _system_states
{
  STATE_DISENGAGED = 0,
  STATE_GET_INPUT,
  STATE_ARM_CANNON_HORIZONTAL,
  STATE_ARM_CANNON_VERTICAL,
  STATE_FIRE_CANNON,
  MAX_STATES
}System_states;

System_states curr_state = STATE_DISENGAGED;

typedef struct _user_input
{
  Movement_Angles user_angles; 
  bool fire;
}User_Input;

User_Input input_data;

Movement_Angles reset_angles;

//_____________________________________________________________________________________
//                        Setup
//_____________________________________________________________________________________
void setupSensor()
{
  // 1.) Set the accelerometer range
  Serial.println("Enter setup sensor");

  // 2.) Set the magnetometer sensitivity
  //lsm.setupMag(lsm.LSM9DS1_MAGGAIN_4GAUSS);

  // 3.) Setup the gyroscope
  //lsm.setupGyro(lsm.LSM9DS1_GYROSCALE_2000DPS);

  // Try to initialise and warn if we couldn't detect the chip
  if (!lsm.begin())
  {
    Serial.println("Oops ... unable to initialize the LSM9DS1. Check your wiring!");
    while (1);
  }

  lsm.setupAccel(lsm.LSM9DS1_ACCELRANGE_2G);

  Serial.println("LSM begin done");
  Serial.println("Found LSM9DS1 9DOF");

  Serial.println("Aceelerometer Intialized");

}

void setupSerial()
{
  Serial.begin(115200);           // set up Serial library at 115200 bps
  delay(3000);
  Serial.println("Serial Driver Intialized");
  
}

void setupHorizontalMotor()
{
  pinMode(LIM_SW, INPUT_PULLUP);
  stepper.begin(RPM, 1);
}

void setupVerticalMotor()
{
  if (!AFMS.begin()) {         // create with the default frequency 1.6KHz
  // if (!AFMS.begin(1000)) {  // OR with a different frequency, say 1KHz
    Serial.println("Could not find Motor Shield. Check wiring.");
    while (1);
  }
  Serial.println("Vertical Motor Shield found.");
}

void setupCannon()
{
  pinMode(CANNON_PIN, OUTPUT);
}

void setup() {

  // Setup serial port
  setupSerial();

  // Setup Cannon
  setupCannon();

  // Setup Accelerometer Sensor
  setupSensor();

  // Setup Vertical Motor
  setupVerticalMotor();

  // Setup Stepper(Horizontal Motor)
  setupHorizontalMotor();

  reset_angles.pan_angle  = 0, // TODO - Find experimentally and set value here
  reset_angles.tilt_angle = 0, // TODO - Find experimentally and set value here

  curr_state = STATE_DISENGAGED;
}

void loop() 
{
  switch (curr_state)
  {
    case STATE_DISENGAGED:
    {
      bool status = false;
      
      status = homeStepper();
      if (status == true)
      {
        status = goToTiltAngle(reset_angles.tilt_angle);
        if (status == true)
        {
          curr_state = STATE_GET_INPUT;
        }
        else
        {
          Serial.println("Reset tilt angle failed!");
        }
      }
      else
      {
        Serial.println("Reset pan angle failed!");
      }
    }
    break;

    case STATE_GET_INPUT:
    {
      if(Serial.available())
      {
        double buff;
        char input;
        input = Serial.read();
        switch (input) 
        {
          case 'x':
          {
            // Get Horizontal Angle Input
            buff = 361;
            while (buff > -HOME_POSITION || buff < HOME_POSITION) 
            {
              if(Serial.available() > 0)
              {
                buff = Serial.parseFloat();
                //Serial.print("pan angle input"); Serial.println(buff);
                input_data.user_angles.pan_angle = buff;

                while (Serial.available()) {
                  Serial.read();  
                }
                // set state to STATE_ARM_CANNON_HORIZONTAL
                curr_state = STATE_ARM_CANNON_HORIZONTAL;
              }
            }

          }
          break;

          case 'y':
          {
            // Get Vertical Angle Input         
            buff = 361;
            while (buff > MAX_ELEVATION || buff < MIN_ELEVATION) 
            {
              if(Serial.available() > 0)
              {
                buff = Serial.parseFloat();
                //Serial.print("tilt angle input"); Serial.println(buff);
                input_data.user_angles.tilt_angle = buff;

                while (Serial.available()) {
                  Serial.read();  
                }

                // set state to STATE_ARM_CANNON_VERTICAL
                curr_state = STATE_ARM_CANNON_VERTICAL;
              }
            }
          }
          break;

          case 'f':
          {
            curr_state = STATE_FIRE_CANNON;
            while (Serial.available()) {
              Serial.read();  
            }
          }
          break;

          default:
          break;
        }
      }
    }
    break;

    case STATE_ARM_CANNON_VERTICAL:
    {
        bool status = false;
        status = goToTiltAngle(input_data.user_angles.tilt_angle);
        if(status == true)
        {
            // Send response to app
            Serial.write('1');
            curr_state = STATE_GET_INPUT;
        }
        else
        {
            // Go home in case of trouble :)
            // Send response to app
            Serial.write('0');
            curr_state = STATE_DISENGAGED;
        }
    }
    break;

    case STATE_ARM_CANNON_HORIZONTAL:
    {
        bool status = false;
        status = goToRotation(input_data.user_angles.pan_angle);
        if(status == true)
        {
            // Send response to app
            Serial.write('1');
            curr_state = STATE_GET_INPUT;
        }
        else
        {
            // Go home in case of trouble :)
            // Send response to app
            Serial.write('0');
            curr_state = STATE_DISENGAGED;
        }
    }
    break;

    case STATE_FIRE_CANNON:
    {
        bool status = false;
        
        status = fire();
        if(status == true)
        {
            // Send response to app
            Serial.write('1');
        }
        else
        {
            // Go home in case of trouble :)
            // Send response to app
            Serial.write('0');
        }
        curr_state = STATE_DISENGAGED;
    }
    break;    
    
    default:
    {

    }
    break;
  }
}

bool homeStepper(void)
{
  bool isHome = false;

  stepper.startRotate(-360 * GEAR_RATIO); //start moving the gantry towards the limit switch (degrees)
  
  while (!isHome)
  {
    if (digitalRead(LIM_SW) == LOW)
    {
        stepper.stop();
        isHome = true;
        Serial.println("LIMIT SWITCH REACHED");
    }
    stepper.nextAction();
  }
 
  gantryPosition = HOME_POSITION;
  return true;
}

bool goToRotation(double newGantryPosition)
{
  double movementAmount = (newGantryPosition - gantryPosition) * GEAR_RATIO;
  stepper.rotate(movementAmount);
  gantryPosition = newGantryPosition;
  return true;
}

bool goToTiltAngle(double target) 
{
  /* Get a new sensor event */ 
  sensors_event_t a, m, g, temp;
  double tilt_angle;

  do{
    lsm.read();  /* ask it to read in the data */ 
    lsm.getEvent(&a, &m, &g, &temp); 
    tilt_angle = atan2(a.acceleration.x, a.acceleration.y) * (180/3.141592);
    //Serial.print(" Current tilt angle:"); Serial.print(tilt_angle);   Serial.println("deg");

    if (tilt_angle > target) {
      yMotor->run(FORWARD);
      yMotor->setSpeed(255);      
    }
    else {
      yMotor->run(BACKWARD);
      yMotor->setSpeed(255);
    }
    delay(50);
  }
  while (abs(tilt_angle - target) > error);

  yMotor->setSpeed(0);

  return true;
}

bool fire() 
{
  digitalWrite(CANNON_PIN, HIGH);
  delay(500);
  digitalWrite(CANNON_PIN, LOW);
}
