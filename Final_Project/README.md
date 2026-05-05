# Final Project – Pressure-Based Digital Piano

## Concept
This project is a pressure-based digital piano using Arduino and p5.js.  
Users can press FSR sensors to play different notes based on pressure levels.  
The goal is to combine physical interaction with digital feedback.

## System Overview
- Arduino reads multiple FSR sensors through a CD74HC4067 multiplexer  
- Data is sent to p5.js via serial communication  
- p5.js visualizes notes and interaction in real-time  
- LCD displays system status and note names  

## Interaction
1. Click "Connect to Arduino"  
2. Press the red button to turn ON the system  
3. LCD shows: "System ON / Press pads"  
4. Press FSR pads to play notes  

## Files Structure
- /arduino/Final_R5/Final_R5.ino → Arduino code  
- /p5/ → p5.js interface (visual + interaction)

## Technologies Used
- Arduino (C++)  
- p5.js  
- Web Serial API  
- FSR sensors  
- Multiplexer (CD74HC4067)  

## Notes
This project focuses on interaction design, combining hardware input and visual feedback.
