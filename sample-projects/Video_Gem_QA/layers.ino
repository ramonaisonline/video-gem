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












