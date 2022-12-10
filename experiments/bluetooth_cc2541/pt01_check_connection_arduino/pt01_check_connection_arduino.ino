#include <SoftwareSerial.h>

/*
Input "AT+HELP" in Arduino terminal to see if it returns output.

Sometimes if the module is not returning desired outputs, make it power out for enough time to reset.
*/

SoftwareSerial mySerial(2, 3); // RX, TX

void initBluetooth() {
  mySerial.begin(9600);
  Serial.begin(9600);
}

void updateSerial() {
  if (mySerial.available()) {
    Serial.write(mySerial.read());
  }
  if (Serial.available()) {
    mySerial.write(Serial.read());
  }
}

void setup() {
  initBluetooth();
}

void loop() {
  updateSerial();
  // delay(10); //WARNING: adding delay to loop will cause receiving giberrish from BLE module.
}
