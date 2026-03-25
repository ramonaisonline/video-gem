void drawBackground() {
  display.fillScreen(0);
}

void drawDebugScreen() {
  // Draw a border, to see edges of projection better
  display.drawRect(0, 0, W, H, 255);

  // Draw bar graph for knobs
  int barHeight = 2;
  int barGap = 1;
  for (int i = 0; i < 16; i++) {
    display.fillRect(32, (i * (barHeight + barGap)) + 16, potMap(i, 0, 256), barHeight, 255);
  }

  // Draw video gem diagram
  display.drawRoundRect(32, 82, 256, 100, 4, 255);
  display.drawRoundRect(58, 114, 32, 62, 3, 255);
  display.drawRect(63, 114, 22, 17, 255);
  display.fillRect(68, 141, 12, 12, 255);
  display.drawRect(68, 164, 12, 12, 255);

  // Draw knob + button grid
  int r = 8;
  int offset = r/2;
  int gap = 6 + r;
  int gridX = 99 + offset;
  int gridY = 91 + offset;

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 8; j++) {
      int x = gridX + (j * (gap + r)) + offset;
      int y = gridY + (i * (gap + r)) + offset;

      if (i == 0) { // All pots
        display.fillCircle(x, y, r, potMap(j, 0, 255));
        display.drawCircle(x, y, r, 255);
      }
      else if (i < 3) { // Mixed
        if (j <= 3) {
          if (keysPressed[(i - 1) * 4 + j]) {
            display.fillCircle(x, y, r - 2, 255);
          }
        }
        else {
          display.fillCircle(x, y, r, potMap((i+1) * 4 + (j - 4), 0, 255));
        }
        display.drawCircle(x, y, r, 255);
      }
      else { // All buttons
        display.drawCircle(x, y, r, 255);
        if (keysPressed[8+j]) {
          display.fillCircle(x, y, r - 2, 255);
        }
      }
    }
  }

  // Draw palette at the bottom of the screen
  for (int i = 0; i < 256; i++) {
    display.drawFastVLine(32 + i, 196, 30, i);
  }
}

// Examples that use inputs only

void coolRectangles() {

  int rw = potMap(0, 1, W);
  int rh = potMap(1, 1, H);
  int c = potMap(2, 254, 0);
  int mode = potMap(3, 0, 2);

  if (mode == 0) {
    display.fillScreen(255);
    display.fillRect(0, 0, rw, rh, c);
    display.fillRect(W-rw, H-rh, rw, rh, c);
  }
  else {
    display.drawRect(0, 0, rw, rh, c);
    display.drawRect(W-rw, H-rh, rw, rh, c);
  }
}

void doodle() {
  int x = potMap(0, 0, W);
  int y = potMap(1, 0, H);
  int r = potMap(2, 1, 100);
  int c = potMap(3, 0, 255);
  display.fillCircle(x, y, r, c);
}

void stretchyRectangles() {
  display.fillScreen(0);

  int rw0 = potMap(0, 0, W);
  int rw1 = potMap(0, W, 0);

  int rh0 = potMap(1, 0, H);
  int rh1 = potMap(1, H, 0);

  int c0 = potMap(2, 0, 255);
  int c1 = potMap(3, 0, 255);

  display.fillRect(HALFW - (rw0/2), HALFH - (rh0/2), rw0, rh0, c0);
  display.fillRect(HALFW - (rw1/2), HALFH - (rh1/2), rw1, rh1, c1);
}

void drawRegularShapes() {
  display.fillScreen(0);

  int r = potMap(0, 1, 150);
  int sides = potMap(1, 3, 20);
  float rotation = (float)potMap(2, 0, 359)/360.0;

  int rowSpacing = potMap(3, 40, HALFH);
  int colSpacing = potMap(3, 40, HALFW);
  int rows = H/rowSpacing;
  int cols = W/colSpacing;

  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      drawPolygon(HALFW + (i * colSpacing), HALFH + (j * rowSpacing), r, sides, rotation);

      if (i > 0 || j > 0) {
        drawPolygon(HALFW - (i * colSpacing), HALFH - (j * rowSpacing), r, sides, rotation);
        drawPolygon(HALFW - (i * colSpacing), HALFH + (j * rowSpacing), r, sides, rotation);
        drawPolygon(HALFW + (i * colSpacing), HALFH - (j * rowSpacing), r, sides, rotation);
      }
    }
  }
}

void star() {
  display.fillScreen(0);

  int r1 = potMap(0, 1, 150);
  int r2 = potMap(1, 1, 150);
  int points = potMap(2, 2, 20);
  float rotation = (float)potMap(3, 0, 359)/360.0;

  drawStar(HALFW, HALFH, r1, r2, points, rotation);
}

void fourTriangles() {
  int cx = HALFW;
  int cy = HALFH;
  int size = 100;

  // top
  display.fillTriangle(cx, cy, cx - size, cy, cx, cy - size, potMap(0, 0, 255));
  // right
  display.fillTriangle(cx, cy, cx + size, cy, cx, cy - size, potMap(1, 0, 255));
  // bottom-right
  display.fillTriangle(cx, cy, cx + size, cy, cx, cy + size, potMap(2, 0, 255));
  // bottom-left
  display.fillTriangle(cx, cy, cx - size, cy, cx, cy + size, potMap(3, 0, 255));
}

// Examples that use time
void slidingCircles() {
  display.fillScreen(0);
  
  int r0 = potMap(0, 1, 40);
  int r1 = potMap(1, 1, 40);
  int r2 = potMap(2, 1, 40);

  int c = potMap(3, 0, 255);

  float t0 = normalizedTime(1000, 0);
  int x0 = floor(t0 * (W + r0)) - r0;
  float t1 = normalizedTime(1000, 0.33);
  int x1 = floor(t1 * (W + r1)) - r1;
  float t2 = normalizedTime(1000, 0.66);
  int x2 = floor(t2 * (W + r2)) - r2;

  display.drawCircle(x0, 80, r0, c);
  display.drawCircle(x1, 120, r1, c);
  display.drawCircle(x2, 160, r2, c);
}

void randomCircle() {
  display.fillScreen(0);
  int periodMS = potMap(0, 40, 5000);

  // This is a hack to get a random value that only updates every so often
  // instead of every time this function is called
  // The random seed provides a predictable starting point for the randomness!
  randomSeed(millis() / periodMS);

  int x = random(W);
  int y = random(H);
  int r0 = potMap(1, 10, 200);
  int r1 = potMap(2, 10, 200);
  int c = potMap(3, 0, 255);

  display.fillCircle(x, y, r0, c);
  display.drawCircle(x, y, r1, 255-c);
}

void slidingLines() {
  display.fillScreen(0);

  randomSeed(potMap(0, 0, 8));
  int numSegments = potMap(1, 1, 32);
  int segmentWidth = W/numSegments;
  int maxSpread = potMap(2, 0, HALFH);
  int maxFreq = potMap(3, 50, 5);

  for (int i = 0; i < numSegments; i++) {
    int y = floor(sin(normalizedTime(random(maxFreq) * 100 + 500, 0) * TWO_PI) * maxSpread) + HALFH;
    display.drawFastHLine(i * segmentWidth, y, segmentWidth, 255);
  }
}

void growingCircle() {
  display.fillScreen(0);

  int periodMS = potMap(0, 1, 30) * 100;
  int rMin = potMap(1, 5, 100);
  int rMax = potMap(2, 10, 200);
  int c = potMap(3, 0, 255);

  int r0 = normalizedTime(periodMS, 0) * rMax + rMin;
  int r1 = normalizedTime(periodMS, 0.25) * rMax + rMin;
  int r2 = normalizedTime(periodMS, 0.5) * rMax + rMin;
  int r3 = normalizedTime(periodMS, 0.6) * rMax + rMin;
  int r4 = normalizedTime(periodMS, 0.65) * rMax + rMin;

  display.drawCircle(HALFW, HALFH, r0, c);
  display.drawCircle(HALFW, HALFH, r1, c);
  display.drawCircle(HALFW, HALFH, r2, c);
  display.drawCircle(HALFW, HALFH, r3, c);
  display.drawCircle(HALFW, HALFH, r4, c);
}

void blinkyRectangles() {
  display.fillScreen(0);

  int numBlocks = 8;
  int period = 1000;
  int timeChunk = period/numBlocks;

  int rw = W/numBlocks;
  int rh = rw;

  for (int i = 0; i < numBlocks; i++) {
    int t = millis() % 1000;
    if (t > (i * timeChunk)) {
      display.fillRect(i * rw, HALFH - (rh/2), rw-1, rh, 255);
    }
  }
}

void easingCircles() {
  display.fillScreen(0);
  
  float t = normalizedTime(4000, 0);
  float tEaseIn = t * t;
  float tEaseOut = 1.0 - (1.0 - t) * (1.0 - t);
  float tEaseInOut = t < 0.5 ? 2*t*t : 1 - 2*(1-t)*(1-t);

  int r = 20;

  int xLinear = floor(t * (W - r)) + r;
  int xEaseIn = floor(tEaseIn * (W-r)) + r;
  int xEaseOut = floor(tEaseOut * (W-r)) + r;
  int xEaseInOut = floor(tEaseInOut * (W-r)) + r;

  display.fillCircle(xLinear, 20, r, 255);
  display.fillCircle(xEaseIn, 70, r, 255);
  display.fillCircle(xEaseOut, 120, r, 255);
  display.fillCircle(xEaseInOut, 170, r, 255);

  float tPingPong = pingPongTime(4000, 0);
  int xPingPong = floor(tPingPong * (W-r)) + r;
  display.fillCircle(xPingPong, 220, r, 255);
}

void spinningLines() {
  display.fillScreen(0);

  drawSpinningLine(HALFW, HALFH, 100, 2000, 0, 255);
  drawSpinningLine(HALFW, HALFH, 100, 2000, 0.25, 255);
  drawSpinningLine(HALFW, HALFH, 50, 3000, 0, 255);
  drawSpinningLine(HALFW, HALFH, 50, 3000, 0.25, 255);
}

void drawSpinningLine(int cx, int cy, int r, int periodMS, float phase, int color) {

  float t = normalizedTime(periodMS, phase);

  int x0 = cx + r * sin(t * TWO_PI);
  int y0 = cy + r * cos(t * TWO_PI);
  int x1 = cx - r * sin(t * TWO_PI);
  int y1 = cy - r * cos(t * TWO_PI);

  display.drawLine(x0, y0, x1, y1, color);
}



// HELPERS

// Draw a regular polygon
void drawPolygon(int cx, int cy, int r, int sides, float rotation) {
  int x0 = 0;
  int y0 = 0;
  int x1 = 0;
  int y1 = 0;

  float angle = rotation * TWO_PI; // rotation is 0-1
  float incrementalAngle = TWO_PI/sides;

  for (int i = 0; i < sides; i++) {
    x0 = int(cx + r * sin(angle));
    y0 = int(cy + r * cos(angle));
    x1 = int(cx + r * sin(angle + incrementalAngle));
    y1 = int(cy + r * cos(angle + incrementalAngle));

    display.drawLine(x0, y0, x1, y1, 255);
    angle += incrementalAngle;
  }
}

// Draw a star based on two radii
void drawStar(int cx, int cy, int r1, int r2, int points, float rotation) {
  int x0 = 0;
  int y0 = 0;
  int x1 = 0;
  int y1 = 0;

  float angle = rotation * TWO_PI;
  float incrementalAngle = TWO_PI / (points * 2);

  for (int i = 0; i < points * 2; i++) {
    int r_current = (i % 2 == 0) ? r1 : r2;
    int r_next    = ((i + 1) % 2 == 0) ? r1 : r2;

    x0 = int(cx + r_current * sin(angle));
    y0 = int(cy + r_current * cos(angle));
    x1 = int(cx + r_next    * sin(angle + incrementalAngle));
    y1 = int(cy + r_next    * cos(angle + incrementalAngle));

    display.drawLine(x0, y0, x1, y1, 255);
    angle += incrementalAngle;
  }
}

// This helper function takes a period and a phase and returns a value in the range [0, 1)
float normalizedTime(int periodMS, float phase) {
  return ((millis() + int(phase * periodMS)) % periodMS) / (float)periodMS;
}

float pingPongTime(int periodMS, float phase) {
  float t = normalizedTime(periodMS, phase);
  return 1.0 - abs(2.0 * t - 1.0);
}






void customArt() {

}






