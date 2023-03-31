#include <Wire.h>
#include "MAX30105.h"

MAX30105 particleSensor;

void setup()
{
  Serial.begin(115200);
  Serial.println("MAX30105 Basic Readings Example");
  if (particleSensor.begin() == false)
  {
    while (1)
      ;
  }
  particleSensor.setup();
}

void loop()
{
  int ppgIR = particleSensor.getIR()-110000;
  Serial.println(ppgIR);
}
