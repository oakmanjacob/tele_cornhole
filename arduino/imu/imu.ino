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

int xTime = 0;
int yTime = 0;


// i2c
Adafruit_LSM9DS1 lsm = Adafruit_LSM9DS1();

#define LSM9DS1_SCK A5
#define LSM9DS1_MISO 12
#define LSM9DS1_MOSI A4
#define LSM9DS1_XGCS 6
#define LSM9DS1_MCS 5
// You can also use software SPI
//Adafruit_LSM9DS1 lsm = Adafruit_LSM9DS1(LSM9DS1_SCK, LSM9DS1_MISO, LSM9DS1_MOSI, LSM9DS1_XGCS, LSM9DS1_MCS);
// Or hardware SPI! In this case, only CS pins are passed in
//Adafruit_LSM9DS1 lsm = Adafruit_LSM9DS1(LSM9DS1_XGCS, LSM9DS1_MCS);
double min = 0, max = 0;
double input = 90; //[10] = {20,45,110,245,300,5,190,100, 80,  330};
double reset_tilt = 0;
bool tilt_in_range = false;
double tilt_angle = 0;

double tilt_offset = 0;
double error = 0.5;

void setupSensor()
{
  // 1.) Set the accelerometer range
  lsm.setupAccel(lsm.LSM9DS1_ACCELRANGE_2G);
  //lsm.setupAccel(lsm.LSM9DS1_ACCELRANGE_4G);
  //lsm.setupAccel(lsm.LSM9DS1_ACCELRANGE_8G);
  //lsm.setupAccel(lsm.LSM9DS1_ACCELRANGE_16G);
  
  // 2.) Set the magnetometer sensitivity
  lsm.setupMag(lsm.LSM9DS1_MAGGAIN_4GAUSS);
  //lsm.setupMag(lsm.LSM9DS1_MAGGAIN_8GAUSS);
  //lsm.setupMag(lsm.LSM9DS1_MAGGAIN_12GAUSS);
  //lsm.setupMag(lsm.LSM9DS1_MAGGAIN_16GAUSS);

  // 3.) Setup the gyroscope
  //lsm.setupGyro(lsm.LSM9DS1_GYROSCALE_245DPS);
  //lsm.setupGyro(lsm.LSM9DS1_GYROSCALE_500DPS);
  lsm.setupGyro(lsm.LSM9DS1_GYROSCALE_2000DPS);
}

void setup() {
  
  Serial.begin(115200);           // set up Serial library at 115200 bps

  Serial.println("LSM9DS1 data read demo");

   if (!AFMS.begin()) {         // create with the default frequency 1.6KHz
  // if (!AFMS.begin(1000)) {  // OR with a different frequency, say 1KHz
    Serial.println("Could not find Motor Shield. Check wiring.");
    while (1);
  }
  Serial.println("Motor Shield found.");
  
  // Try to initialise and warn if we couldn't detect the chip
  if (!lsm.begin())
  {
    Serial.println("Oops ... unable to initialize the LSM9DS1. Check your wiring!");
    while (1);
  }
  Serial.println("Found LSM9DS1 9DOF");

  // helper to just set the default scaling we want, see above!
  setupSensor();

  lsm.read();  /* ask it to read in the data */ 

  /* Get a new sensor event */ 
  sensors_event_t a, m, g, temp;

  lsm.getEvent(&a, &m, &g, &temp); 

  reset_tilt = atan2(a.acceleration.x, a.acceleration.y) * (180/3.141592);

  Serial.print(" Reset tilt angle:"); Serial.print(reset_tilt);   Serial.println("deg");  

  goToTiltAngle(0);
}

bool first_read = true;

void loop() {
  
  // double pan_angle;
  // double tilt_angle;

  // bool found = false;
 
  // lsm.read();  /* ask it to read in the data */ 

  // /* Get a new sensor event */ 
  // sensors_event_t a, m, g, temp;

  // lsm.getEvent(&a, &m, &g, &temp); 

  // /*Serial.print("Accel X: "); Serial.print(a.acceleration.x); Serial.print(" m/s^2");
  // Serial.print("\tY: "); Serial.print(a.acceleration.y);     Serial.print(" m/s^2 ");
  // Serial.print("\tZ: "); Serial.print(a.acceleration.z);     Serial.println(" m/s^2 ");

  // Serial.print("Gyro X: "); Serial.print(g.gyro.x);   Serial.print(" rad/s");
  // Serial.print("\tY: "); Serial.print(g.gyro.y);      Serial.print(" rad/s");
  // Serial.print("\tZ: "); Serial.print(g.gyro.z);      Serial.println(" rad/s");*/

  // // Serial.print("Mag X: "); Serial.print(m.magnetic.x);   Serial.print(" uT");
  // // Serial.print("\tY: "); Serial.print(m.magnetic.y);     Serial.print(" uT");
  // //Serial.print("\tZ: "); Serial.print(m.magnetic.z);     Serial.println(" uT");

  // tilt_angle = atan2(a.acceleration.x, a.acceleration.y) * (180/3.141592);

  // pan_angle = atan2(m.magnetic.z, m.magnetic.x)* (180/3.141592);

  // Serial.print(" Current tilt angle:"); Serial.print(tilt_angle);   Serial.println("deg"); 

  // if ((tilt_angle <= (input + 5)) && (tilt_angle >= (input - 5)))
  // {
  //   tilt_in_range = true;
  //   Serial.println(" Tilt angle in range!");
  //   yMotor->setSpeed(0);
  // }
  // else
  // {
  //   tilt_in_range = false;
  //   if(tilt_angle >= reset_tilt)
  //   {
  //     Serial.println("Need to tilt");

  //     if (tilt_angle < (input))
  //     {
  //         // move motor up if it is not in motion already
  //         if (yTime <= 0)
  //         {
  //           Serial.println("Going up");
  //           moveUp();
  //         }
  //     }
  //     else
  //     {
  //       // move motor down
  //         if (yTime <= 0)
  //         {
  //           Serial.println("Going down");
  //           moveDown();
  //         }
  //     }

  //   }
  // }

  // // If motor is running, check if time is up
  // if (yTime > 0) {
  //   yTime-= 20;
  // }
  // else {
  //   yMotor->setSpeed(0);
  // }
  // //Serial.print("Pan angle:"); Serial.print(pan_angle);   Serial.println("deg");
  // //Serial.print("Tilt angle:"); Serial.print(tilt_angle);   Serial.println("deg");  
  // delay(20);

}

void goToTiltAngle(double target) {
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
  while (abs(tilt_angle - target) > error);

  yMotor->setSpeed(0);
}

void moveUp() {
  yTime = 250;
  yMotor->run(BACKWARD);
  yMotor->setSpeed(255);
}

void moveDown() {
  yTime = 250;
  yMotor->run(FORWARD);
  yMotor->setSpeed(255);
}