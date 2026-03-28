int sensorPin = 2;

// green LEDs
int greenPins[] = {3,4,5,6,7,8,9};

// red LEDs
int redPins[] = {10,11,12,13,A0,A1,A2};

void setup() {
  pinMode(sensorPin, INPUT);

  for(int i=0;i<7;i++){
    pinMode(greenPins[i], OUTPUT);
    pinMode(redPins[i], OUTPUT);
  }
}

void loop() {
  int state = digitalRead(sensorPin);

  if(state == HIGH){
    // Normal face → Green LED lip 
    for(int i=0;i<7;i++){
      digitalWrite(greenPins[i], HIGH);
      digitalWrite(redPins[i], LOW);
    }
  } else {
    // Angry face → Red LED lip
    for(int i=0;i<7;i++){
      digitalWrite(greenPins[i], LOW);
      digitalWrite(redPins[i], HIGH);
    }
  }
}