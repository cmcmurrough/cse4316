// include AccelStepper library
#include <AccelStepper.h>

// define motor controller pins
#define motorPin1 9 // IN1 on the ULN2003 driver
#define motorPin2 10 // IN2 on the ULN2003 driver
#define motorPin3 11 // IN3 on the ULN2003 driver
#define motorPin4 12 // IN4 on the ULN2003 driver

// define the AccelStepper interface type; 4 wire motor in half step mode:
#define MotorInterfaceType 8

// initialize with pin sequence IN1-IN3-IN2-IN4 for using the AccelStepper library with 28BYJ-48 stepper motor:
AccelStepper stepper = AccelStepper(MotorInterfaceType, motorPin1, motorPin3, motorPin2, motorPin4);
void setup() {
    // set the motor speed / acceleration values
    stepper.setMaxSpeed(1000);
    stepper.setAcceleration(200);
}

void loop() {
    // Set the current position to 0:
    stepper.setCurrentPosition(0);

    while (true) 
    {
        // move the motor in the forward direction until desired position is reached
        stepper.moveTo(4096);
        stepper.runToPosition();

        // pause for 1 second
        delay(1000);

        // move the motor in the reverse direction until the initial position is reached
        stepper.moveTo(0);
        stepper.runToPosition();

        // pause for 1 second
        delay(1000);
    }
}
