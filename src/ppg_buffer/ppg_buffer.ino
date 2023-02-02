#include <Wire.h>
#include "MAX30105.h"
#include <CircularBuffer.h>
#define BUFFERSIZE 10

// Initialize constants and variables
CircularBuffer<int,BUFFERSIZE> buffer; // Create a circular buffer for sensor data
int sensorValue;
MAX30105 particleSensor; // Set up the sensor configuration

void setup(){
  Serial.begin(9600);
  Serial.println("MAX30105 Basic Readings Example");
  if (particleSensor.begin() == false){
    while (1);
  }
  particleSensor.setup();
}

void loop(){
  sensorValue = particleSensor.getIR();
  buffer.push(sensorValue);
  Serial.println(buffer.first()-buffer.last());
}
