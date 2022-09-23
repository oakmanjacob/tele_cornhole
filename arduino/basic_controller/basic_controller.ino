/*
This is a test sketch for the Adafruit assembled Motor Shield for Arduino v2
It won't work with v1.x motor shields! Only for the v2's with built in PWM
control

For use with the Adafruit Motor Shield v2
---->  http://www.adafruit.com/products/1438
*/
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_LSM9DS1.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_MotorShield.h>

Adafruit_LSM9DS1 lsm = Adafruit_LSM9DS1();

#define LSM9DS1_SCK A5
#define LSM9DS1_MISO 12
#define LSM9DS1_MOSI A4
#define LSM9DS1_XGCS 6
#define LSM9DS1_MCS 5

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield();

// Select which 'port' M1, M2, M3 or M4. In this case, M1
Adafruit_DCMotor *xMotor = AFMS.getMotor(1);
Adafruit_DCMotor *yMotor = AFMS.getMotor(2);
char input;

int xTime = 0;
int xDirection = RELEASE;
int yTime = 0;
int yDirection = RELEASE;
int stepTime = 200;

int limitSwitch = 10;

int cannon = 8;
int cannonTime = 0;

int LEFT = FORWARD;
int RIGHT = BACKWARD;
int UP = BACKWARD;
int DOWN = FORWARD;

void setup() {
  Serial.begin(115200);           // set up Serial library at 115200 bps
  Serial.println("Adafruit Motorshield v2 - DC Motor test!");

  if (!lsm.begin())
  {
    Serial.println("Oops ... unable to initialize the LSM9DS1. Check your wiring!");
    while (1);
  }
  Serial.println("Found LSM9DS1 9DOF");
  
  lsm.setupAccel(lsm.LSM9DS1_ACCELRANGE_2G);
  lsm.setupMag(lsm.LSM9DS1_MAGGAIN_4GAUSS);
  lsm.setupGyro(lsm.LSM9DS1_GYROSCALE_2000DPS);

  pinMode(cannon, OUTPUT);
  digitalWrite(cannon, LOW);

  pinMode(limitSwitch, INPUT_PULLUP);

  if (!AFMS.begin()) {         // create with the default frequency 1.6KHz
  // if (!AFMS.begin(1000)) {  // OR with a different frequency, say 1KHz
    Serial.println("Could not find Motor Shield. Check wiring.");
    while (1);
  }
  Serial.println("Motor Shield found.");
  Serial.println("Input a direction");
}

void loop() {
  int switchValue = digitalRead(limitSwitch);
  if (xDirection == LEFT && switchValue == HIGH) {
    xMotor->setSpeed(0);
    xTime = 0;  
  }
  
  if(Serial.available()){
    input = Serial.read();
    bool found = true;
    switch (input) {
      case 'w':
        Serial.println("Going up");
        moveUp();
        break;
      case 's':
        Serial.println("Going down");
        moveDown();
        break;
      case 'd':
        Serial.println("Going right");
        moveRight();
        break;
      case 'a':
        if (switchValue == HIGH) {
          break;
        }
        Serial.println("Going left");
        moveLeft();
        break;
      case 'f':
        Serial.println("Firing");
        fire();
        break;
      case 'z':
        Serial.println("Finding Zero");
        findZero();
        break;
      default:
        found = false;
        break;
    }
    if (found) {
      Serial.println("Input a direction");
    }
  }

  if (yTime > 0) {
    yTime--;
  }
  else {
    yMotor->setSpeed(0);
  }

  if (xTime > 0) {
    xTime--;
  }
  else {
    xMotor->setSpeed(0);
  }

  if (cannonTime > 0) {
    cannonTime--;
  }
  else {
    digitalWrite(cannon, LOW);  
  }
  delay(1);
}

void fire() {
  digitalWrite(cannon, HIGH);
  cannonTime = 500;
}

void moveUp() {
  yTime = stepTime;
  yMotor->run(UP);
  yDirection = UP;
  yMotor->setSpeed(255);
}

void moveDown() {
  yTime = stepTime;
  yMotor->run(DOWN);
  yDirection = DOWN;
  yMotor->setSpeed(255);
}

void moveRight() {
  xTime = stepTime;
  xMotor->run(RIGHT);
  xDirection = RIGHT;
  xMotor->setSpeed(255);
}

void moveLeft() {
  xTime = stepTime;
  xMotor->run(LEFT);
  xDirection = LEFT;
  xMotor->setSpeed(255);
}

void clearAll() {
    xTime = 0;
    yTime = 0;
    xMotor->setSpeed(0);  
    yMotor->setSpeed(0);
}

void findZero() {
  clearAll();
  xMotor->run(LEFT);
  xMotor->setSpeed(100);

  while (digitalRead(limitSwitch) == LOW) {
    delay(1);  
  }

  while(Serial.available()){
    Serial.read();
  }
  xMotor->setSpeed(0);
}
