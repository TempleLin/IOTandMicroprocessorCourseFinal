#include <Key.h>
#include <Keypad.h>

/*
Make sure to comment out the macros if its relevant functionality is not setup hardware-wise.
*/
#ifdef ESP_PLATFORM
  #define USE_NETWORKING // Only ESP32 can use networking features. Arduino doesn't have them internally.
#endif
#define USE_KEYPAD
#define USE_RFID

#define USER_KEY_INPUTS_MAX 5

const static bool NETWORKING_SERIAL = true;
const static bool RFID_SERIAL = true;


#ifdef USE_RFID
#include <SPI.h>
/*
Installed from external library: https://github.com/miguelbalboa/rfid
*/
#include <MFRC522.h>
#include "rfid_control.h"
#endif

#ifdef USE_KEYPAD
/*
Installed from external library: https://github.com/Chris--A/Keypad
*/
#include <Keypad.h>
#include "keypad_config_data.h"
#include "stack.h"
#endif

#ifdef USE_NETWORKING
#include "networking_control.h"
#endif


#ifdef USE_KEYPAD
#ifdef ESP_PLATFORM 
/*
Be aware: GPIO 34~39 in ESP32 are INPUT ONLY. Therefore, they cannot be used to connect to keypad.
*/
const byte rowPins[] {32, 33, 25, 26};
const byte colPins[] {27, 14, 12, 13};

#elif defined(__AVR__) // Arduino
const byte rowPins[] {9, 8, 7, 6};
const byte colPins[] {5, 4, 3, 2};
#endif

const char keypadRegisterPassword[] = {'A', '1', '2', '3', '4'}; // userInputs data must match this array in order to register new user(RFID) to database.

// Create the Keypad
Keypad keypad = Keypad(makeKeymap(keys), const_cast<byte*>(rowPins), const_cast<byte*>(colPins), ROWS, COLS);
Stack userInputs{}; // Keypad inputs get added to stack.

#endif

#ifdef USE_NETWORKING
WiFiClient* client;
const char* wifiSSID = "Galaxy A53 5GDC0F";
const char* wifiPass = "bcvs5702";
const char* server = "www.google.com";
#endif

#ifdef USE_RFID
#ifdef ESP_PLATFORM 
/*
Pins for ESP32 SPI:
- MOSI: IO23
- MISO: IO19
- SCK: IO18
*/
const uint8_t ssPin = 5; // SDA
const uint8_t rstPin = 16; // RST

#elif defined(__AVR__)
const uint8_t ssPin = 9;
const uint8_t rstPin = 10;
#endif

MFRC522* mfrc522;
#endif

void setup() {
  Serial.begin(9600);
  Serial.println("Setup.");  

#ifdef USE_RFID
  SPI.begin(); // This is needed for setting up RFID reading.
  mfrc522 = setupMFRC522(ssPin, rstPin, RFID_SERIAL);
#endif

#ifdef USE_KEYPAD
  userInputs.top = nullptr;
  userInputs.length = 0;
#endif

#ifdef USE_NETWORKING
    scanNearbyWiFi();
    connectToWiFi(wifiSSID, wifiPass, NETWORKING_SERIAL);
    client = connectToServer(server, NETWORKING_SERIAL);
#endif
}

void loop() {
#ifdef USE_KEYPAD
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
      Serial.println("Keypad input reached max.");
      if (EqualsArr(&userInputs, const_cast<char*>(keypadRegisterPassword), sizeof(keypadRegisterPassword) * sizeof(char), false)) {
        onKeypadRegisterPasswordMatch();
      } else {
        Clear(&userInputs);
      }
    }
  } else {
    if (currentMillis - previousMillis >= TIMER_MILLISECS) {
      previousMillis = currentMillis;
      onIdleTimerReached();
    }        
  }
#endif

#ifdef USE_RFID
  getRFIDCardUID(mfrc522);
#endif

#ifdef USE_NETWORKING
  // while (client->available()) { // If this loop keeps going, keypad won't be able to work.
  //   // char c = client->read();
  //   // Serial.write(c);
  // }

  // if the server's disconnected, stop the client:
  if (!client->connected()) {
    Serial.println();
    Serial.println("disconnecting from server.");
    client->stop();

    // do nothing forevermore:
    while (true);
  }
#endif
// Serial.println("Loop finish.");
}

#ifdef USE_KEYPAD
void onKeypadRegisterPasswordMatch() {
  Serial.println("Keypad register password match.");
  Serial.print("Length of userInput: ");
  Serial.println(Length(&userInputs));
}

void onIdleTimerReached() {
  Serial.println("Idle reached. Clearing");
  Clear(&userInputs);
}
#endif
