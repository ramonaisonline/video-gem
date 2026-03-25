
// Canvas FX

void cutFade() {
  int num = 40;

  for (int i = 0; i < num; i++) {
    int y = random(H);
    display.drawFastHLine(0, y, W, 0);
  }
}

void dotFade() {
  int num = 300;

  for (int i = 0; i < num; i++) {
    int x = random(W);
    int y = random(H);
    display.drawPixel(x, y, 0);
  }
}

// Framebuffer FX

void descend() {
  for (int i = 0; i < W; i++) {
    for (int j = 0; j < H; j++) {
      int c = display.getPixel(i, j);
      if (c > 4) {
        display.drawPixel(i, j, c - 4);
      }
    }
  }
}

void ascend() {
  for (int i = 0; i < W; i++) {
    for (int j = 0; j < H; j++) {
      int c = display.getPixel(i, j);
      if (c < 255) {
        display.drawPixel(i, j, c + 1);
      }
    }
  }
}

void mirrorHorizontally() {
  for (int i = 0; i <= HALFW; i++) {
    for (int j = 0; j < H; j++) {
      int c = display.getPixel(i, j);
      display.drawPixel(W-i, j, c);
    }
  }
}

void mirrorVertically() {
  for (int i = 0; i < W; i++) {
    for (int j = 0; j <= HALFH; j++) {
      int c = display.getPixel(i, j);
      display.drawPixel(i, H-j, c);
    }
  }
}

void mirrorQuad() {
  for (int i = 0; i <= HALFW; i++) {
    for (int j = 0; j <= HALFH; j++) {
      int c = display.getPixel(i, j);
      display.drawPixel(W-i, j, c);
      display.drawPixel(i, H-j, c);
      display.drawPixel(W-i, H-j, c);
    }
  }
}

// Palette FX

void invert() {
  for (int i = 0; i < 256; i++) {
    uint16_t color565 = display.getColor(i);
    uint16_t inverted = ~color565;
    display.setColor(i, inverted);
  }
}

void reverse() {
  for (int i = 0; i < 128; i++) {
    uint16_t color1 = display.getColor(i);
    uint16_t color2 = display.getColor(255-i);
    display.setColor(i, color2);
    display.setColor(255-i, color1);
  }
}

void wipe() {
  int width = 20;
  int period = 1000;
  int index = floor(normalizedTime(period, 0) * 255);

  for (int i = 0; i < width; i++) {
    uint16_t c = display.getColor(255);
    display.setColor((index + i) % 256, c);
  }
}

void dropouts() {
  int period = 1000;
  int offset = floor(normalizedTime(period, 0) * 255);

  for (int i = 0; i < 256; i++) {
    if (i % 4 != 0) {
      display.setColor((i + offset) % 256, 0);
    }
  }
}
