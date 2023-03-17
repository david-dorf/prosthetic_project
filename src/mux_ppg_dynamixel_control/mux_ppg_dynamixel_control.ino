#include <Wire.h>
#include "MAX30105.h"
#include <Dynamixel2Arduino.h>
#define NUMBER_OF_SENSORS 2 // Number of sensors connected to the multiplexer
#define MUX_ADDR 0x70       // Default I2C address for the mux
#define DXL_SERIAL Serial1  // Serial port for the Dynamixel

// PPG sensor configuration
MAX30105 particleSensor;
float ppgIR1;
float ppgIR2;
float threshold = 1.1; // Threshold for the PPG sensor to switch gestures

// Dynamixel configuration
const int DXL_DIR_PIN = -1;
const uint8_t DXL_ID = 1;
const uint8_t DXL_ID2 = 2;
const float DXL_PROTOCOL_VERSION = 2.0;
Dynamixel2Arduino dxl(DXL_SERIAL, DXL_DIR_PIN);
using namespace ControlTableItem;

int state = 0; // Initial state of motors is open (0). Closed is (1)

void enableMuxPort(byte portNumber)
{
    if (portNumber > 7)
        portNumber = 7;
    Wire.beginTransmission(MUX_ADDR);
    Wire.requestFrom(MUX_ADDR, 1);
    if (!Wire.available())
        return; // Error
    byte settings = Wire.read();
    settings |= (1 << portNumber); // Clear the bit to disable the port
    Wire.write(settings);
    Wire.endTransmission();
}

void disableMuxPort(byte portNumber)
{
    if (portNumber > 7)
        portNumber = 7;
    Wire.beginTransmission(MUX_ADDR);
    Wire.requestFrom(MUX_ADDR, 1);
    if (!Wire.available())
        return; // Error
    byte settings = Wire.read();
    settings &= ~(1 << portNumber); // Clear the bit to disable the port
    Wire.write(settings);
    Wire.endTransmission();
}

void sensorRead()
{
    for (byte x = 0; x < NUMBER_OF_SENSORS; x++)
    {
        enableMuxPort(x);
        if (x == 0)
        {
            ppgIR1 = particleSensor.getIR();
        }
        else if (x == 1)
        {
            ppgIR2 = particleSensor.getIR();
        }
        disableMuxPort(x);
    }
}

void setup()
{
    Wire.begin();
    // Initialize the PPG sensors
    for (byte x = 0; x < NUMBER_OF_SENSORS; x++)
    {
        enableMuxPort(x); // Tell mux to connect to port X
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

    // Set max velocity. Higher values mean faster movement, except 0, which gives the maximum.
    dxl.writeControlTableItem(PROFILE_VELOCITY, DXL_ID, 120);
    dxl.writeControlTableItem(PROFILE_VELOCITY, DXL_ID2, 120);
}

void loop()
{
    if (ppgIR1 / ppgIR2 > threshold)
    {
        // Close both Dynamixels when in open state when PPG1 activated
        sensorRead();
        dxl.setGoalPosition(DXL_ID2, 90.0, UNIT_DEGREE); // close
        dxl.setGoalPosition(DXL_ID, 270.0, UNIT_DEGREE); // close
        state = 1;
    }

    else if (ppgIR2 / ppgIR1 > threshold && state == 0)
    {
        // Close Dynamixel 2 when in open state when PPG2 activated
        sensorRead();
        dxl.setGoalPosition(DXL_ID2, 90.0, UNIT_DEGREE); // close
    }

    else if (ppgIR2 / ppgIR1 > threshold && state == 1)
    {
        // Open Dynamixel 1 when in closed state when PPG1 activated
        sensorRead();
        dxl.setGoalPosition(DXL_ID, 90.0, UNIT_DEGREE); // open
    }

    else
    {
        sensorRead();
        dxl.setGoalPosition(DXL_ID, 90.0, UNIT_DEGREE);   // open
        dxl.setGoalPosition(DXL_ID2, 270.0, UNIT_DEGREE); // open
        state = 0;
    }
    delay(1);
}
