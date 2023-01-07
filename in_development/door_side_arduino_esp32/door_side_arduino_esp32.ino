/*
Make sure to comment out the macros if its relevant functionality is not setup hardware-wise.
*/
#ifdef ARDUINO_ARCH_ESP32 // Macros for identifing if the device is ESP32. (For Arduino: ARDUINO_AVR_UNO)
  #define USE_NETWORKING
#endif
#define USE_KEYPAD
#define USE_UART_PICO_RFID

/*
--- INCLUDES START ---
*/
#ifdef USE_KEYPAD
#include <Keypad.h>
#include "keypad_config_data.h"
#include "stack.h"
#endif

#ifdef USE_NETWORKING
#include "networking_control.h"
#endif

#ifdef USE_UART_PICO_RFID
#include "HardwareSerial.h"
#endif
/*
--- INCLUDES END ---
*/

/*
--- GLOBAL VARS START ---
*/
#ifdef ARDUINO_ARCH_ESP32 
/*
Be aware: GPIO 34~39 in ESP32 are INPUT ONLY. Therefore, they cannot be used to connect to keypad.
*/
const byte rowPins[] {32, 33, 25, 26};
const byte colPins[] {27, 14, 12, 13};
#else // Arduino
const byte rowPins[] {9, 8, 7, 6};
const byte colPins[] {5, 4, 3, 2};
#endif


#ifdef USE_KEYPAD
const int USER_KEY_INPUTS_MAX = 5;
const char keypadRegisterPassword[] = {'A', '1', '2', '3', '4'}; // userInputs data must match this array in order to register new user(RFID) to database.
// Create the Keypad
Keypad keypad = Keypad(makeKeymap(keys), const_cast<byte*>(rowPins), const_cast<byte*>(colPins), ROWS, COLS);
Stack userInputs{}; // Keypad inputs get added to stack.
#endif

#ifdef USE_NETWORKING
const static bool NETWORKING_SERIAL = true; // Log out stuffs in networking_control.h.
const char* wifiSSID = "MeetingIOTCtrl";
const char* wifiPass = "meeting1234";
const char* server = "http://192.168.4.1/";
#endif

#ifdef USE_UART_PICO_RFID
const static int UART2DATA_LEN = 10;
HardwareSerial serial2(2); // Use "UART2" in ESP32.
uint8_t uart2Data[UART2DATA_LEN];
String lastScannedRFID = "";
#endif
/*
--- GLOBAL VARS END ---
*/


void setup() {
  Serial.begin(9600);
  Serial.println("Setup.");  

#ifdef USE_UART_PICO_RFID
  serial2.begin(115200);
#endif

#ifdef USE_KEYPAD
  userInputs.top = nullptr;
  userInputs.length = 0;
#endif

#ifdef USE_NETWORKING
    scanNearbyWiFi();
    connectToWiFi(wifiSSID, wifiPass, NETWORKING_SERIAL);
#endif
}

void loop() {
  // Serial.println("Loop.");
  static long previousMillis = 0; // Milliseconds for counting timer.
  unsigned long currentMillis = millis();

#ifdef USE_UART_PICO_RFID
int readBytes = serial2.readBytes(uart2Data, 10);
if (readBytes > 0) {
  lastScannedRFID = "";
  for (int i = 0; i < UART2DATA_LEN; i++) {
    lastScannedRFID += (char)uart2Data[i];
    // Serial.print((char)uart2Data[i]);
  }
  Serial.println("Scanned RFID: " + lastScannedRFID);
#ifdef USE_NETWORKING
  loginRFIDToServer(const_cast<char*>(server), lastScannedRFID);
#endif
}
#endif

#ifdef USE_KEYPAD
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

#ifdef USE_NETWORKING

#endif
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
