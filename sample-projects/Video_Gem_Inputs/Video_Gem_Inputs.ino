// Video Gem by Ramona Sharples aka RMNA for Gray Area workshop
// Example code adapted from tutorials by Adafruit
// Uses a double-buffered 8-bit Adafruit_GFX-compatible framebuffer for PicoDVI.
// Animates without redraw flicker. Requires Adafruit_GFX >= 1.11.4.

#include <PicoDVI.h>
#include <math.h>
#include "Adafruit_Keypad.h"
#include <Arduino.h>

// These are some convenient values that we'll use in drawing routines later
#define W 320
#define H 240
#define HALFW 160
#define HALFH 120

// The pins the pots are connected to
#define POT_A_PIN A0
#define POT_B_PIN A1
#define POT_C_PIN A2
#define POT_D_PIN A3
#define MUX_CTRL_0 24
#define MUX_CTRL_1 25

// The number of rows and columns in the keypad
#define ROWS 4
#define COLS 4

// Declare a 320x240 8-bit (color-paletted) framebuffer
// Second argument ('true' here) enables double-buffering for flicker-free animation. 
// Third argument is a hardware configuration for Adafruit Feather RP2040 DVI.
DVIGFX8 display(DVI_RES_320x240p60, true, adafruit_feather_dvi_cfg);


// a keymap for the keypad - we'll use these numbers to identify the keys later
int keys[ROWS][COLS] = {
  {0, 1, 2, 3},
  {4, 5, 6, 7},
  {8, 9, 10, 11},
  {12, 13, 14, 15}
};
uint8_t rowPins[ROWS] = {13, 12, 11, 0};    // Connect to the row pinouts of the keypad
uint8_t colPins[COLS] = {10, 9, 6, 5};      // Connect to the column pinouts of the keypad
bool keysPressed[ROWS * COLS] = {false};    // Tracks which keys are held down
long keypressTimes[ROWS * COLS] = {0};      // Tracks when each key went down
int longPressThreshold = 300;               // ms

Adafruit_Keypad keypad = Adafruit_Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS);

int potIntervalMS = 16;
long nextPotUpdateTime = 0;
int pots[16] = {0};

int layers = 12;
int activeLayer = 0;

bool showDebug = false;

// This code runs once, and is for initializing things
void setup() {
  // Blink LED if insufficient RAM
  if (!display.begin()) {
    pinMode(LED_BUILTIN, OUTPUT);
    for (;;) digitalWrite(LED_BUILTIN, (millis() / 500) & 1);
  }

  // Start keypad monitoring
  keypad.begin();

  // Prepare our control pins to set multiplexer channels
  pinMode(MUX_CTRL_0, OUTPUT);
  pinMode(MUX_CTRL_1, OUTPUT);

  // Set a grayscale palette
  for (int i = 0; i < 256; i++) {
    display.setColor(i, i, i, i);
  }
  // ...and sync it to both framebuffers
  display.swap(false, true);
}

void loop() {
  // Update our input controls!
  keypad.tick();
  handleKeypad();

  if (millis() > nextPotUpdateTime) {
    updatePots();
    nextPotUpdateTime = millis() + potIntervalMS;
  }

  // Draw stuff! Conditionally show debug screen, otherwise normal drawing
  if (showDebug == false) {
    switch(activeLayer) {
      // This is where you should pick your pattern drawing functions and
      // put them in slots! The inputs.ino file sets the activeLayer based
      // on which buttons get pressed. Add new drawing functions in the
      // layers.ino file!
      
      case 0:
        coolRectangles();
        break;
      case 1:
        stretchyRectangles();
        break;
      case 2:
        drawRegularShapes();
        break;
      case 3:
        star();
        break;
      case 4:
        fourTriangles();
        break;
      case 5:
        slidingCircles();
        break;
      case 6:
        randomCircle();
        break;
      case 7:
        slidingLines();
        break;
      case 8:
        growingCircle();
        break;
      case 9:
        blinkyRectangles();
        break;
      case 10:
        easingCircles();
        break;
      case 11:
        spinningLines();
        break;
      default:
        drawBackground();
        break;
    }
  }
  else {
    drawBackground();
    drawDebugScreen();
  }

  // Swap front/back buffers
  // 1st parameter: copy framebuffer contents (usually should)
  // 2nd parameter: copy color palette contents (usually no need to)
  display.swap(true, false);
}
