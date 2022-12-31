#ifndef KEYPAD_SETUP
#define KEYPAD_SETUP

const byte ROWS = 4; // Four rows
const byte COLS = 4; // Four columns
int TIMER_MILLISECS = 3000; // Reset keypad input when idling for given seconds.

// Define the Keymap
const char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
class KeypadSetup {
private:
  byte* rowPins;
  byte* colPins;
public:


  KeypadSetup(byte* rowPins, byte* colPins) {
    this->rowPins = rowPins;
    this->colPins = colPins;
  }

  byte* getRowPins() {
    return rowPins;
  }
  byte* getColPins() {
    return colPins;
  }
};
#endif