#include <Dynamixel2Arduino.h>
#include <Wire.h>
#include "MAX30105.h"
#include <CircularBuffer.h>
#include <SparkFun_I2C_Mux_Arduino_Library.h> //Click here to get the library: http://librarymanager/All#SparkFun_I2C_Mux
QWIICMUX myMux;
#define DXL_SERIAL Serial1
#define BUFFERSIZE 10 // Size of the buffer for the sensor data
#define SENSORSIZE 2  // Number of sensors to use with the multiplexer

// Initialize constants and variables
CircularBuffer<int, BUFFERSIZE> buffer; // Create a circular buffer for sensor data
int sensorValue;
int smoothValue;
int sensorPrevious;
int bufferValue;
MAX30105 particleSensor; // Set up the sensor configuration

// Dynamixel configuration
const int DXL_DIR_PIN = -1;
const uint8_t DXL_ID = 1;
const uint8_t DXL_ID2 = 2;
const float DXL_PROTOCOL_VERSION = 2.0;
Dynamixel2Arduino dxl(DXL_SERIAL, DXL_DIR_PIN);

using namespace ControlTableItem;

void setup()
{
  Wire.begin(); // Start the I2C bus

  for (byte x = 0; x < SENSORSIZE; x++)
  {
    enableMuxPort(x);
    if (particleSensor.begin() == false)
    {
      while (1)
        ;
    }
    particleSensor.setup();
    disableMuxPort(x);
  }

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
  dxl.writeControlTableItem(PROFILE_VELOCITY, DXL_ID, 50);
  dxl.writeControlTableItem(PROFILE_VELOCITY, DXL_ID2, 50);
}

void loop()
{
  for (byte x = 0; x < SENSORSIZE; x++)
  {
    enableMuxPort(x); // Select port on the multiplexer

    // Read the PPG sensor and smooth the data, then push it to the buffer
    sensorValue = particleSensor.getIR();
    smoothValue = 0.8 * sensorValue + 0.2 * sensorPrevious;
    sensorPrevious = smoothValue;
    buffer.push(smoothValue);
    bufferValue = buffer.first() - buffer.last();

    // Close full hand
    if (bufferValue < -5000)
    {
      dxl.setGoalPosition(DXL_ID, 270.0, UNIT_DEGREE);
      dxl.setGoalPosition(DXL_ID2, 270.0, UNIT_DEGREE);
      // Wait for the contraction to end
      while (dxl.getPresentPosition(DXL_ID, UNIT_DEGREE) < 250)
      { // wait until the servo has reached the goal position
        sensorValue = particleSensor.getIR();
        smoothValue = 0.8 * sensorValue + 0.2 * sensorPrevious;
        sensorPrevious = smoothValue;
        buffer.push(smoothValue);
        bufferValue = buffer.first() - buffer.last();
      }
    }

    // Open full hand
    else if (bufferValue > 5000)
    {
      dxl.setGoalPosition(DXL_ID, 90.0, UNIT_DEGREE);
      dxl.setGoalPosition(DXL_ID2, 90.0, UNIT_DEGREE);
      while (dxl.getPresentPosition(DXL_ID, UNIT_DEGREE) > 100)
      { // wait until the servo has reached the goal position
        sensorValue = particleSensor.getIR();
        smoothValue = 0.8 * sensorValue + 0.2 * sensorPrevious;
        sensorPrevious = smoothValue;
        buffer.push(smoothValue);
        bufferValue = buffer.first() - buffer.last();
      }
    }

    disableMuxPort(x); // Disable the port on the multiplexer
  }
}
