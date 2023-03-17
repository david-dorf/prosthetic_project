#include <Wire.h>
#include "MAX30105.h"

MAX30105 particleSensor;    // Set up the sensor configuration
#define NUMBER_OF_SENSORS 2 // Number of sensors to use with the multiplexer
#define MUX_ADDR 0x70       // Default I2C address for the mux

int ppgIR1;
int ppgIR2;
int smoothValue1;
int smoothValue2;
int sensorPrevious1;
int sensorPrevious2;

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

void setup()
{
    Serial.begin(9600);
    Serial.println("Qwiic Mux Shield Read Example");
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

    Serial.println("Mux Shield online");
}

void loop()
{
    for (byte x = 0; x < NUMBER_OF_SENSORS; x++)
    {
        enableMuxPort(x);
        if (x == 0)
        {
            ppgIR1 = particleSensor.getIR();
            // smoothValue1 = 0.7 * ppgIR1 + 0.3 * sensorPrevious1;
            // sensorPrevious1 = smoothValue1;
            // Serial.print((float)smoothValue1/smoothValue2);
            Serial.println((float)ppgIR1/ppgIR2);
        }
        else if (x == 1)
        {
            ppgIR2 = particleSensor.getIR();
            Serial.println((float)ppgIR1/ppgIR2);
        }
        disableMuxPort(x);
    }
    delay(1);
}
