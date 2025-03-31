#include <RadioLib.h>
#include <AES.h>

// Define the LoRa module (NSS, DIO1, RESET, BUSY)
SX1262 lora = new Module(10, 2, 9, 3);  

// AES encryption setup
AES128 aes;
uint8_t aesKey[16] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 
                      0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10}; // Example key
uint8_t plainText[16] = {0};  // AES block size (16 bytes)
uint8_t encryptedText[16] = {0}; // Buffer for encrypted data

// Timer variables
unsigned long previousMillis = 0;
const unsigned long interval = 60000;  // 60 seconds

void setup() {
  Serial.begin(115200);
  while (!Serial);  // Ensure Serial Monitor is ready
  Serial.println("LoRa AES-128 Sender with Timer Starting...");

  // Initialize LoRa module
  int state = lora.begin(868.0f, 125.0f, 7, 5, RADIOLIB_SX126X_SYNC_WORD_PRIVATE, 17, 8, 0);
  if (state != RADIOLIB_ERR_NONE) {
    Serial.print("LoRa initialization failed, code: ");
    Serial.println(state);
    while (true);  // Halt execution
  }
  Serial.println("LoRa Initialized!");

  // Initialize AES with the key
  aes.setKey(aesKey, aes.keySize());
}

void loop() {
  unsigned long currentMillis = millis();

  // Check if the interval has elapsed
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;  // Update the timer

    // Prepare the message
    String message = "Hello, World!";
    memset(plainText, 0, sizeof(plainText)); // Clear previous data
    message.getBytes(plainText, sizeof(plainText)); // Copy message with padding

    // Encrypt the message
    aes.encryptBlock(plainText, encryptedText);

    // Print encrypted data in HEX for debugging
    Serial.print("Encrypted Message: ");
    for (int i = 0; i < 16; i++) {
      Serial.print(encryptedText[i], HEX);
      Serial.print(" ");
    }
    Serial.println();

    // Transmit the encrypted message
    int state = lora.transmit(encryptedText, sizeof(encryptedText));
    if (state == RADIOLIB_ERR_NONE) {
      Serial.println("Encrypted message sent!");
    } else {
      Serial.print("Transmit failed, code: ");
      Serial.println(state);
    }
  }
}
