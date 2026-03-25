// Scan the potentiometer grid and record values
void updatePots() { 
  int sum = 0;
  int samples = 20;
  int newValue = 0;

  for (int i = 0; i < 4; i++) {
    digitalWrite(MUX_CTRL_0, i & 1);
    digitalWrite(MUX_CTRL_1, i & 2);
    delayMicroseconds(50);

    // Read first input
    sum = 0;
    for (int i = 0; i < samples; i++) {
      sum += analogRead(POT_A_PIN);
    }

    newValue = sum/samples;
    pots[i] = 1023-newValue;
    
    // Read second input
    sum = 0;
    for (int i = 0; i < samples; i++) {
      sum += analogRead(POT_B_PIN);
    }

    newValue = sum/samples;
    pots[i+4] = 1023-newValue;

    // Read third input
    sum = 0;
    for (int i = 0; i < samples; i++) {
      sum += analogRead(POT_C_PIN);
    }

    newValue = sum/samples;
    pots[i+8] = 1023-newValue;

    // Read fourth input
    sum = 0;
    for (int i = 0; i < samples; i++) {
      sum += analogRead(POT_D_PIN);
    }

    newValue = sum/samples;
    pots[i+12] = 1023-newValue;
  }
}

int potMap(int potIndex, int min, int max) {
  return map(pots[potIndex], 0, 1023, min, max);
}

int threePointMap(int potIndex, int splitPoint, int targetMin, int targetMid, int targetMax) {
  if (pots[potIndex] < splitPoint) {
    return map(pots[potIndex], 0, splitPoint, targetMin, targetMid);
  }
  else {
    return map(pots[potIndex], splitPoint, 1023, targetMid, targetMax);
  }
}

int cyclicalPotMap(int potIndex, float multiple, float phase, int min, int max) {
  int diff = max - min;
  float cycle = sin((pots[potIndex] / 1023.0) * multiple * TWO_PI + (phase * TWO_PI)) / 2.0 + 0.5; //0-1
  return(min + (diff * cycle));
}

