#include <Wire.h>
#include "MAX30105.h"

MAX30105 particleSensor;

void setup(){
  Serial.begin(9600);
  Serial.println("MAX30105 Basic Readings Example");
  if (particleSensor.begin() == false){
    while (1);
  }
  particleSensor.setup();
}

void loop(){
  int ppgIR = particleSensor.getIR();
  Serial.print(ppgIR+25000);
  Serial.print(",");
  Serial.print(analogRead(A0));
  Serial.println();
}
