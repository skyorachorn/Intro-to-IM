  // Exercise 03 - simple bi-directional communication
// joystick controls wind in p5
// p5 sends 1 or 0 back to control LED

int ledPin = 5;

void setup() {
  Serial.begin(9600);

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(ledPin, OUTPUT);

  // blink once to check wiring
  digitalWrite(ledPin, HIGH);
  delay(200);
  digitalWrite(ledPin, LOW);

  // start handshake
  while (Serial.available() <= 0) {
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("0");
    delay(300);
    digitalWrite(LED_BUILTIN, LOW);
    delay(50);
  }
}

void loop() {
  while (Serial.available()) {
    digitalWrite(LED_BUILTIN, HIGH);

    int ledState = Serial.parseInt();

    if (Serial.read() == '\n') {
      digitalWrite(ledPin, ledState);
    }

    // send joystick value to p5
    int joystickX = analogRead(A0);
    Serial.println(joystickX);
  }

  digitalWrite(LED_BUILTIN, LOW);
}