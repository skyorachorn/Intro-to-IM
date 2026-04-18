// Week 11 Exercise 02 - p5 to Arduino communication

int ledPin = 5;

void setup() {
  // Start serial communication
  Serial.begin(9600);

  // Built-in LED as status output
  pinMode(LED_BUILTIN, OUTPUT);

  // Output LED
  pinMode(ledPin, OUTPUT);

  // Blink once to check wiring
  digitalWrite(ledPin, HIGH);
  delay(200);
  digitalWrite(ledPin, LOW);

  // Start the handshake
  while (Serial.available() <= 0) {
    digitalWrite(LED_BUILTIN, HIGH); // blink while waiting for serial data
    Serial.println("0");             // send a starting message
    delay(300);
    digitalWrite(LED_BUILTIN, LOW);
    delay(50);
  }
}

void loop() {
  // Wait for data from p5 before doing something
  while (Serial.available()) {
    digitalWrite(LED_BUILTIN, HIGH); // LED on while receiving data

    int brightness = Serial.parseInt();

    if (Serial.read() == '\n') {
      analogWrite(ledPin, brightness);
    }
  }

  digitalWrite(LED_BUILTIN, LOW);
}
