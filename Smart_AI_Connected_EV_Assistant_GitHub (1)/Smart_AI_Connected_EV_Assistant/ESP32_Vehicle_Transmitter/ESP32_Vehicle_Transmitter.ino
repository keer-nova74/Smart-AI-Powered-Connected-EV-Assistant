/*
 * ESP32 #1 - Vehicle ECU / CAN Transmitter
 * Sends DEMONSTRATION vehicle values over MCP2515 CAN.
 *
 * Libraries: autowp MCP2515 library
 * CAN ID 0x100, 8-byte project-defined payload.
 */
#include <SPI.h>
#include <mcp2515.h>

#define CAN_CS_PIN 5
#define CAN_INT_PIN 4
#define MCP2515_CLOCK MCP_8MHZ   // Change to MCP_16MHZ if your module has a 16 MHz crystal.

MCP2515 mcp2515(CAN_CS_PIN);

unsigned long lastSendMs = 0;
const unsigned long SEND_INTERVAL_MS = 500;

void putU16(uint8_t *data, uint8_t index, uint16_t value) {
  data[index] = value & 0xFF;
  data[index + 1] = (value >> 8) & 0xFF;
}

void setup() {
  Serial.begin(115200);
  pinMode(CAN_INT_PIN, INPUT);
  SPI.begin(18, 19, 23, CAN_CS_PIN);

  mcp2515.reset();
  mcp2515.setBitrate(CAN_500KBPS, MCP2515_CLOCK);
  mcp2515.setNormalMode();

  Serial.println("Vehicle ECU transmitter started.");
}

void loop() {
  if (millis() - lastSendMs >= SEND_INTERVAL_MS) {
    lastSendMs = millis();

    // Simulated values for bench testing. Replace with measured/decoded values
    // only after integrating real sensors or a documented vehicle CAN database.
    float speedKmh = 35.0 + (millis() / 1000UL) % 45;
    float batteryVoltage = 350.0;
    float motorTempC = 42.0 + ((millis() / 5000UL) % 12);
    float socPercent = 78.0;

    struct can_frame frame;
    frame.can_id = 0x100;
    frame.can_dlc = 8;

    uint16_t speed10 = (uint16_t)(speedKmh * 10.0f);
    uint16_t voltage100 = (uint16_t)(batteryVoltage * 100.0f);
    int16_t temp10 = (int16_t)(motorTempC * 10.0f);
    uint16_t soc10 = (uint16_t)(socPercent * 10.0f);

    putU16(frame.data, 0, speed10);
    putU16(frame.data, 2, voltage100);
    putU16(frame.data, 4, (uint16_t)temp10);
    putU16(frame.data, 6, soc10);

    if (mcp2515.sendMessage(&frame) == MCP2515::ERROR_OK) {
      Serial.println("CAN frame sent: ID 0x100");
    } else {
      Serial.println("CAN transmit error");
    }
  }
}
