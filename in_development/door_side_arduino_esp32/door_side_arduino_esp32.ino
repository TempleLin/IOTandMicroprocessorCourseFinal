#include <Keypad.h>

const byte ROWS = 4; // Four rows
const byte COLS = 4; // Four columns

const int TIMER_MILLISECS = 3000; // Reset keypad input when idling for given seconds.
long previousMillis = 0; // Milliseconds for counting timer.

// Define the Keymap
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

// Connect keypad ROW0, ROW1, ROW2 and ROW3 to these Arduino pins.
byte rowPins[ROWS] = {9, 8, 7, 6};

// Connect keypad COL0, COL1 and COL2, COL3 to these Arduino pins.
byte colPins[COLS] = {5, 4, 3, 2};

// Create the Keypad
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

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
