#include <Keypad.h>
#include "keypad_setup.h"

long previousMillis = 0; // Milliseconds for counting timer.

// Connect keypad ROW0, ROW1, ROW2 and ROW3 to these Arduino pins.
byte rowPins[ROWS] = {9, 8, 7, 6};

// Connect keypad COL0, COL1 and COL2, COL3 to these Arduino pins.
byte colPins[COLS] = {5, 4, 3, 2};

KeypadSetup keypadSetup(rowPins, colPins);

// Create the Keypad
Keypad keypad = Keypad(makeKeymap(keys), keypadSetup.getRowPins(), keypadSetup.getColPins(), ROWS, COLS);

void setup() {
  Serial.begin(9600);
}

void loop() {
  unsigned long currentMillis = millis();

  char key = keypad.getKey();

  if (key != NO_KEY) {
    Serial.println(key);
    previousMillis = currentMillis; // Reset timer if key input happens.
  } else {
    if (currentMillis - previousMillis >= TIMER_MILLISECS) {
      previousMillis = currentMillis;
      onIdleTimerReached();
    }        
  }
}

void onIdleTimerReached() {
  Serial.print("Idle reached.\n");
}
