#include <Keypad.h>
#include "keypad_control.h"
#include "stack.h"

#define USER_KEY_INPUTS_MAX 5

const byte rowPins[] {9, 8, 7, 6};
const byte colPins[] {5, 4, 3, 2};

// Create the Keypad
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

Stack userInputs{};

void setup() {
  Serial.begin(9600);
  userInputs.top = nullptr;
  userInputs.length = 0;
}

void loop() {
  static long previousMillis = 0; // Milliseconds for counting timer.
  unsigned long currentMillis = millis();

  char key = keypad.getKey();

  if (key != NO_KEY) {
    previousMillis = currentMillis; // Reset timer if key input happens.
    Serial.print("User input appending: ");
    Serial.println(key);

    Push(&userInputs, key);

    Serial.print("Length: ");
    Serial.println(Length(&userInputs));

    if (Length(&userInputs) == USER_KEY_INPUTS_MAX) {
      Serial.println("Keypad input reached max. Clearing.");
      Clear(&userInputs);
    }
    if (!IsEmpty(&userInputs)) {
            
    } 
  } else {
    if (currentMillis - previousMillis >= TIMER_MILLISECS) {
      previousMillis = currentMillis;
      onIdleTimerReached();
    }        
  }
}

void onIdleTimerReached() {
  Serial.println("Idle reached. Clearing");
  Clear(&userInputs);
}
