/*
This is a test sketch for the Adafruit assembled Motor Shield for Arduino v2
It won't work with v1.x motor shields! Only for the v2's with built in PWM
control

For use with the Adafruit Motor Shield v2
---->  http://www.adafruit.com/products/1438
*/

#include <Adafruit_MotorShield.h>

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield();

// Select which 'port' M1, M2, M3 or M4. In this case, M1
Adafruit_DCMotor *xMotor = AFMS.getMotor(1);
Adafruit_DCMotor *yMotor = AFMS.getMotor(2);
char input;

int xTime = 0;
int yTime = 0;

void setup() {
  Serial.begin(115200);           // set up Serial library at 115200 bps
  Serial.println("Adafruit Motorshield v2 - DC Motor test!");

  if (!AFMS.begin()) {         // create with the default frequency 1.6KHz
  // if (!AFMS.begin(1000)) {  // OR with a different frequency, say 1KHz
    Serial.println("Could not find Motor Shield. Check wiring.");
    while (1);
  }
  Serial.println("Motor Shield found.");
  Serial.println("Input a direction");
}

void loop() {
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
        Serial.println("Going left");
        moveLeft();
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
  delay(1);
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

void moveRight() {
  xTime = 250;
  xMotor->run(BACKWARD);
  xMotor->setSpeed(255);
}

void moveLeft() {
  xTime = 250;
  xMotor->run(FORWARD);
  xMotor->setSpeed(255);
}
