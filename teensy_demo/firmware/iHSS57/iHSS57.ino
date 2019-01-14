/***********************************************************************************************************************
 * @FILE iHSS57.ino
 * @BRIEF An example Arduino sketch for controlling the iHSS57 closed loop stepper
 * @AUTHOR Christopher D. McMurrough
 **********************************************************************************************************************/

// inlcude necessary header files
#include <AccelStepper.h>

// define GPIO pins
const int LED_PIN = 13;
const int LED_ON = HIGH;
const int LED_OFF = LOW;
const int STEPPER_ENABLED = HIGH;
const int STEPPER_DISABLED = LOW;
const int STEPPER_COMMON_PIN = 19;
const int STEPPER_1_STEP_PIN = 21;
const int STEPPER_1_DIR_PIN = 20;
const int STEPPER_1_ENABLE_PIN = 23;

AccelStepper Stepper_1(1, STEPPER_1_STEP_PIN, STEPPER_1_DIR_PIN);

void setup()
{
  // initialize the LED
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LED_OFF);

  // flash the LED 
  for (int i = 0; i < 25; i++)
  {
    digitalWrite(LED_PIN, LED_ON);
    delay(50);
    digitalWrite(LED_PIN, LED_OFF);
    delay(50);
  }

  // initialize the motor
  pinMode(STEPPER_COMMON_PIN, OUTPUT);
  digitalWrite(STEPPER_COMMON_PIN, HIGH);

  // enable the motor
  Stepper_1.enableOutputs();

  // set the speed command
  Stepper_1.setMaxSpeed(50000);
  Stepper_1.setSpeed(1000);
}

void loop()
{
  Stepper_1.runSpeed();
}
