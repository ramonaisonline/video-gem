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

// Examples that use translation symmetry

void circleTranslation() {
  display.fillScreen(0);
  
  int copies = potMap(0, 1, 33);
  int gap = potMap(1, 1, 11);

  int rBase = 10;
  int rIncrement = potMap(2, 0, 50);
  int rMax = 200;
  
  int cBase = 128;
  int cIncrement = potMap(3, 0, 255);

  for (int i = 0; i < copies; i++) {
    int x = i * gap;
    int r = (rBase + (i * rIncrement)) % rMax;
    int c = (cBase + (i * cIncrement)) % 256;
    display.drawCircle(x, HALFH, r, c);
  }
}

void circleTranslationSmooth() {
  // Want a smoother control experience? Try this version with floats!
  display.fillScreen(0);
  int copies = potMap(0, 1, 33);
  float gap = (pots[1] / 1023.0) * 10;

  int rBase = 10;
  float rIncrement = (pots[2] / 1023.0) * 100;
  int rMax = 200;
  
  int cBase = 128;
  float cIncrement = (pots[3] / 1023.0) * 256;

  for (int i = 0; i < copies; i++) {
    // We let fractional values accumulate into gradual changes
    // then snap it into integers when we actually need it to be integers...
    // ...at the point of drawing!
    // usually just floor() is good enough for rounding down
    // but the extra (int) case is needed for the % operator
    int x = floor(i * gap);
    int r = (int)floor(rBase + (i * rIncrement)) % rMax;
    int c = (int)floor(cBase + (i * cIncrement)) % 256;
    display.drawCircle(x, HALFH, r, c);
  }
}

void wobblyLines() {
  display.fillScreen(0);

  int copies = potMap(0, 1, 7);

  int xRange = 15; //center +/- 15 = segment width of 30... about 11 copies in 320px
  int xWidth = xRange * 2;
  float phaseOffset = potMap(1, 0, 360)/360.0;
  int period = potMap(2, 500, 50) * 10;
  int c = potMap(3, 0, 255);

  int x0 = HALFW + floor(sin(normalizedTime(period, 0) * TWO_PI) * xRange);
  int x1 = HALFW + floor(sin(normalizedTime(period, phaseOffset) * TWO_PI) * xRange);
  display.drawLine(x0, 0, x1, H, c);

  //draw copies, if any
  if (copies > 1) {
    for (int i = 1; i < copies; i++) {
      x0 = HALFW + (i * xWidth) + floor(sin(normalizedTime(period, i * phaseOffset) * TWO_PI) * xRange);
      x1 = HALFW + (i * xWidth) + floor(sin(normalizedTime(period, (i + 1) * phaseOffset) * TWO_PI) * xRange);
      display.drawLine(x0, 0, x1, H, c);

      x0 = HALFW - (i * xWidth) + floor(sin(normalizedTime(period, i * phaseOffset) * TWO_PI) * xRange);
      x1 = HALFW - (i * xWidth) + floor(sin(normalizedTime(period, (i + 1) * phaseOffset) * TWO_PI) * xRange);
      display.drawLine(x0, 0, x1, H, c);
    }
  }
}

void polygonGrid() {
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
      int x = j * colSpacing;
      int y = i * rowSpacing;

      drawPolygon(HALFW + x, HALFH + y, r, sides, rotation, 255);
      if (i > 0) {
        drawPolygon(HALFW + x, HALFH - y, r, sides, rotation, 255);
      }
      if (j > 0) {
        drawPolygon(HALFW - x, HALFH + y, r, sides, rotation, 255);
      }          
      if (i > 0 && j > 0) {
        drawPolygon(HALFW - x, HALFH - y, r, sides, rotation, 255);
      }
    }
  }
}

void gradientGrid() {
  // This one has a gap at the end of the rows because of the rounding errors
  int numRows = potMap(0, 1, 64);
  int numCols = potMap(1, 1, 64);
  int rw = ceil((float)W/numCols);
  int rh = ceil((float)H/numRows);

  int index = 0;
  int c = 0;
  int firstC = (millis() / 10) % 254 + 1;
  int iGap = potMap(2, 0, 255);
  int jGap = potMap(3, 0, 255);

  double period = 500.0 + 1000;

  for (int i = 0; i < numCols; i++) {
    for (int j = 0; j < numRows; j++) {
      c = (firstC + (i * iGap) + (j * jGap)) % 254 + 1;

      display.fillRect(i * rw, j * rh, rw, rh, c);
    }
  }
}


// Examples that use reflective symmetry

void basicReflections() {
  int xBase = 80;
  int yBase = 60;
  int xRange = potMap(0, 0, 80);
  int yRange = potMap(1, 0, 60);

  int r = potMap(2, 1, 100);

  int mode = potMap(3, 0, 5);

  float t = normalizedTime(2000, 0); // 0-1 ramp

  int cx = xBase + floor(sin(t * TWO_PI) * xRange);
  int cy = yBase + floor(cos(t * TWO_PI) * yRange);

  display.fillScreen(0);
  display.fillCircle(cx, cy, r, 255);

  if (mode == 0) {
    // do nothing
  }
  else if (mode == 1) {
    // horizontal reflection
    display.fillCircle(W-cx, cy, r, 255);
  }
  else if (mode == 2) {
    // vertical reflection
    display.fillCircle(cx, H-cy, r, 255);
  }
  else if (mode == 3) {
    // horizontal and vertical together
    display.fillCircle(W-cx, H-cy, r, 255);
  }
  else if (mode == 4) {
    // combined
    display.fillCircle(W-cx, cy, r, 255);
    display.fillCircle(cx, H-cy, r, 255);
    display.fillCircle(W-cx, H-cy, r, 255);
  }
}

void reflectedRects() {
  // slow down the pace of random updates 
  // by only allowing new random values once per second
  display.fillScreen(0);
  randomSeed(millis() / 1000);

  // our constraints for the randomness
  int widthLimit = potMap(0, 1, HALFW/2);
  int heightLimit = potMap(1, 1, H/2);
  int quantity = potMap(2, 1, 10);
  int c = potMap(3, 0, 255-20);

  for (int i = 0; i < quantity; i++) {
    int rw = random(1, widthLimit);
    int rh = random(1, heightLimit);
    int x = random(W - rw);
    int y = random(H - rh);
    
    display.fillRect(x, y, rw, rh, c + random(20));
    display.fillRect(W - x - rw, H - y - rh, rw, rh, c + random(20));
    display.fillRect(W - x - rw, y, rw, rh, c + random(20));
    display.fillRect(x, H - y - rh, rw, rh, c + random(20));
  }
}

void expandingCircles() {
  // pairs of circles with opposing motion and position
  int xOffset = cyclicalPotMap(0, 3, 0, 0, HALFW);
  int yOffset = cyclicalPotMap(1, 3, 0, 0, HALFH);

  int period0 = potMap(2, 100, 10) * 100;
  int period1 = potMap(3, 100, 10) * 100;

  int rMax0 = cyclicalPotMap(0, 4, 0, 40, HALFW);
  int rMax1 = cyclicalPotMap(1, 4, 0.5, 40, HALFW);

  int c = floor(normalizedTime(period1, 0) * 255);

  int r0 = floor(normalizedTime(period0, 0) * rMax0) + 1;
  int r1 = floor(normalizedTime(period0, 0.5) * rMax1) + 1;

  // First pair (notice opposing offset signs?)
  display.drawCircle(HALFW + xOffset, HALFH - yOffset, r0, c);
  display.drawCircle(HALFW - xOffset, HALFH + yOffset, r0, c);
  // Second pair
  display.drawCircle(HALFW + xOffset, HALFH + yOffset, r1, c);
  display.drawCircle(HALFW - xOffset, HALFH - yOffset, r1, c);
}

void reflectedLines() {
  // One line attached to the axes of reflection
  // One line radiating from the reflection point
  display.fillScreen(0);

  int copies = potMap(0, 1, 20);

  int cBase = 255;
  int cIncrement = potMap(1, 0, 255);

  int period = potMap(2, 5, 50) * 100;
  randomSeed(millis() / period);

  int wiggleRange = potMap(3, 0, 100);
  int wiggle = floor(normalizedTime(5000, 0) * wiggleRange);


  for (int i = 0; i < copies; i++) {
    // Chord line
    int x0 = random(0, HALFW) - wiggle;
    int y0 = HALFH-1;
    int x1 = HALFW-1;
    int y1 = random(0, HALFH) - wiggle;

    // Radiating line
    int x2 = HALFW-1;
    int y2 = HALFH-1;
    int x3 = random(0, HALFW) - wiggle;
    int y3 = random(0, HALFH) - wiggle;

    int c = (cBase + (i * cIncrement)) % 256;

    display.drawLine(x0, y0, x1, y1, c);
    display.drawLine(x2, y2, x3, y3, c);

    // Horizontal reflection
    display.drawLine(W-x0, y0, W-x1, y1, c);
    display.drawLine(W-x2, y2, W-x3, y3, c);

    // Vertical reflection
    display.drawLine(x0, H-y0, x1, H-y1, c);
    display.drawLine(x2, H-y2, x3, H-y3, c);

    // Diagonal reflection
    display.drawLine(W-x0, H-y0, W-x1, H-y1, c);
    display.drawLine(W-x2, H-y2, W-x3, H-y3, c);
  }
  
}


// Examples that use rotational symmetry

void orbitingCircles() {x
  display.fillScreen(0);

  int numCircles = potMap(0, 1, 16);
  float incrementalAngle = 1/(float)numCircles;

  int orbitR = potMap(1, 4, 100);
  int circleR = potMap(2, 4, 100);

  int period = potMap(3, 20, 1) * 500;
  float rotationOffset = normalizedTime(period, 0);

  for (int i = 0; i < numCircles; i++) {
    int cx = HALFW + floor(orbitR * cos(((i * incrementalAngle) + rotationOffset) * TWO_PI));
    int cy = HALFH + floor(orbitR * sin(((i * incrementalAngle) + rotationOffset) * TWO_PI));

    display.drawCircle(cx, cy, circleR, 255);
  }
}

void twistingLines() {
  display.fillScreen(0);

  int numLines = potMap(0, 1, 16) * 2 + 1;
  float angle = TWO_PI/(float)numLines;

  int r0 = potMap(1, 4, 200);
  int r1 = potMap(2, 4, 200);

  // int period = cyclicalPotMap(3, 3, 0, 20, 1) * 500;
  int period = 5000;
  float rotationOffset = normalizedTime(period, 0) * TWO_PI;
  float phaseOffset = cyclicalPotMap(3, 2, 0, 0, 360)/360.0f * TWO_PI;

  for (int i = 0; i < numLines; i++) {
    int x0 = HALFW + floor(r0 * cos((i * angle) + rotationOffset));
    int y0 = HALFH + floor(r0 * sin((i * angle) + rotationOffset));

    int x1 = HALFW + floor(r1 * cos((i * angle) + rotationOffset + phaseOffset));
    int y1 = HALFH + floor(r1 * sin((i * angle) + rotationOffset + phaseOffset));

    display.drawLine(x0, y0, x1, y1, 255);
  }
}

void multiPolygon() {
  display.fillScreen(0);

  int numShapes = potMap(0, 1, 16);
  float angle = TWO_PI/(float)numShapes;

  int r0 = potMap(1, 4, 200);
  int r1 = potMap(2, 4, 200);

  int sides = potMap(3, 3, 9);

  // int period = cyclicalPotMap(3, 3, 0, 20, 1) * 500;
  int period = 5000;
  float rotationOffset = normalizedTime(period, 0) * TWO_PI;
  // float phaseOffset = cyclicalPotMap(3, 2, 0, 0, 360)/360.0f * TWO_PI;

  for (int i = 0; i < numShapes; i++) {
    float theta = i * angle;
    int cx = HALFW + floor(r0 * cos(theta + rotationOffset));
    int cy = HALFH + floor(r0 * sin(theta + rotationOffset));

    drawPolygon(cx, cy, r1, sides, (-1 * rotationOffset - theta)/TWO_PI, 255);
  }
}

void bloom() {
  int petals = potMap(0, 2, 16);
  int levels = potMap(1, 1, 16);
  int rBase = potMap(2, 2, 24);
  float rGrowth = cyclicalPotMap(3, 2, 0, 1, 100)/10.0f;
  
  float gap = sineTime(4000, 0) * cyclicalPotMap(0, 3, 0, 1, 80);
  float spiral = sineTime(4500, 0) * ((float)cyclicalPotMap(1, 3, 0, 1, 100)/100.0f);
  float offset = sineTime(5700, 0) * ((float)cyclicalPotMap(2, 3, 0, 1, 100)/100.0f);

  int cBase = 0;
  int cPeriod = potMap(3, 300, 30) * 100;
  int cOffset = floor(sineTime(cPeriod, 0) * 255);
  // int color = constrain(cBase + cOffset, 0, 255);
  int color = (cBase + cOffset) % 256;

  int cx = 0;
  int cy = 0;
  int cr = 0;

  for (int i = 0; i < petals; i++) {
    for (int j = 1; j <= levels; j++) {
      cx = floor(HALFW + (sin((float(i) / float(petals)) * TWO_PI + (j * spiral + offset) * TWO_PI) * (j * gap)));
      cy = floor(HALFH + (cos((float(i) / float(petals)) * TWO_PI + (j * spiral + offset) * TWO_PI) * (j * gap)));
      cr = floor(rBase + (j * rGrowth));
      display.fillCircle(cx, cy, cr, color);
    }
  }
}


// HELPERS

// Draw a regular polygon
void drawPolygon(int cx, int cy, int r, int sides, float rotation, int color) {
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

    display.drawLine(x0, y0, x1, y1, color);
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

float sineTime(int periodMS, float phase) {
  return sin(normalizedTime(periodMS, phase) * TWO_PI);
}

float pingPongTime(int periodMS, float phase) {
  float t = normalizedTime(periodMS, phase);
  return 1.0 - abs(2.0 * t - 1.0);
}

void drawCenteredRect(int cx, int cy, int rw, int rh, int c) {
  display.drawRect(cx - (rw/2), cy - (rh/2), rw, rh, c);
}

void fillCenteredRect(int cx, int cy, int rw, int rh, int c) {
  display.fillRect(cx - (rw/2), cy - (rh/2), rw, rh, c);
}










