#include <RadioLib.h>
#include <AES.h>

// Define the LoRa module
SX1262 lora = new Module(10, 2, 9, 3);  // Replace with your actual pin connections (NSS, DIO1, RESET, BUSY)

// AES encryption setup
AES128 aes;
byte aesKey[16] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10};  // Example key (replace for security)
byte plainText[16];  // Block size for AES-128 encryption

// Timer variables
unsigned long previousMillis = 0;
const unsigned long interval = 60000;  // 60 seconds

void setup() {
  Serial.begin(9600);
  Serial.println("LoRa AES-128 Sender with Timer Starting...");

  // Initialize LoRa module
  int state = lora.begin(868.0, 125.0, 7, 5, RADIOLIB_SX126X_SYNC_WORD_PRIVATE, 17, 8, 0);
  if (state != RADIOLIB_ERR_NONE) {
    Serial.print("Initialization failed, code: ");
    Serial.println(state);
    while (true);
  }
  Serial.println("LoRa Initialized!");

  // Initialize AES with the key
  aes.setKey(aesKey, aes.keySize());
}

void loop() {
  unsigned long currentMillis = millis();  // Check the current time

  // Check if the interval has elapsed
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;  // Update the timer

    // Create the message
    String message = "Hello, World!";
    message.getBytes(plainText, 16);  // Convert the message to a byte array

    // Encrypt the message
    aes.encryptBlock(plainText);

    // Transmit the encrypted message
    int state = lora.transmit((byte*)plainText, sizeof(plainText));
    if (state == RADIOLIB_ERR_NONE) {
      Serial.println("Encrypted message sent!");
    } else {
      Serial.print("Transmit failed, code: ");
      Serial.println(state);
    }
  }
}
