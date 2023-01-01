#ifndef RFID_CONTROL
#define RFID_CONTROL

/*
Installed from external library: https://github.com/miguelbalboa/rfid
*/
#include <MFRC522.h>

void dump_byte_array(byte *buffer, byte bufferSize);

MFRC522* setupMFRC522(int ssPin, int rstPin, bool serialEnabled) {
  MFRC522* mfrc522 = new MFRC522();

  mfrc522->PCD_Init(ssPin, rstPin); // Initialize MFRC522.

  if (serialEnabled) {
    Serial.print("Reader: ");
    mfrc522->PCD_DumpVersionToSerial(); // Show device version.
  }

  return mfrc522;
}

void getRFIDCardUID(MFRC522* mfrc522) {
  if (mfrc522->PICC_IsNewCardPresent() && mfrc522->PICC_ReadCardSerial()) {
    // Show card contents.
    Serial.print(F("Card UID:"));
    dump_byte_array(mfrc522->uid.uidByte, mfrc522->uid.size); // Show card UID.
    Serial.println();
    Serial.print(F("PICC type: "));
    MFRC522::PICC_Type piccType = mfrc522->PICC_GetType(mfrc522->uid.sak);
    Serial.println(mfrc522->PICC_GetTypeName(piccType));  // Show card type.
    mfrc522->PICC_HaltA();  // Card enter stop mode.
  }
}

/**
 * Print out UID in hexadecimal.
 */
void dump_byte_array(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}

#endif