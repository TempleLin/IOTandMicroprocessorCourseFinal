/*
  Serial Event example

  When new serial data arrives, this sketch adds it to a String.
  When a newline is received, the loop prints the string and clears it.

  A good test for this is to try it with a GPS receiver that sends out
  NMEA 0183 sentences.

  NOTE: The serialEvent() feature is not available on the Leonardo, Micro, or
  other ATmega32U4 based boards.

  https://www.arduino.cc/en/Tutorial/BuiltInExamples/SerialEvent
*/
#include <Wire.h> // I2C Library
#include <LiquidCrystal_I2C.h> // LCD_I2C Model Library

// LCD I2C address，default: 0x27 or 0x3F
LiquidCrystal_I2C lcd(0x27, 16, 2); //(address, width, height)

String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

void setup() {
  // initialize serial:
  Serial.begin(9600);
  
  // Magnetic valve
  pinMode(8, OUTPUT);
  
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);

  lcd.init();
  lcd.backlight();
}

void loop() {
  // print the string when a newline arrives:
  if (stringComplete) {
    // Lcd start printing from (0, 0)
    lcd.setCursor(0, 0);
    
    Serial.println(inputString);
    
    // Close the valve
    if(inputString[0] == 0){
      digitalWrite(8, LOW);
    }
    // Open the valve
    else{
      digitalWrite(8, HIGH);
    }

    // Remove door toggle status
    inputString.remove(0, 1);
    // Print the msg 
    lcd.print(inputString);
    
    // clear the string:
    inputString = "";
    stringComplete = false;
  }
}

/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}
