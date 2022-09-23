#include <Arduino.h>
#include <Adafruit_LSM9DS1.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <SPI.h>
#include <BasicStepperDriver.h>
#include <Adafruit_MotorShield.h>
#include <math.h>

//_____________________________________________________________________________________
//                        IMU DEFININITIONS
//_____________________________________________________________________________________

#define LSM9DS1_SCK A5
#define LSM9DS1_MISO 12
#define LSM9DS1_MOSI A4
#define LSM9DS1_XGCS 6
#define LSM9DS1_MCS 5

//_____________________________________________________________________________________
//                        IMU GLOBALS
//_____________________________________________________________________________________

Adafruit_LSM9DS1 lsm = Adafruit_LSM9DS1();

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
double error_bound = 1;

//_____________________________________________________________________________________
//                        CANNON DEFININITIONS
//_____________________________________________________________________________________

#define CANNON_PIN 8

//_____________________________________________________________________________________
//                        Setup
//_____________________________________________________________________________________

void setup() {
  // Start Serial Connection
  Serial.begin(115200);

  // Initialize Cannon
  pinMode(CANNON_PIN, OUTPUT);

  // Initialize Stepper
  pinMode(LIM_SW, INPUT_PULLUP);
  stepper.begin(RPM, 1);

  // Initialize Motor Shield
  if (!AFMS.begin()) {         // create with the default frequency 1.6KHz
    Serial.println("Could not find Motor Shield. Check wiring.");
    while (1);
  }

  // Initialize IMU
  if (!lsm.begin())
  {
    Serial.println("Oops ... unable to initialize the LSM9DS1. Check your wiring!");
    while (1);
  }

  lsm.setupAccel(lsm.LSM9DS1_ACCELRANGE_2G);
  lsm.setupMag(lsm.LSM9DS1_MAGGAIN_4GAUSS);
  lsm.setupGyro(lsm.LSM9DS1_GYROSCALE_2000DPS);

  // Send to home position
  homeStepper();
  //goToElevation(0);
}

void loop() {
  double xAngle, yAngle;
  
  Serial.println("Enter your x angle: ");
  double buff = 361;
  while (buff > -HOME_POSITION || buff < HOME_POSITION) {
    if(Serial.available() > 0){
      buff = Serial.parseFloat();
      Serial.println(buff);
      
      while (Serial.available()) {
        Serial.read();  
      }
    }
  }
  xAngle = buff;
  
  Serial.println("Enter your y angle: ");
  buff = 361;
  while (buff > MAX_ELEVATION || buff < MIN_ELEVATION) {
    if(Serial.available() > 0){
      buff = Serial.parseFloat();
      Serial.println(buff);

      while (Serial.available()) {
        Serial.read();  
      }
    }
  }
  yAngle = buff;

  Serial.println("Aiming");
  goToRotation(xAngle);
  goToElevation(yAngle);
  delay(200);
  
  Serial.println("Firing");
  fire();
  delay(1000);
}

int homeStepper(void)
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
  return 0;
}

int goToRotation(double newGantryPosition)
{
  double movementAmount = (newGantryPosition - gantryPosition) * GEAR_RATIO;
  stepper.rotate(movementAmount);
  gantryPosition = newGantryPosition;
  return 0;
}

int goToElevation(double target) {
  target = target + tilt_offset;

  /* Get a new sensor event */ 
  sensors_event_t a, m, g, temp;
  double tilt_angle;

  do{
    lsm.read();  /* ask it to read in the data */ 
    lsm.getEvent(&a, &m, &g, &temp); 
    tilt_angle = atan2(a.acceleration.x, a.acceleration.y) * (180/3.141592);
    Serial.print(" Current tilt angle:"); Serial.print(tilt_angle);   Serial.println("deg");

    if (tilt_angle > target) {
      yMotor->run(FORWARD);
      yMotor->setSpeed(255);      
    }
    else {
      yMotor->run(BACKWARD);
      yMotor->setSpeed(255);
    }
    delay(20);
  }
  while (abs(tilt_angle - target) > error_bound);

  yMotor->setSpeed(0);
  return 0;
}

int fire() {
  digitalWrite(CANNON_PIN, HIGH);
  delay(500);
  digitalWrite(CANNON_PIN, LOW);
}
