// This is mostly the standard boilerplate provided by adafruit!
void handleKeypad() {
  // While there are events in the queue...
  while(keypad.available()){

    // Pull out an event
    keypadEvent e = keypad.read();

    // If it was a key press...
    if (e.bit.EVENT == KEY_JUST_PRESSED) {
      // Get some info about the key
      uint8_t row = e.bit.ROW;
      uint8_t col = e.bit.COL;
      int key = keys[e.bit.ROW][e.bit.COL];
      int keynum = row * COLS + col;

      // Record the fact that the key was pressed
      keypressTimes[keynum] = millis();
      keysPressed[keynum] = true;

      if (keynum <= 11) {
        // It's one of the twelve layer keys
        activeLayer = keynum;
        
        // If both button 8 and button 11 are held down, turn on debug mode
        if (keysPressed[8] == true && keysPressed[11] == true) {
          showDebug = !showDebug;
        }
      }
      else {
        // Handle the color palette keys
        previousActivePalette = activePalette;
        activePalette = keynum - 12;
      }
    }
    // if the event was a key release
    else if (e.bit.EVENT == KEY_JUST_RELEASED) {
      uint8_t row = e.bit.ROW;
      uint8_t col = e.bit.COL;
      int key = keys[e.bit.ROW][e.bit.COL];
      int keynum = row * COLS + col;
      keypressTimes[keynum] = millis();

      // Update our tracking of which keys are currently down
      keysPressed[keynum] = false;
    }
  }
}