//
// PRESSYR - Arduino FSR Piano System
// Reads FSR inputs through CD74HC4067 multiplexer
// Sends note index to p5.js through serial communication
// Receives note names back from p5.js for LCD display
//

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// -------- LCD setup --------
// I2C LCD address: 0x27
LiquidCrystal_I2C lcd(0x27, 16, 2);

String incoming = "";

// -------- CD74HC4067 multiplexer pins --------
// SIG sends analog data from selected channel
const int SIG_PIN = A0;

const int S0_PIN = 2;
const int S1_PIN = 3;
const int S2_PIN = 4;
const int S3_PIN = 5;

// -------- Arcade switch --------
// Controls system ON/OFF state
const int SWITCH_PIN = 7;

// -------- Arcade LED --------
// LED turns on when system is active
const int LED_PIN = 8;

// -------- System settings --------
const int NUM_KEYS = 13;
const int THRESHOLD = 95;
const int READ_DELAY = 2;

// If no new note is played for 3 seconds,
// LCD returns to the default ON instruction screen
const unsigned long LCD_RESET_TIME = 3000;
unsigned long lastNoteTime = 0;
bool lcdShowingNote = false;

// Stores whether each key is currently pressed
// Prevents repeated triggering while holding one key
bool keyPressedState[NUM_KEYS] = {false};

// Tracks whether piano system is enabled
bool systemOn = true;

void setup() {
  Serial.begin(9600);

  // Configure multiplexer select pins
  pinMode(S0_PIN, OUTPUT);
  pinMode(S1_PIN, OUTPUT);
  pinMode(S2_PIN, OUTPUT);
  pinMode(S3_PIN, OUTPUT);

  // Configure arcade switch and LED
  pinMode(SWITCH_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);

  // Start with system ON
  digitalWrite(LED_PIN, HIGH);

  // Initialize LCD
  lcd.init();
  lcd.backlight();

  showSystemOnScreen();
}

void loop() {
  // Read arcade switch
  // LOW means pressed because INPUT_PULLUP is used
  if (digitalRead(SWITCH_PIN) == LOW) {
    systemOn = !systemOn;

    if (systemOn) {
      digitalWrite(LED_PIN, HIGH);
      showSystemOnScreen();
    } else {
      digitalWrite(LED_PIN, LOW);

      // Tell p5 to fade out any currently playing sound
      Serial.println(-1);

      // Reset all key states when system is turned off
      for (int i = 0; i < NUM_KEYS; i++) {
        keyPressedState[i] = false;
      }

      showSystemOffScreen();
    }

    // Simple debounce delay
    delay(300);
  }

  // Stop reading piano keys if system is OFF
  if (!systemOn) {
    return;
  }

  // Receive note names from p5.js
  // Used for LCD note visualization
  readFromP5();

  // If LCD is showing an old note for too long,
  // return to the default instruction screen
  if (lcdShowingNote && millis() - lastNoteTime > LCD_RESET_TIME) {
    showSystemOnScreen();
  }

  // Scan all FSR channels through multiplexer
  for (int channel = 0; channel < NUM_KEYS; channel++) {
    int value = readMux(channel);

    // Trigger note only when pressure rises above threshold.
    // +5 helps reduce accidental noise triggering.
    if (value > THRESHOLD + 5 && !keyPressedState[channel]) {
      Serial.println(channel);
      keyPressedState[channel] = true;
    }

    // Detect release when pressure drops back below threshold
    if (value <= THRESHOLD && keyPressedState[channel]) {
      Serial.println(-1);
      keyPressedState[channel] = false;
    }
  }

  delay(2);
}

// Read one channel from CD74HC4067 multiplexer
int readMux(int channel) {
  digitalWrite(S0_PIN, bitRead(channel, 0));
  digitalWrite(S1_PIN, bitRead(channel, 1));
  digitalWrite(S2_PIN, bitRead(channel, 2));
  digitalWrite(S3_PIN, bitRead(channel, 3));

  // Allow analog signal to stabilize
  delay(READ_DELAY);

  return analogRead(SIG_PIN);
}

// Read incoming note names from p5.js
void readFromP5() {
  while (Serial.available()) {
    char c = Serial.read();

    if (c == '\n') {
      updateLCD(incoming);
      incoming = "";
    } else {
      incoming += c;
    }
  }
}

// Default ON screen
void showSystemOnScreen() {
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("System ON");

  lcd.setCursor(0, 1);
  lcd.print("Press pads");

  lcdShowingNote = false;
}

// Default OFF screen
void showSystemOffScreen() {
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("System OFF");

  lcd.setCursor(0, 1);
  lcd.print("Press RED Btn");

  lcdShowingNote = false;
}

// Display note names on LCD
// Notes are positioned similarly to a piano keyboard layout
void updateLCD(String message) {
  lcd.clear();

  // White keys shown on bottom row
  if (message == "C4") {
    lcd.setCursor(0, 1);
  } else if (message == "D4") {
    lcd.setCursor(2, 1);
  } else if (message == "E4") {
    lcd.setCursor(4, 1);
  } else if (message == "F4") {
    lcd.setCursor(6, 1);
  } else if (message == "G4") {
    lcd.setCursor(8, 1);
  } else if (message == "A4") {
    lcd.setCursor(10, 1);
  } else if (message == "B4") {
    lcd.setCursor(12, 1);
  } else if (message == "C5") {
    lcd.setCursor(14, 1);
  }

  // Black keys shown on top row
  else if (message == "C#4") {
    lcd.setCursor(1, 0);
  } else if (message == "D#4") {
    lcd.setCursor(3, 0);
  } else if (message == "F#4") {
    lcd.setCursor(7, 0);
  } else if (message == "G#4") {
    lcd.setCursor(9, 0);
  } else if (message == "A#4") {
    lcd.setCursor(11, 0);
  }

  lcd.print(message);

  lastNoteTime = millis();
  lcdShowingNote = true;
}