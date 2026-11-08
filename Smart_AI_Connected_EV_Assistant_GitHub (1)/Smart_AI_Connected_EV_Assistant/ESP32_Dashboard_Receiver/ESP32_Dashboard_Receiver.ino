/*
 * ESP32 #2 - Dashboard ECU / CAN Receiver
 * Receives project-defined CAN frame 0x100, reads NEO-6M GPS,
 * displays speed/SOC on an I2C LCD and prints CSV over USB serial.
 *
 * Libraries: autowp MCP2515, TinyGPSPlus, LiquidCrystal_I2C
 */
#include <SPI.h>
#include <mcp2515.h>
#include <TinyGPSPlus.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define CAN_CS_PIN 5
#define CAN_INT_PIN 4
#define MCP2515_CLOCK MCP_8MHZ   // Change to MCP_16MHZ if your module has a 16 MHz crystal.
#define GPS_RX_PIN 16
#define GPS_TX_PIN 17
#define LCD_ADDRESS 0x27         // Try 0x3F if your LCD backpack uses that address.

MCP2515 mcp2515(CAN_CS_PIN);
TinyGPSPlus gps;
HardwareSerial GPSSerial(2);
LiquidCrystal_I2C lcd(LCD_ADDRESS, 16, 2);

float vehicleSpeed = 0.0f;
float batteryVoltage = 0.0f;
float motorTemp = 0.0f;
float soc = 0.0f;
unsigned long lastCsvMs = 0;

uint16_t getU16(const uint8_t *data, uint8_t index) {
  return (uint16_t)data[index] | ((uint16_t)data[index + 1] << 8);
}

void readCan() {
  struct can_frame frame;
  if (mcp2515.readMessage(&frame) == MCP2515::ERROR_OK) {
    if ((frame.can_id & CAN_EFF_FLAG) == 0 && frame.can_id == 0x100 && frame.can_dlc == 8) {
      vehicleSpeed = getU16(frame.data, 0) / 10.0f;
      batteryVoltage = getU16(frame.data, 2) / 100.0f;
      motorTemp = ((int16_t)getU16(frame.data, 4)) / 10.0f;
      soc = getU16(frame.data, 6) / 10.0f;

      lcd.setCursor(0, 0);
      lcd.print("Speed:");
      lcd.print(vehicleSpeed, 1);
      lcd.print(" km/h   ");

      lcd.setCursor(0, 1);
      lcd.print("SOC:");
      lcd.print(soc, 1);
      lcd.print("%          ");
    }
  }
}

void printCsvIfReady() {
  if (millis() - lastCsvMs >= 1000) {
    lastCsvMs = millis();

    double lat = gps.location.isValid() ? gps.location.lat() : 0.0;
    double lon = gps.location.isValid() ? gps.location.lng() : 0.0;

    // Header is printed once in setup. Each row: millis,speed,voltage,temp,soc,lat,lon
    Serial.print(millis()); Serial.print(",");
    Serial.print(vehicleSpeed, 1); Serial.print(",");
    Serial.print(batteryVoltage, 2); Serial.print(",");
    Serial.print(motorTemp, 1); Serial.print(",");
    Serial.print(soc, 1); Serial.print(",");
    Serial.print(lat, 6); Serial.print(",");
    Serial.println(lon, 6);
  }
}

void setup() {
  Serial.begin(115200);
  GPSSerial.begin(9600, SERIAL_8N1, GPS_RX_PIN, GPS_TX_PIN);

  Wire.begin(21, 22);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("EV Dashboard");
  lcd.setCursor(0, 1);
  lcd.print("Starting...");

  pinMode(CAN_INT_PIN, INPUT);
  SPI.begin(18, 19, 23, CAN_CS_PIN);
  mcp2515.reset();
  mcp2515.setBitrate(CAN_500KBPS, MCP2515_CLOCK);
  mcp2515.setNormalMode();

  Serial.println("elapsed_ms,speed_kmh,battery_voltage_v,motor_temp_c,soc_percent,latitude,longitude");
  lcd.clear();
}

void loop() {
  while (GPSSerial.available()) {
    gps.encode(GPSSerial.read());
  }

  readCan();
  printCsvIfReady();
}
