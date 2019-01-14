/***********************************************************************************************************************
 * @FILE serial_accelstepper_control.ino
 * @BRIEF An example Arduino sketch showing USB-serial communications with the Teensy microcontroller for stepper control
 * using the AccelStepper library
 *
 * This program provides an example of USB-serial communications with the Teensy 3.1 microcontroller. The communication 
 * is based on variable width byte packets containing an error checksum. The packet structure is defined as follows:
 *
 * packet[0] = PACKET_START_BYTE (0xAA)
 * packet[1] = PACKET_SIZE (total number of bytes including overhead and payload)
 * packet[n+2] = payload byte n -> [0, PAYLOAD_SIZE - 1]
 * packet[PACKET_SIZE - 1] = packet checksum
 *
 * The checksum is computed as the XOR chain of each byte in the packet before the checksum:
 * packet[0] XOR packet[1] XOR ... XOR packet[PACKET_SIZE - 2]
 *
 * The program handles the following packets:
 * 1. Immediate step command    [0xAA][17][0x0A][motor_index][acceleration][velocity][num steps][checksum]
 *    The acceleration, velocity, and num steps fields are 4 byte each, MSB first
 * 2. Deferred step command     [0xAA][17][0x0B][motor_index][acceleration][velocity][num steps][checksum]
 *    The acceleration, velocity, and num steps fields are 4 byte each, MSB first
 * 3. Execute deferred commands [0xAA][4][0x0C][checksum]
 *    Executes all deferred commands simultaneously
 *
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
const int STEPPER_1_STEP_PIN = 23;
const int STEPPER_1_DIR_PIN = 22;
const int STEPPER_1_ENABLE_PIN = 21;
const int STEPPER_2_STEP_PIN = 20;
const int STEPPER_2_DIR_PIN = 19;
const int STEPPER_2_ENABLE_PIN = 18;
const int STEPPER_3_STEP_PIN = 17;
const int STEPPER_3_DIR_PIN = 16;
const int STEPPER_3_ENABLE_PIN = 15;
const int STEPPER_4_STEP_PIN = 2;
const int STEPPER_4_DIR_PIN = 3;
const int STEPPER_4_ENABLE_PIN = 4;
const int STEPPER_5_STEP_PIN = 5;
const int STEPPER_5_DIR_PIN = 6;
const int STEPPER_5_ENABLE_PIN = 7;
const int STEPPER_6_STEP_PIN = 8;
const int STEPPER_6_DIR_PIN = 9;
const int STEPPER_6_ENABLE_PIN = 10;

// define serial communication parameters
const unsigned long BAUD_RATE = 9600;

// define packet parameters
const byte PACKET_START_BYTE = 0xAA;
const unsigned int PACKET_OVERHEAD_BYTES = 3;
const unsigned int PACKET_MIN_BYTES = PACKET_OVERHEAD_BYTES + 1;
const unsigned int PACKET_MAX_BYTES = 255;

// define special packets
const byte STEP_COMMAND_IMMEDIATE = 0x0A;
const unsigned int STEP_COMMAND_IMMEDIATE_LENGTH = 17;
const byte STEP_COMMAND_DEFERRED = 0x0B;
const unsigned int STEP_COMMAND_DEFERRED_LENGTH = 17;
const byte STEP_COMMAND_EXECUTE = 0x0C;
const unsigned int STEP_COMMAND_EXECUTE_LENGTH = 4;

// define the stepper objects
AccelStepper Stepper_1(1, STEPPER_1_STEP_PIN, STEPPER_1_DIR_PIN);
AccelStepper Stepper_2(1, STEPPER_2_STEP_PIN, STEPPER_2_DIR_PIN);
AccelStepper Stepper_3(1, STEPPER_3_STEP_PIN, STEPPER_3_DIR_PIN);
AccelStepper Stepper_4(1, STEPPER_4_STEP_PIN, STEPPER_4_DIR_PIN);
AccelStepper Stepper_5(1, STEPPER_5_STEP_PIN, STEPPER_5_DIR_PIN);
AccelStepper Stepper_6(1, STEPPER_6_STEP_PIN, STEPPER_6_DIR_PIN);

/***********************************************************************************************************************
 * @BRIEF perform initial setup of the microcontroller
 * @AUTHOR Christoper D. McMurrough
 **********************************************************************************************************************/
void setup()
{
  // initialize the LED
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LED_OFF);

  // initialize the stepper motors
  Stepper_1.setEnablePin(STEPPER_1_ENABLE_PIN);
  Stepper_2.setEnablePin(STEPPER_2_ENABLE_PIN);
  Stepper_3.setEnablePin(STEPPER_3_ENABLE_PIN);
  Stepper_4.setEnablePin(STEPPER_4_ENABLE_PIN);
  Stepper_5.setEnablePin(STEPPER_5_ENABLE_PIN);
  Stepper_6.setEnablePin(STEPPER_6_ENABLE_PIN);
  
  // activate the stepper motors
  Stepper_1.enableOutputs();
  Stepper_2.enableOutputs();
  Stepper_3.disableOutputs();
  Stepper_4.disableOutputs();
  Stepper_5.disableOutputs();
  Stepper_6.disableOutputs();

  // initialize the serial port
  Serial.begin(BAUD_RATE);

  // flash the LED state
  for(int i = 0; i < 25; i++)
  {
    digitalWrite(LED_PIN, LED_ON);
    delay(50);
    digitalWrite(LED_PIN, LED_OFF);
    delay(50);
  }
}

/***********************************************************************************************************************
 * @BRIEF assembles and transmits a serial packet containing the given payload
 * @PARAM[in] payloadSize the size of the given payload in bytes
 * @PARAM[in] payload pointer to the data payload array
 * @RETURN true if the packet was transmitted successfully
 * @AUTHOR Christoper D. McMurrough
 **********************************************************************************************************************/
boolean sendPacket(unsigned int payloadSize, byte *payload)
{ 
  // check for max payload size
  unsigned int packetSize = payloadSize + PACKET_OVERHEAD_BYTES;
  if(packetSize > PACKET_MAX_BYTES)
  {
    return false;
  }

  // create the serial packet transmit buffer
  static byte packet[PACKET_MAX_BYTES];

  // populate the overhead fields
  packet[0] = PACKET_START_BYTE;
  packet[1] = packetSize;
  byte checkSum = packet[0] ^ packet[1];

  // populate the packet payload while computing the checksum
  for(int i = 0; i < payloadSize; i++)
  {
    packet[i + 2] = payload[i];
    checkSum = checkSum ^ packet[i + 2];
  }

  // store the checksum
  packet[packetSize - 1] = checkSum;

  // send the packet
  Serial.write(packet, packetSize);
  Serial.flush();
  return true;
}

/***********************************************************************************************************************
 * @BRIEF checks to see if the given packet is complete and valid
 * @PARAM[in] packetSize the size of the given packet buffer in bytes
 * @PARAM[in] packet pointer to the packet buffer
 * @RETURN true if the packet is valid
 * @AUTHOR Christoper D. McMurrough
 **********************************************************************************************************************/
boolean validatePacket(unsigned int packetSize, byte *packet)
{
  // check the packet size
  if(packetSize < PACKET_MIN_BYTES || packetSize > PACKET_MAX_BYTES)
  {
    return false;
  }

  // check the start byte
  if(packet[0] != PACKET_START_BYTE)
  {
    return false;
  }

  // check the length byte
  if(packet[1] != packetSize)
  {
    return false;
  }

  // compute the checksum
  byte checksum = 0x00;
  for(int i = 0; i < packetSize - 1; i++)
  {
    checksum = checksum ^ packet[i];
  }

  // check to see if the computed checksum and packet checksum are equal
  if(packet[packetSize - 1] != checksum)
  {
    return false;
  }

  // all validation checks passed, the packet is valid
  return true;
}

/***********************************************************************************************************************
 * @BRIEF main program loop
 * @AUTHOR Christoper D. McMurrough
 **********************************************************************************************************************/
void loop()
{
  // define control variables
  boolean isRunning = true;
  boolean ledState = false;

  // create the serial packet receive buffer
  static byte buffer[PACKET_MAX_BYTES];
  int count = 0;
  int packetSize = PACKET_MIN_BYTES;

  // continuously check for received packets
  while(isRunning)
  {
    // check to see if serial byte is available
    if(Serial.available())
    { 
      // get the byte
      byte b = Serial.read();

      // handle the byte according to the current count
      if(count == 0 && b == PACKET_START_BYTE)
      {
        // this byte signals the beginning of a new packet
        buffer[count] = b;
        count++;
        continue;
      }
      else if(count == 0)
      {
        // the first byte is not valid, ignore it and continue
        continue;
      }
      else if(count == 1)
      {
        // this byte contains the overall packet length
        buffer[count] = b;

        // reset the count if the packet length is not in range
        if(packetSize < PACKET_MIN_BYTES || packetSize > PACKET_MAX_BYTES)
        {
          count = 0;
        }
        else
        {
          packetSize = b;
          count++;
        }
        continue;
      }
      else if(count < packetSize)
      {
        // store the byte
        buffer[count] = b;
        count++;
      }

      // check to see if we have acquired enough bytes for a full packet
      if(count >= packetSize)
      {
        // validate the packet
        if(validatePacket(packetSize, buffer))
        {
          // change the LED state
          ledState = !ledState;
          digitalWrite(LED_PIN, ledState);

          // echo back the packet payload
          sendPacket(packetSize - PACKET_OVERHEAD_BYTES, buffer + 2);

          // handle any defined packets
          if(buffer[2] == STEP_COMMAND_IMMEDIATE && packetSize == STEP_COMMAND_IMMEDIATE_LENGTH)
          {
            // parse an immediate step command
            byte stepperID = buffer[3];
            long stepAcc = (long) buffer[4] * 16777216 + (long) buffer[5] * 65536 + (long) buffer[6] * 256 + (long) buffer[7];
            long stepVel = (long) buffer[8] * 16777216 + (long) buffer[9] * 65536 + (long) buffer[10] * 256 + (long) buffer[11];
            long stepPos = (long) buffer[12] * 16777216 + (long) buffer[13] * 65536 + (long) buffer[14] * 256 + (long) buffer[15];

            // update the target stepper motor
            if(stepperID == 0)
            {
              Stepper_1.setCurrentPosition(0); // reset current position
              Stepper_1.setAcceleration((float) stepAcc); // steps per second squared
              Stepper_1.setMaxSpeed((float) stepVel); // steps per second
              Stepper_1.runToNewPosition((float) stepPos); // steps per second
            }
            if(stepperID == 1)
            {
              Stepper_2.setCurrentPosition(0); // reset current position
              Stepper_2.setAcceleration((float) stepAcc); // steps per second squared
              Stepper_2.setMaxSpeed((float) stepVel); // steps per second
              Stepper_2.runToNewPosition((float) stepPos); // steps per second
            }
            if(stepperID == 2)
            {
              Stepper_3.setCurrentPosition(0); // reset current position
              Stepper_3.setAcceleration((float) stepAcc); // steps per second squared
              Stepper_3.setMaxSpeed((float) stepVel); // steps per second
              Stepper_3.runToNewPosition((float) stepPos); // steps per second
            }
             if(stepperID == 3)
            {
              Stepper_4.setCurrentPosition(0); // reset current position
              Stepper_4.setAcceleration((float) stepAcc); // steps per second squared
              Stepper_4.setMaxSpeed((float) stepVel); // steps per second
              Stepper_4.runToNewPosition((float) stepPos); // steps per second
            }
            if(stepperID == 4)
            {
              Stepper_5.setCurrentPosition(0); // reset current position
              Stepper_5.setAcceleration((float) stepAcc); // steps per second squared
              Stepper_5.setMaxSpeed((float) stepVel); // steps per second
              Stepper_5.runToNewPosition((float) stepPos); // steps per second
            }
            if(stepperID == 5)
            {
              Stepper_6.setCurrentPosition(0); // reset current position
              Stepper_6.setAcceleration((float) stepAcc); // steps per second squared
              Stepper_6.setMaxSpeed((float) stepVel); // steps per second
              Stepper_6.runToNewPosition((float) stepPos); // steps per second
            }      
          }
          else if(buffer[2] == STEP_COMMAND_DEFERRED && packetSize == STEP_COMMAND_DEFERRED_LENGTH)
          {
            // parse a deferred step command
            byte stepperID = buffer[3];
            long stepAcc = (long) buffer[4] * 16777216 + (long) buffer[5] * 65536 + (long) buffer[6] * 256 + (long) buffer[7];
            long stepVel = (long) buffer[8] * 16777216 + (long) buffer[9] * 65536 + (long) buffer[10] * 256 + (long) buffer[11];
            long stepPos = (long) buffer[12] * 16777216 + (long) buffer[13] * 65536 + (long) buffer[14] * 256 + (long) buffer[15];

            // update the target stepper motor
            if(stepperID == 0)
            {
              Stepper_1.setCurrentPosition(0); // reset current position
              Stepper_1.setAcceleration((float) stepAcc); // steps per second squared
              Stepper_1.setMaxSpeed((float) stepVel); // steps per second
              Stepper_1.moveTo((float) stepPos); // steps per second
            }
            else if(stepperID == 1)
            {
              Stepper_2.setCurrentPosition(0); // reset current position
              Stepper_2.setAcceleration((float) stepAcc); // steps per second squared
              Stepper_2.setMaxSpeed((float) stepVel); // steps per second
              Stepper_2.moveTo((float) stepPos); // steps per second
            }
            else if(stepperID == 2)
            {
              Stepper_3.setCurrentPosition(0); // reset current position
              Stepper_3.setAcceleration((float) stepAcc); // steps per second squared
              Stepper_3.setMaxSpeed((float) stepVel); // steps per second
              Stepper_3.moveTo((float) stepPos); // steps per second
            }
            else if(stepperID == 3)
            {
              Stepper_4.setCurrentPosition(0); // reset current position
              Stepper_4.setAcceleration((float) stepAcc); // steps per second squared
              Stepper_4.setMaxSpeed((float) stepVel); // steps per second
              Stepper_4.moveTo((float) stepPos); // steps per second
            }
            else if(stepperID == 4)
            {
              Stepper_5.setCurrentPosition(0); // reset current position
              Stepper_5.setAcceleration((float) stepAcc); // steps per second squared
              Stepper_5.setMaxSpeed((float) stepVel); // steps per second
              Stepper_5.moveTo((float) stepPos); // steps per second
            }
            else if(stepperID == 5)
            {
              Stepper_6.setCurrentPosition(0); // reset current position
              Stepper_6.setAcceleration((float) stepAcc); // steps per second squared
              Stepper_6.setMaxSpeed((float) stepVel); // steps per second
              Stepper_6.moveTo((float) stepPos); // steps per second
            }      
          }
          else if(buffer[2] == STEP_COMMAND_EXECUTE && packetSize == STEP_COMMAND_EXECUTE_LENGTH)
          {
            // execute all deferred motion commands until completion
            while(Stepper_1.distanceToGo() != 0 || Stepper_1.distanceToGo() != 0 || Stepper_1.distanceToGo() != 0 || Stepper_1.distanceToGo() != 0 || Stepper_1.distanceToGo() != 0 || Stepper_1.distanceToGo() != 0)
            {
              Stepper_1.run();
              Stepper_2.run();
              Stepper_3.run();
              Stepper_4.run();
              Stepper_5.run();
              Stepper_6.run();
            }
          }
        }

        // reset the count
        count = 0;
      }
    }
  }
}



