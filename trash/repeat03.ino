#include <RadioLib.h>

// Define the LoRa module (NSS, DIO1, RESET, BUSY)
SX1262 lora = new Module(10, 2, 9, 3);  

// Variables for receiving and transmitting
String receivedMessage;
int state;

// Initialization
void setup() {
  Serial.begin(115200);
  while (!Serial);  // Ensure Serial Monitor is ready
  Serial.println("LoRa Repeater Starting...");

  // Initialize LoRa module
  state = lora.begin(868.0f, 125.0f, 7, 5, RADIOLIB_SX126X_SYNC_WORD_PRIVATE, 17, 8, 0);
  if (state != RADIOLIB_ERR_NONE) {
    Serial.print("Initialization failed, code: ");
    Serial.println(state);
    while (true);  // Halt execution
  }
  Serial.println("LoRa Initialized!");
}

// Main loop
void loop() {
  // Receive message
  state = lora.receive(receivedMessage);
  if (state == RADIOLIB_ERR_NONE) {
    Serial.print("Received: ");
    Serial.println(receivedMessage);

    // Check the first three bytes of the message
    if (receivedMessage.startsWith("ABC") || receivedMessage.startsWith("CBA")) {
      Serial.println("Matching sequence found!");

      // Forward the received message
      state = lora.transmit(receivedMessage);
      if (state == RADIOLIB_ERR_NONE) {
        Serial.println("Message relayed!");
      } else {
        Serial.print("Transmit failed, code: ");
        Serial.println(state);
      }
    } else {
      Serial.println("No matching sequence found.");
    }
  } else if (state != RADIOLIB_ERR_RX_TIMEOUT && state != RADIOLIB_ERR_CRC_MISMATCH) {
    Serial.print("Receive failed, code: ");
    Serial.println(state);
  }

  delay(10); // Small delay to avoid flooding LoRa module
}
