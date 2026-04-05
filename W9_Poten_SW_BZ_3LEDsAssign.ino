const int YELLOW_LED = 2;
const int GREEN_LED = 5;   // PWM pin
const int RED_LED = 3;
const int BUZZER = 6;
const int SWITCH_PIN = 7;
const int POT_PIN = A0;

int switchState = 0;
int potValue = 0;
int brightness = 0;

const int THRESHOLD = 700;   // change to red LED + buzzer

void setup() {
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(SWITCH_PIN, INPUT);   
}

void loop() {
  switchState = digitalRead(SWITCH_PIN);

  // if s/w OFF -> no light
  if (switchState == LOW) {
    digitalWrite(YELLOW_LED, LOW);
    analogWrite(GREEN_LED, 0);
    digitalWrite(RED_LED, LOW);
    noTone(BUZZER);
  }

  // if s/w ON -> yellow LED light show digital mode and read potentiometer
  else {
    digitalWrite(YELLOW_LED, HIGH);

    potValue = analogRead(POT_PIN);

    if (potValue < THRESHOLD) {
      // analog mode: dimmable green LED
      brightness = map(potValue, 0, THRESHOLD, 0, 255);
      analogWrite(GREEN_LED, brightness);

      digitalWrite(RED_LED, LOW);
      noTone(BUZZER);
    } 
    else {
      // if >= threshold -> green LED off, red LED on, buzzer sounds
      analogWrite(GREEN_LED, 0);
      digitalWrite(RED_LED, HIGH);
      tone(BUZZER, 1000);
    }
  }

  delay(10);
}
