// Video Gem by Ramona Sharples aka RMNA for Gray Area workshop
// Example code adapted from tutorials by Adafruit
// Uses a double-buffered 8-bit Adafruit_GFX-compatible framebuffer for PicoDVI.
// Animates without redraw flicker. Requires Adafruit_GFX >= 1.11.4.

#include <PicoDVI.h>
#include <math.h>
#include <Arduino.h>

// These are some convenient values that we'll use in drawing routines later
#define W 320
#define H 240
#define HALFW 160
#define HALFH 120

// Declare a 320x240 8-bit (color-paletted) framebuffer
// Second argument ('true' here) enables double-buffering for flicker-free animation. 
// Third argument is a hardware configuration for Adafruit Feather RP2040 DVI.
DVIGFX8 display(DVI_RES_320x240p60, true, adafruit_feather_dvi_cfg);

// This code runs once, and is for initializing things
void setup() {
  // Blink LED if insufficient RAM
  if (!display.begin()) {
    pinMode(LED_BUILTIN, OUTPUT);
    for (;;) digitalWrite(LED_BUILTIN, (millis() / 500) & 1);
  }

  // Set a grayscale palette
  for (int i = 0; i < 256; i++) {
    display.setColor(i, i, i, i);
  }
  // ...and sync it to both framebuffers
  display.swap(false, true);
}

void loop() {
  // Clear the canvas first... what happens if you delete this??
  display.fillScreen(0);

  // Your drawing functions go here! Define them below!
  nestedCircles();
  // bubbles();
  // eye();
  // mountains();
  // nestedRoundRects();
  // fannedLines();
  // rectangleGrid();
  // pixelGrid();
  // mystery1();
  // mystery2();
  // mystery3();

  // Swap front/back buffers
  // 1st parameter: copy framebuffer contents (usually should)
  // 2nd parameter: copy color palette contents (usually no need to)
  display.swap(true, false);
}

// Add some functions here, then call them in the loop function above!

// SHAPE
void nestedCircles() {
  display.fillScreen(0);

  int cx = 160;
  int cy = 120;

  // drawCircle(centerX, centerY, radius, color)
  for (int i = 1; i <= 8; i++) {
    int r     = i * 13;
    int color = 30 + i * 26;
    display.drawCircle(cx, cy, r, color);
  }
}

void bubbles() {
  display.fillScreen(10);

  // fillCircle(centerX, centerY, radius, color)
  display.fillCircle(120, 130,  70,  40);   // large, dark
  display.fillCircle(200, 100,  55,  80);   // medium
  display.fillCircle(160, 160,  40, 140);   // smaller, brighter
  display.fillCircle(230, 150,  30, 190);   // small, bright
  display.fillCircle(180, 120,  18, 240);   // tiny highlight
}

void eye() {
  display.fillScreen(0);

  int cx = 160;
  int cy = 120;

  // fillEllipse(centerX, centerY, radiusW, radiusH, color)
  display.fillEllipse(cx, cy, 140, 60,  60);    // outer — dark gray
  display.fillEllipse(cx, cy, 100, 45, 120);    // middle
  display.fillEllipse(cx, cy,  50, 30, 200);    // inner — bright
  display.fillEllipse(cx, cy,  15, 15, 255);    // pupil — white
}

void mountains() {
  display.fillScreen(0);

  // fillTriangle(x0,y0, x1,y1, x2,y2, color)

  // Far mountain (dark)
  display.fillTriangle( 10, H, 140,  30, 270, H,  50);
  // Middle mountain
  display.fillTriangle( 80, H, 190,  60, 300, H, 110);
  // Near mountain (bright)
  display.fillTriangle(130, H, 220,  90, 310, H, 180);
}

void nestedRoundRects() {
  display.fillScreen(0);

  // drawRoundRect(x, y, width, height, cornerRadius, color)
  for (int i = 0; i < 6; i++) {
    int inset  = i * 18;
    int x      = 30  + inset;
    int y      = 15  + inset;
    int w      = 260 - inset * 2;
    int h      = 210 - inset * 2;
    int radius = 20 - i * 2;           // corner radius shrinks inward
    int color  = 60 + i * 35;
    display.drawRoundRect(x, y, w, h, radius, color);
  }
}

void fannedLines() {
  display.fillScreen(0);

  // drawLine(x0, y0, x1, y1, color)
  int originX = 0;
  int originY = H;

  int numLines = 10;
  int lineSpacing = W/numLines;

  for (int i = 0; i < numLines; i++) {
    int endX  = i * lineSpacing;       // spread across the top
    int endY  = 0;
    int color = 80 + i * ((255-80)/numLines);
    display.drawLine(originX, originY, endX, endY, color);
  }
}

void rectangleGrid() {
  display.fillScreen(0);

  int cols = 40;
  int rows = 4;

  int rw = W/cols;
  int rh = H/rows;

  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      display.fillRect(j * rw, i * rh, rw - 1, rh - 1, ((i * cols) + j) * (255/(cols * rows)));
    }
  }
}

void pixelGrid() {
  display.fillScreen(0);

  // drawPixel(x, y, color)
  for (int row = 0; row < 16; row++) {
    for (int col = 0; col < 20; col++) {
      int x     = 40 + col * 12;
      int y     = 28 + row * 12;
      int color = (row * 20 + col) * 255 / 319;   // gradient across grid
      display.drawPixel(x, y, color);
    }
  }
}

void mystery1() {
  display.drawCircle(70, 70, 50, 255);
  display.fillCircle(200, 170, 30, 128);
  display.fillCircle(230, 170, 30, 255);
}

void mystery2() {
  display.fillCircle(160, 120, 80, 255);
  display.fillRect(60, 20, 200, 100, 0);
  display.fillCircle(120, 80, 20, 255);
  display.fillCircle(200, 80, 20, 255);
  display.drawCircle(160, 120, 120, 255);
}

void mystery3() {
  display.drawFastHLine(80, 120, 160, 128);
  display.drawLine(240, 120, 200, 80, 128);
  display.drawLine(240, 120, 200, 160, 128);
}











