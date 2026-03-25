void drawBackground() {
  display.fillScreen(0);
}

// Put your custom art functions here! Use this format:

void customArtProgram() {
  // Draw fun stuff in here!
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

void orbitingCircles() {
  // display.fillScreen(0);

  int numCircles = potMap(0, 1, 16);
  float incrementalAngle = 1/(float)numCircles;

  int orbitR = cyclicalPotMap(1, 3, 0, 4, 100);
  int circleR = cyclicalPotMap(1, 5, 0, 4, 100);

  int period = potMap(2, 20, 1) * 500;
  float rotationOffset = normalizedTime(period, 0);

  int c = potMap(3, 0, 256);

  for (int i = 0; i < numCircles; i++) {
    int cx = HALFW + floor(orbitR * cos(((i * incrementalAngle) + rotationOffset) * TWO_PI));
    int cy = HALFH + floor(orbitR * sin(((i * incrementalAngle) + rotationOffset) * TWO_PI));

    display.drawCircle(cx, cy, circleR, c);
  }
}

void animatedSpark() {
  display.fillScreen(0);

  for (int i = 0; i < 16; i++) {
    for (int j = 0; j < 12; j++) {
      int x = i * 20;
      int y = j * 20;

      float xPhaseIncrement = potMap(0, 0, 21) / 20.0;
      float yPhaseIncrement = potMap(1, 0, 21) / 20.0;
      float phaseOffset = i * xPhaseIncrement + j * yPhaseIncrement;
      int bitmapIndex = floor(pingPongTime(1000, phaseOffset) * 5); // 0-5

      int xColorIncrement = potMap(2, 0, 256);
      int yColorIncrement = potMap(3, 0, 256);
      int c = (i * xColorIncrement + j * yColorIncrement) % 255;

      display.drawBitmap(x, y, spark_bitmap_array[bitmapIndex], 17, 17, c);
    }
  }
}

void twistingLines() {
  int numLines = potMap(0, 1, 16) * 2 + 1;
  float angle = TWO_PI/(float)numLines;

  int r0 = potMap(1, 4, 200);
  int r1 = potMap(2, 4, 200);
  int c = potMap(3, 0, 256);

  // int period = cyclicalPotMap(3, 3, 0, 20, 1) * 500;
  int period = 5000;
  float rotationOffset = normalizedTime(period, 0) * TWO_PI;
  float phaseOffset = cyclicalPotMap(3, 2, 0, 0, 360)/360.0f * TWO_PI;

  for (int i = 0; i < numLines; i++) {
    int x0 = HALFW + floor(r0 * cos((i * angle) + rotationOffset));
    int y0 = HALFH + floor(r0 * sin((i * angle) + rotationOffset));

    int x1 = HALFW + floor(r1 * cos((i * angle) + rotationOffset + phaseOffset));
    int y1 = HALFH + floor(r1 * sin((i * angle) + rotationOffset + phaseOffset));

    display.drawLine(x0, y0, x1, y1, c);
  }
}

void sparkBrush() {
  // display.fillScreen(0);

  int xPeriod = potMap(0, 50, 10) * 100;
  int yPeriod = potMap(1, 50, 10) * 100;
  int bitmap = potMap(2, 0, 6);
  int c = potMap(3, 0, 256);

  int x = floor(sineTime(xPeriod, 0.5) * HALFW) + HALFW;
  int y = floor(sineTime(yPeriod, 0) * HALFH) + HALFH;

  display.drawBitmap(x - 8, y - 8, spark_bitmap_array[bitmap], 17, 17, c);
}

void wobblyLines() {
  // display.fillScreen(0);

  int copies = potMap(0, 1, 7);
  int xRange = 15; //center +/- 15 = segment width of 30... about 11 copies in 320px
  int xWidth = xRange * 2;
  float phaseOffset = potMap(1, 0, 360)/360.0;
  int period = potMap(2, 5, 1) * 1000;
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

void randomGlyphs() {
  display.setFont();

  int glyph = potMap(0, 1, 256);
  int num =   potMap(1, 1, 50);
  int size =  potMap(2, 1, 12);
  int color = potMap(3, 0, 256);

  for (int i = 0; i < num; i++) {
    int x = random(W) / (6 * size) * (6 * size);
    int y = random(H) / (8 * size) * (8 * size);
    display.drawChar(x, y, glyph, color, 0, size);
  }
}

void polygonGrid() {
  // display.fillScreen(0);

  int r = potMap(0, 1, 150);
  int sides = potMap(1, 3, 20);
  float rotation = (float)potMap(2, 0, 359)/360.0;

  int rowSpacing = potMap(3, 40, HALFH);
  int colSpacing = potMap(3, 40, HALFW);
  int rows = H/rowSpacing;
  int cols = W/colSpacing;

  int c = floor(sineTime(2000, 0) * 255);

  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      int x = j * colSpacing;
      int y = i * rowSpacing;

      drawPolygon(HALFW + x, HALFH + y, r, sides, rotation, c);
      if (i > 0) {
        drawPolygon(HALFW + x, HALFH - y, r, sides, rotation, c);
      }
      if (j > 0) {
        drawPolygon(HALFW - x, HALFH + y, r, sides, rotation, c);
      }          
      if (i > 0 && j > 0) {
        drawPolygon(HALFW - x, HALFH - y, r, sides, rotation, c);
      }
    }
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

void reflectedRects() {
  // slow down the pace of random updates 
  // by only allowing new random values once per second
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

// LFO examples
void slidingCirclesWithLFOs() {
  display.fillScreen(0);
  
  float frequency = potMap(0, 20, 200) / 100.0;
  lfos[0].setFrequency(frequency);
  int x = floor(lfos[0].sineBipolar() * HALFW) + HALFW;

  display.fillCircle(x, HALFH, 40, 255);
}

void slidingCirclesDifferentWaveforms() {
  display.fillScreen(0);
  
  float frequency = potMap(0, 20, 200) / 100.0;
  lfos[0].setFrequency(frequency);

  int r = 20;

  int x0 = floor(lfos[0].sineBipolar() * HALFW) + HALFW;
  int x1 = floor(lfos[0].triangleBipolar() * HALFW) + HALFW;
  int x2 = floor(lfos[0].squareBipolar() * HALFW) + HALFW;
  int x3 = floor(lfos[0].sawBipolar() * HALFW) + HALFW;
  int x4 = floor(lfos[0].rampBipolar() * HALFW) + HALFW;

  display.fillCircle(x0, HALFH - 60, r, 255);
  display.fillCircle(x1, HALFH - 30, r, 255);
  display.fillCircle(x2, HALFH, r, 255);
  display.fillCircle(x3, HALFH + 30, r, 255);
  display.fillCircle(x4, HALFH + 60, r, 255);
}

void slidingCirclesDifferentPhases() {
  display.fillScreen(0);
  
  float frequency = potMap(0, 20, 200) / 100.0;
  lfos[0].setFrequency(frequency);

  int r = 20;

  int x0 = floor(lfos[0].sineBipolar(0) * HALFW) + HALFW;
  int x1 = floor(lfos[0].sineBipolar(0.2 * TWO_PI) * HALFW) + HALFW;
  int x2 = floor(lfos[0].sineBipolar(0.4 * TWO_PI) * HALFW) + HALFW;
  int x3 = floor(lfos[0].sineBipolar(0.6 * TWO_PI) * HALFW) + HALFW;
  int x4 = floor(lfos[0].sineBipolar(0.8 * TWO_PI) * HALFW) + HALFW;

  display.fillCircle(x0, HALFH - 60, r, 255);
  display.fillCircle(x1, HALFH - 30, r, 255);
  display.fillCircle(x2, HALFH, r, 255);
  display.fillCircle(x3, HALFH + 30, r, 255);
  display.fillCircle(x4, HALFH + 60, r, 255);
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

void drawScaledBitmap(int x, int y, const unsigned char* bitmap, int w, int h, int scale, int color) {
  for (int row = 0; row < h; row++) {
    for (int col = 0; col < w; col++) {
      int bitIndex = row * w + col;
      uint8_t byte = pgm_read_byte(&bitmap[bitIndex / 8]);
      bool lit = (byte >> (7 - (bitIndex % 8))) & 1;
      display.fillRect(x + col * scale, y + row * scale, scale, scale, lit ? color : 0);
    }
  }
}







