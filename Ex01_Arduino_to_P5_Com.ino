/*
 * Week 11 Exercise 01
 * Arduino -> p5 communication
 * Pair with https://editor.p5js.org/po2127/sketches/y159RDHrv
 * Input:
 *   - A1 - 10k potentiometer connected to 5V and GND
 *
 * Sends one value to p5 so the ellipse can move horizontally.
 
*/

int interval = 100;
int lastMessageTime = 0;

void setup() {
  Serial.begin(9600); // initialize serial communication
}

void loop() {
  // read potentiometer value from A1
  int potentiometer = analogRead(A1);

  // map the value from 0-1023 to 0-255
  int mappedPotValue = map(potentiometer, 0, 1023, 0, 255);

  // send value to p5 as a string with newline
  Serial.println(mappedPotValue);

  // small delay for stable readings
  delay(100);
}