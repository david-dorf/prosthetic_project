#include <Dynamixel2Arduino.h>
#define DXL_SERIAL Serial1
#define DEBUG_SERIAL Serial

// Dynamixel configuration
const int DXL_DIR_PIN = -1;
const uint8_t DXL_ID = 1;
const uint8_t DXL_ID2 = 2;
const float DXL_PROTOCOL_VERSION = 2.0;
Dynamixel2Arduino dxl(DXL_SERIAL, DXL_DIR_PIN);

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
  dxl.setGoalPosition(DXL_ID2, 90.0, UNIT_DEGREE); // close
  delay(3000);

  dxl.setGoalPosition(DXL_ID, 270.0, UNIT_DEGREE); // close
  delay(3000);

  dxl.setGoalPosition(DXL_ID, 90.0, UNIT_DEGREE); // open
  delay(3000);

  dxl.setGoalPosition(DXL_ID2, 270.0, UNIT_DEGREE); // open
  delay(3000);
}
