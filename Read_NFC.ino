#include <Wire.h>
#include <Adafruit_PN532.h>

// I2C configuration: IRQ connected to D2, RESET to D3
#define PN532_IRQ 2
#define PN532_RESET 3

Adafruit_PN532 nfc(PN532_IRQ, PN532_RESET);

void setup() {
  Serial.begin(9600);
  while (!Serial); // Wait for Serial Monitor to open (helpful for Nano/Leonardo)
  
  Serial.println("Arduino NFC Card Reader");
  Serial.println("Starting PN532...");
  
  nfc.begin();

  uint32_t version = nfc.getFirmwareVersion();
  if (!version) {
    Serial.println("PN532 not detected. Check wiring and mode.");
    while (1) {
      delay(10);
    }
  }

  // Print firmware version details
  Serial.print("PN532 firmware version: ");
  Serial.print((version >> 16) & 0xFF, HEX);
  Serial.print(".");
  Serial.println((version >> 8) & 0xFF, HEX);

  nfc.SAMConfig();
  Serial.println("Ready. Scan an NFC card or tag.");
}

void loop() {
  uint8_t uid[7];
  uint8_t uidLength;

  // Try to read a passive target (ISO14443A card/tag) with a 1-second timeout
  bool success = nfc.readPassiveTargetID(
    PN532_MIFARE_ISO14443A,
    uid,
    &uidLength,
    1000
  );

  if (success) {
    Serial.print("Card detected. UID: ");

    for (uint8_t i = 0; i < uidLength; i++) {
      if (uid[i] < 0x10) {
        Serial.print("0");
      }
      Serial.print(uid[i], HEX);
      if (i < uidLength - 1) {
        Serial.print(" ");
      }
    }

    Serial.println();
    delay(1000); // Debounce delay so it doesn't spam reads
  }
}
