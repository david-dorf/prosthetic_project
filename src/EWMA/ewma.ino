int prevValue;
// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input on analog pin 0:
  int sensorValue = pow(analogRead(A0)-300,2);
  int smoothValue = 0.8 * sensorValue + 0.2 * prevValue;
  int prevValue = smoothValue;
  // print out the value you read:
  Serial.println(smoothValue);
  delay(1);  // delay in between reads for stability
}
