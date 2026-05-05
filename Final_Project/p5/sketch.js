//
// PRESSYR - p5.js VISUAL + AUDIO SYSTEM
// Receives FSR key data from Arduino through serial communication
// Plays notes, shows visuals, and sends note names back to Arduino LCD
//

let bgImg;
let osc;
let env;

let port;
let connectBtn;
let baudrate = 9600;

let baseW = 1536;
let baseH = 1152;

let notes = [
  { name: "C4",  freq: 261.63, black: false, x: 120,  y: 820 },
  { name: "C#4", freq: 277.18, black: true,  x: 175,  y: 420 },
  { name: "D4",  freq: 293.66, black: false, x: 310,  y: 820 },
  { name: "D#4", freq: 311.13, black: true,  x: 395,  y: 420 },
  { name: "E4",  freq: 329.63, black: false, x: 500,  y: 820 },
  { name: "F4",  freq: 349.23, black: false, x: 690,  y: 820 },
  { name: "F#4", freq: 369.99, black: true,  x: 770,  y: 420 },
  { name: "G4",  freq: 392.00, black: false, x: 880,  y: 820 },
  { name: "G#4", freq: 415.30, black: true,  x: 965,  y: 420 },
  { name: "A4",  freq: 440.00, black: false, x: 1075, y: 820 },
  { name: "A#4", freq: 466.16, black: true,  x: 1200, y: 420 },
  { name: "B4",  freq: 493.88, black: false, x: 1275, y: 820 },
  { name: "C5",  freq: 523.25, black: false, x: 1460, y: 820 }
];

let balls = [];

function preload() {
  bgImg = loadImage("piano_bg.jpg");
}

function setup() {
  createCanvas(windowWidth, windowHeight);

  osc = new p5.Oscillator("triangle");
  osc.start();
  osc.amp(0);

  env = new p5.Envelope();
  env.setADSR(0.01, 0.05, 0.8, 0.2);
  env.setRange(0.5, 0);

  textFont("Arial");

  port = createSerial();

  connectBtn = createButton("Connect to Arduino");
  connectBtn.position(10, 10);
  connectBtn.mousePressed(connectToSerial);
}

function draw() {
  background(245);
  image(bgImg, 0, 0, width, height);

  let str = port.readUntil("\n");

  if (str.length > 0) {
    let keyIndex = int(trim(str));

    if (!isNaN(keyIndex)) {
      if (keyIndex === -1) {
        env.triggerRelease(osc);
      } else if (keyIndex >= 0 && keyIndex < notes.length) {
        triggerNote(keyIndex);
      }
    }
  }

  for (let i = balls.length - 1; i >= 0; i--) {
    let b = balls[i];

    b.size += 1.8;
    b.ring += 6;
    b.alpha -= 5;

    let jitterX = random(-1.5, 1.5);
    let jitterY = random(-1.5, 1.5);

    // Glow
    noStroke();
    fill(red(b.col), green(b.col), blue(b.col), b.alpha * 0.18);
    ellipse(b.x + jitterX, b.y + jitterY, b.size * 2.4);

    // Main
    fill(red(b.col), green(b.col), blue(b.col), b.alpha * 0.75);
    ellipse(b.x + jitterX, b.y + jitterY, b.size);

    // Ring
    noFill();
    strokeWeight(3);
    stroke(red(b.col), green(b.col), blue(b.col), b.alpha * 0.7);
    ellipse(b.x, b.y, b.ring);

    // -------- TEXT (UPDATED) --------
    textAlign(CENTER, CENTER);

    // outline
    stroke(0, b.alpha);
    strokeWeight(3);

    // fill
    fill(255, b.alpha);

    // dynamic size
    let tSize = b.label.includes("#") ? 18 : 22;
    textSize(tSize);

    text(b.label, b.x, b.y);

    if (b.alpha <= 0) {
      balls.splice(i, 1);
    }
  }

  drawInstructions();
}

function drawInstructions() {
  noStroke();
  fill(255, 238);
  rect(16, 60, 650, 140, 12);

  fill(0);
  textAlign(LEFT, TOP);
  textSize(14);

  text("1) Click Connect to Arduino", 28, 74);
  text("2) Press the red button to turn ON the system", 28, 96);
  text("3) LCD should show: System ON / Press pads", 28, 118);
  text("4) Press the FSR pads to play notes", 28, 140);
  text("Press F for fullscreen | Press ESC to exit fullscreen", 28, 162);
}

function mousePressed() {
  userStartAudio();
}

function triggerNote(i) {
  let note = notes[i];

  let sx = map(note.x, 0, baseW, 0, width);
  let sy = map(note.y, 0, baseH, 0, height);

  let col = note.black
    ? color(190, 80, 240)
    : color(60, 190, 255);

  osc.freq(note.freq);
  env.triggerAttack(osc);

  sendToArduino(note.name);

  balls.push({
    x: sx,
    y: sy,
    size: note.black ? 38 : 55,
    ring: note.black ? 65 : 90,
    alpha: 255,
    col: col,
    label: note.name
  });
}

function sendToArduino(message) {
  if (port.opened()) {
    port.write(message + "\n");
  }
}

function connectToSerial() {
  if (!port.opened()) {
    port.open(baudrate);
  }
}

function windowResized() {
  resizeCanvas(windowWidth, windowHeight);
}

function keyTyped() {
  if (key === 'f') toggleFullscreen();
}

function toggleFullscreen() {
  let fs = fullscreen();
  fullscreen(!fs);
}