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

// Bitmap examples

void randomArt() {
  display.fillScreen(0);
  display.drawBitmap(HALFW, HALFH, epd_bitmap_random_art, 17, 17, 255);
}



void logoSmall() {
  display.fillScreen(0);
  display.drawBitmap(144, 108, epd_bitmap_Video_gem_logo_small, 32, 24, 255);
}

void logoBig() {
  display.fillScreen(0);
  int scale = 3;
  int w = 32;
  int h = 24;
  int x = HALFW - (w * scale)/2;
  int y = HALFH - (h * scale)/2;

  drawScaledBitmap(x, y, epd_bitmap_Video_gem_logo_small, w, h, scale, 255);
}

void randomSparks() {
  // display.fillScreen(0);

  int bitmap = potMap(0, 0, 6);
  int xRange = potMap(1, 0, W);
  int yRange = potMap(2, 0, H);
  int c = potMap(3, 0, 256);

  int x = HALFW + random(xRange) - (xRange/2);
  int y = HALFH + random(yRange) - (yRange/2);

  for (int i = 0; i < 10; i++) {
    display.drawBitmap(x, y, spark_bitmap_array[bitmap], 17, 17, c);
  }
}

void ramonas() {
  // display.fillScreen(0);

  int size = potMap(0, 1, 5); //1-4
  int xRange = potMap(1, 0, W);
  int yRange = potMap(2, 0, H);
  int c = potMap(3, 0, 256);

  int x = HALFW + random(xRange) - (xRange/2);
  int y = HALFH + random(yRange) - (yRange/2);

  drawScaledBitmapWithTransparency(x, y, ramona, 32, 32, size, c);
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

// Text examples
void simpleCharacters() {
  display.setFont();
  display.fillScreen(0);

  display.drawChar(100, 100, 0x52, 128, 0, 1);
  display.drawChar(150, 120, 0x4D, 170, 0, 2);
  display.drawChar(200, 180, 0x4E, 212, 0, 3);
  display.drawChar(250,  40, 0x41, 255, 0, 4);
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

void simpleWords() {
  display.fillScreen(0);
  display.setFont(&FreeSerifItalic12pt7b);

  int x = potMap(0, 0, W);
  int y = potMap(1, 0, H);
  int sizeMultiple = potMap(2, 1, 5);
  int c = potMap(3, 0, 256);


  display.setCursor(x, y);
  display.setTextColor(c);
  display.setTextSize(sizeMultiple);

  display.print("RMNA");
}

void movingWords() {
  display.setFont(&FreeSerifItalic12pt7b);
  display.setTextWrap(false);

  int period = potMap(0, 50, 5) * 100;
  randomSeed(millis() / period);

  int x = random(W);
  int y = random(H);

  int widthMultiple = potMap(1, 1, 5);
  int heightMultiple = potMap(2, 1, 5);
  int c = potMap(3, 0, 256);


  display.setCursor(x, y);
  display.setTextColor(c);
  display.setTextSize(widthMultiple, heightMultiple);

  display.print("RMNA");
}

void simpleSentence() {
  display.fillScreen(0);
  display.setFont(&FreeSerifItalic12pt7b);
  display.setTextWrap(true);

  int x = potMap(0, 0, W);
  int y = potMap(1, 0, H);
  int sizeMultiple = potMap(2, 1, 5);
  int c = potMap(3, 0, 256);


  display.setCursor(x, y);
  display.setTextSize(sizeMultiple);
  display.setTextColor(c);

  display.print("Made with love by RMNA!");
}

void longText() {
  
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

void drawScaledBitmapWithTransparency(int x, int y, const unsigned char* bitmap, int w, int h, int scale, int color) {
  for (int row = 0; row < h; row++) {
    for (int col = 0; col < w; col++) {
      int bitIndex = row * w + col;
      uint8_t byte = pgm_read_byte(&bitmap[bitIndex / 8]);
      bool lit = (byte >> (7 - (bitIndex % 8))) & 1;
      if (lit) {
        display.fillRect(x + col * scale, y + row * scale, scale, scale, color);
      }
    }
  }
}







