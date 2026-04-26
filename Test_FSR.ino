const int SIG_PIN = A0;
const int S0_PIN = 2;
const int S1_PIN = 3;
const int S2_PIN = 4;
const int S3_PIN = 5;

void setup() {
  Serial.begin(9600);

  pinMode(S0_PIN, OUTPUT);
  pinMode(S1_PIN, OUTPUT);
  pinMode(S2_PIN, OUTPUT);
  pinMode(S3_PIN, OUTPUT);
}

void loop() {
  for (int channel = 0; channel < 13; channel++) {
    int value = readMux(channel);

    Serial.print("CH");
    Serial.print(channel);
    Serial.print(":");
    Serial.print(value);
    Serial.print("   ");
  }
  Serial.println();

  delay(200);
}

int readMux(int channel) {
  digitalWrite(S0_PIN, bitRead(channel, 0));
  digitalWrite(S1_PIN, bitRead(channel, 1));
  digitalWrite(S2_PIN, bitRead(channel, 2));
  digitalWrite(S3_PIN, bitRead(channel, 3));

  delay(5);

  return analogRead(SIG_PIN);
}