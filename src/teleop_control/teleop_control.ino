#include <Dynamixel2Arduino.h>
#define DXL_SERIAL Serial1
#define DEBUG_SERIAL Serial

// Dynamixel configuration
const int DXL_DIR_PIN = -1;
const uint8_t DXL_ID = 1;
const uint8_t DXL_ID2 = 2;
const float DXL_PROTOCOL_VERSION = 2.0;
Dynamixel2Arduino dxl(DXL_SERIAL, DXL_DIR_PIN);

// Initialize states
bool servo1_closed = false;
bool servo2_closed = false;

// Use the Control Table namespace
using namespace ControlTableItem;

void setup()
{

  // Set DYNAMIXEL baud rate
  dxl.begin(57600);

  // Set Port Protocol Version
  dxl.setPortProtocolVersion(DXL_PROTOCOL_VERSION);

  // Get DYNAMIXEL information
  dxl.ping(DXL_ID);
  dxl.ping(DXL_ID2);

  // Set operating mode to position mode
  dxl.torqueOff(DXL_ID);
  dxl.setOperatingMode(DXL_ID, OP_POSITION);
  dxl.torqueOn(DXL_ID);
  dxl.torqueOff(DXL_ID2);
  dxl.setOperatingMode(DXL_ID2, OP_POSITION);
  dxl.torqueOn(DXL_ID2);

  // Max velocity is the third argument. Higher values give higher speed limits except 0, which gives the maximum.
  dxl.writeControlTableItem(PROFILE_VELOCITY, DXL_ID, 150);
  dxl.writeControlTableItem(PROFILE_VELOCITY, DXL_ID2, 150);
}

void loop()
{
  if (Serial.available() > 0)
  {
    char inChar = Serial.read();
    if (inChar == 'a') // if 'a' is pressed, close both servos
    {
      dxl.setGoalPosition(DXL_ID2, 90.0, UNIT_DEGREE); // close
      dxl.setGoalPosition(DXL_ID, 270.0, UNIT_DEGREE); // close
      servo1_closed = true;
      servo2_closed = true;
    }
    else if (inChar == 'b') // if 'b' is pressed, open both servos
    {
      dxl.setGoalPosition(DXL_ID2, 270.0, UNIT_DEGREE); // open
      dxl.setGoalPosition(DXL_ID, 90.0, UNIT_DEGREE);   // open
      servo1_closed = false;
      servo2_closed = false;
    }
    else if (inChar == 'c') // if 'c' is pressed, close the first servo and open the second servo
    {
      dxl.setGoalPosition(DXL_ID2, 90.0, UNIT_DEGREE); // close
      dxl.setGoalPosition(DXL_ID, 90.0, UNIT_DEGREE);  // open
      servo1_closed = false;
      servo2_closed = true;
    }
    else if (inChar == 'd') // if 'd' is pressed, open the first servo and close the second servo
    {
      dxl.setGoalPosition(DXL_ID2, 270.0, UNIT_DEGREE); // open
      dxl.setGoalPosition(DXL_ID, 270.0, UNIT_DEGREE);  // close
      servo1_closed = true;
      servo2_closed = false;
    }

    // if closed, open one servo at a time
    else if (inChar == 'e' && servo1_closed && servo2_closed) // if 'e' is pressed, open the first servo
    {
      dxl.setGoalPosition(DXL_ID, 90.0, UNIT_DEGREE); // open
      servo1_closed = false;
    }
    else if (inChar == 'f' && servo1_closed && servo2_closed) // if 'f' is pressed, open the second servo
    {
      dxl.setGoalPosition(DXL_ID2, 270.0, UNIT_DEGREE); // open
      servo2_closed = false;
    }

    // close both to 180 degrees
    else if (inChar == 'g')
    {
      dxl.setGoalPosition(DXL_ID2, 180.0 - 10.0, UNIT_DEGREE); // close
      dxl.setGoalPosition(DXL_ID, 180.0, UNIT_DEGREE);         // close
      servo1_closed = true;
      servo2_closed = true;
    }

    // add slider numeric input
    else if (inChar < 270 && inChar > 90) // if a number is sent over serial, close both servos
    {
      dxl.setGoalPosition(DXL_ID2, inChar, UNIT_DEGREE); // close
      dxl.setGoalPosition(DXL_ID, inChar, UNIT_DEGREE);  // close
      servo1_closed = true;
      servo2_closed = true;
    }
  }
}
