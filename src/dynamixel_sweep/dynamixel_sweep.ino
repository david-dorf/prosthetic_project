#include <Dynamixel2Arduino.h>
#define DXL_SERIAL Serial1
#define DEBUG_SERIAL Serial

// Dynamixel configuration
const int DXL_DIR_PIN = -1;
const uint8_t DXL_ID = 1;
const float DXL_PROTOCOL_VERSION = 2.0;
Dynamixel2Arduino dxl(DXL_SERIAL, DXL_DIR_PIN);

// Use the Control Table namespace
using namespace ControlTableItem;

void setup() {
  // Set serial baud rate for the OpenRB-150
  DEBUG_SERIAL.begin(115200);
  while(!DEBUG_SERIAL);

  // Set DYNAMIXEL baud rate
  dxl.begin(57600);
  // Set Port Protocol Version
  dxl.setPortProtocolVersion(DXL_PROTOCOL_VERSION);
  // Get DYNAMIXEL information
  dxl.ping(DXL_ID);

  // Set operating mode to position mode
  dxl.torqueOff(DXL_ID);
  dxl.setOperatingMode(DXL_ID, OP_POSITION);
  dxl.torqueOn(DXL_ID);

  // Max velocity is the third argument. Higher values give higher speed limits except 0, which gives the maximum.
  dxl.writeControlTableItem(PROFILE_VELOCITY, DXL_ID, 50);
}

void loop() {
  float i_present_position = 0; // Initialize position variable

  dxl.setGoalPosition(DXL_ID, 270.0, UNIT_DEGREE); // Set start and end positions, with a delay until they reach that position
  delay(3000);

  dxl.setGoalPosition(DXL_ID, 90.0, UNIT_DEGREE);
  delay(3000);
}
