#include "pitches.h"   // include note frequencies

// ---------- Pins ----------
const int FSR_PINS[4] = {A0, A1, A2, A3};   // 4 FSR sensors

const int YELLOW_LED = 2;   // system ON indicator
const int GREEN_LED = 5;    // PWM LED (analog output)
const int RED_LED = 4;      // hard press indicator
const int BUZZER = 6;       // sound output
const int SWITCH_PIN = 7;   // on/off switch

// ---------- Notes (4 keys × 3 pressure levels) ----------
const int NOTES[4][3] = {
  {NOTE_C5, NOTE_D6, NOTE_E6},  // FSR1
  {NOTE_D5, NOTE_E6, NOTE_F6},  // FSR2
  {NOTE_E5, NOTE_G5, NOTE_D6},  // FSR3
  {NOTE_F5, NOTE_A5, NOTE_A6}   // FSR4
};

// ---------- Thresholds ----------
const int PRESS_THRESHOLD = 100;   // ignore small noise
const int LEVEL_1_MAX = 350;       // light press
const int LEVEL_2_MAX = 650;       // medium press
const int HARD_THRESHOLD = 650;    // red LED threshold
const int MAX_PRESS = 900;         // max expected pressure

// ---------- millis (non-blocking timing) ----------
unsigned long lastScan = 0;            // last read time
const unsigned long scanInterval = 8;  // read every 8 ms

// ---------- smoothing ----------
int smoothVals[4] = {0, 0, 0, 0};      // store smoothed sensor values

// ---------- note tracking ----------
int currentNote = 0;                   // current playing note

void setup() {
  pinMode(YELLOW_LED, OUTPUT);   // set yellow LED
  pinMode(GREEN_LED, OUTPUT);    // set green LED
  pinMode(RED_LED, OUTPUT);      // set red LED
  pinMode(BUZZER, OUTPUT);       // set buzzer
  pinMode(SWITCH_PIN, INPUT);    // switch (external pull-down)
}

void loop() {

  // ---------- Timing control ----------
  if (millis() - lastScan < scanInterval) {
    return;   // skip if too fast
  }
  lastScan = millis();

  // ---------- Read switch ----------
  int switchState = digitalRead(SWITCH_PIN);

  // ---------- System OFF ----------
  if (switchState == LOW) {

    digitalWrite(YELLOW_LED, LOW);   // turn off yellow LED
    digitalWrite(RED_LED, LOW);      // turn off red LED
    analogWrite(GREEN_LED, 0);       // turn off green LED
    noTone(BUZZER);                  // stop sound
    currentNote = 0;                 // reset note

    return;   // stop rest of loop
  }

  // ---------- System ON ----------
  digitalWrite(YELLOW_LED, HIGH);    // turn on yellow LED

  // ---------- Read + smooth sensors ----------
  for (int i = 0; i < 4; i++) {
    int raw = readAverage(FSR_PINS[i]);   // read sensor
    smoothVals[i] = (smoothVals[i] * 3 + raw) / 4;   // smoothing
  }

  // ---------- Find strongest press ----------
  int strongestIndex = 0;
  int strongestValue = smoothVals[0];

  for (int i = 1; i < 4; i++) {
    if (smoothVals[i] > strongestValue) {
      strongestValue = smoothVals[i];
      strongestIndex = i;
    }
  }

  // ---------- No press ----------
  if (strongestValue < PRESS_THRESHOLD) {

    analogWrite(GREEN_LED, 0);      // green LED off
    digitalWrite(RED_LED, LOW);     // red LED off
    noTone(BUZZER);                 // stop sound
    currentNote = 0;                // reset note

    return;
  }

  // ---------- Determine pressure level ----------
  int level;

  if (strongestValue <= LEVEL_1_MAX) {
    level = 0;   // light press
  } 
  else if (strongestValue <= LEVEL_2_MAX) {
    level = 1;   // medium press
  } 
  else {
    level = 2;   // hard press
  }

  // ---------- Select note ----------
  int nextNote = NOTES[strongestIndex][level];

  // ---------- Play sound ----------
  if (nextNote != currentNote) {
    tone(BUZZER, nextNote);   // play note
    currentNote = nextNote;   // update
  }

  // ---------- Analog output (PWM LED) ----------
  int clipped = constrain(strongestValue, PRESS_THRESHOLD, MAX_PRESS);  // limit range
  int brightness = map(clipped, PRESS_THRESHOLD, MAX_PRESS, 0, 255);    // map to PWM
  analogWrite(GREEN_LED, brightness);   // set brightness

  // ---------- Red LED (hard press) ----------
  if (strongestValue > HARD_THRESHOLD) {
    digitalWrite(RED_LED, HIGH);   // turn on red LED
  } else {
    digitalWrite(RED_LED, LOW);    // turn off red LED
  }
}

// ---------- Helper: average reading ----------
int readAverage(int pin) {
  long sum = 0;

  for (int i = 0; i < 5; i++) {
    sum += analogRead(pin);   // read multiple times
  }

  return sum / 5;   // return average
}