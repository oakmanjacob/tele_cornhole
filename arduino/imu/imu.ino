#include <Wire.h>
#include <SPI.h>
#include <Adafruit_LSM9DS1.h>
#include <Adafruit_Sensor.h>  // not used in this demo but required!
#include <math.h>
#include <Adafruit_MotorShield.h>

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield();

// Select which 'port' M1, M2, M3 or M4. In this case, M1
Adafruit_DCMotor *xMotor = AFMS.getMotor(1);
Adafruit_DCMotor *yMotor = AFMS.getMotor(2);

// i2c
Adafruit_LSM9DS1 lsm = Adafruit_LSM9DS1();

typedef struct _movement_angles
{
  double tilt_angle; 
  double pan_angle;
}Movement_Angles;

double error = 0.5;

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

void setupSensor()
{
  // 1.) Set the accelerometer range
  lsm.setupAccel(lsm.LSM9DS1_ACCELRANGE_2G);

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
  Serial.println("Found LSM9DS1 9DOF");

  Serial.println("Aceelerometer Intialized");

}

void setupSerial()
{
  Serial.begin(115200);           // set up Serial library at 115200 bps
  
  Serial.println("Serial Driver Intialized");
}

void setupHorizontalMotor()
{
    // Needs integration
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

void setup() {

  // Setup serial port
  setupSerial();

  // Setup Accelerometer Sensor
  setupSensor();

  // Setup Vertical Motor
  setupVerticalMotor();

  // Setup Horizontal Motor
  setupHorizontalMotor();

  reset_angles.pan_angle  = 0, // TODO - Find experimentally and set value here
  reset_angles.tilt_angle = 0, // TODO - Find experimentally and set value here

  curr_state = STATE_DISENGAGED;
}

void loop() {
  switch (curr_state)
  {
    case STATE_DISENGAGED:
    {
      bool status = false;
        // TODO - Integrate Horizontal Homing
        // status = goToPanAngle(reset_pan_angle);

        // Vertical Homing
        
        /* Get a new sensor event */
        /* 
        lsm.read();  
        sensors_event_t a, m, g, temp;
        lsm.getEvent(&a, &m, &g, &temp); 
        reset_tilt = atan2(a.acceleration.x, a.acceleration.y) * (180/3.141592);
        Serial.print(" Reset tilt angle:"); Serial.print(reset_tilt);   Serial.println("deg");
        */  

      if (status == true) // TODO - Remove for test
        status = goToTiltAngle(reset_angles.tilt_angle);

      if (status == true) curr_state = STATE_GET_INPUT;
    }
    break;
    case STATE_GET_INPUT:
    {
       // To integrate

       // Get Horizontal Angle Input
       // set state to STATE_ARM_CANNON_HORIZONTAL
       // Send response to app

       // Get Vertical Angle Input
       // set state to STATE_ARM_CANNON_VERTICAL
       // Send response to app

       // Get Fire Input
       // set state to STATE_FIRE_CANNON
       // Send response to app

    }
    break;
    case STATE_ARM_CANNON_VERTICAL:
    {
        bool status = false;
        if( curr_state == STATE_GET_INPUT)
        {
          status = goToTiltAngle(input_data.user_angles.tilt_angle);
        }
    }
    break;
    case STATE_ARM_CANNON_HORIZONTAL:
    {}
    break;
    case STATE_FIRE_CANNON:
    {}
    break;
    default:
    {

    }
    break;
  }

}

bool goToTiltAngle(double target) {

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
  while (abs(tilt_angle - target) > error);

  yMotor->setSpeed(0);

  return true;
}
