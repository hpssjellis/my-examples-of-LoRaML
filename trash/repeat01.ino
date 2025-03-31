#include <RadioLib.h>

// Define the LoRa module
SX1262 lora = new Module(10, 2, 9, 3);  // Replace with your actual pin connections (NSS, DIO1, RESET, BUSY)

// Variables for receiving and transmitting
String receivedMessage;
int state;

// Initialization
void setup() {
  Serial.begin(9600);
  Serial.println("LoRa Repeater Starting...");

  // Initialize LoRa module
  state = lora.begin(868.0, 125.0, 7, 5, RADIOLIB_SX126X_SYNC_WORD_PRIVATE, 17, 8, 0);
  if (state != RADIOLIB_ERR_NONE) {
    Serial.print("Initialization failed, code: ");
    Serial.println(state);
    while (true);
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

    // Forward the received message
    state = lora.transmit(receivedMessage);
    if (state == RADIOLIB_ERR_NONE) {
      Serial.println("Message relayed!");
    } else {
      Serial.print("Transmit failed, code: ");
      Serial.println(state);
    }
  } else if (state != RADIOLIB_ERR_RX_TIMEOUT && state != RADIOLIB_ERR_CRC_MISMATCH) {
    Serial.print("Receive failed, code: ");
    Serial.println(state);
  }
}
