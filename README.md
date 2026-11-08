# Smart AI-Powered Connected EV Assistant 🚗⚡

An ESP32-based connected Electric Vehicle (EV) monitoring prototype developed using Embedded C/C++, CAN communication, GPS tracking and Python-based data analysis. The system uses two ESP32 boards to demonstrate vehicle data transmission, monitoring and dashboard display.

## 🚀 Project Overview

The Smart AI-Powered Connected EV Assistant demonstrates a two-node embedded architecture for connected vehicle monitoring.

- **ESP32 #1 – Vehicle ECU:** Generates vehicle parameters and transmits them through CAN communication using the MCP2515 module.
- **ESP32 #2 – Dashboard ECU:** Receives CAN data, processes vehicle parameters, reads GPS coordinates and displays information on an LCD.
- **Python Analysis:** Collects serial telemetry from the dashboard ESP32 and performs basic vehicle-data analysis.

The initial implementation uses simulated vehicle parameters for testing and demonstration.

## ✨ Key Features

- CAN communication between two ESP32 boards using MCP2515 modules.
- Vehicle parameter acquisition and monitoring.
- GPS-based location tracking using NEO-6M.
- Real-time vehicle information display on an LCD.
- Embedded data processing using ESP32.
- Python-based vehicle data logging and analysis.

## 🛠️ Hardware Requirements

| Component | Quantity |
|---|---:|
| ESP32 Development Board | 2 |
| MCP2515 CAN Module | 2 |
| NEO-6M GPS Module | 1 |
| 16x2 I2C LCD | 1 |
| Jumper Wires | As required |

## 💻 Software and Technologies

- Arduino IDE
- Embedded C/C++
- Python
- CAN Communication
- MCP2515 CAN Controller
- TinyGPSPlus Library
- LiquidCrystal_I2C Library
- PySerial

## 🔌 System Architecture

### ESP32 #1 – Vehicle ECU (Transmitter)

- Generates sample vehicle parameters.
- Encodes vehicle data into an 8-byte CAN frame.
- Transmits CAN messages through MCP2515.

### ESP32 #2 – Dashboard ECU (Receiver)

- Receives CAN messages from the transmitter.
- Decodes vehicle parameters.
- Reads GPS coordinates using NEO-6M.
- Displays vehicle speed and battery SOC on the LCD.
- Sends telemetry through serial communication to a computer.

### Python Data Analysis

- Reads serial data from ESP32.
- Stores vehicle telemetry in CSV format.
- Calculates basic statistics such as minimum, maximum and average values.

## 📡 CAN Communication

The project uses a custom CAN message format for demonstration.

| CAN Parameter | Description |
|---|---|
| CAN ID | 0x100 |
| Data Length | 8 Bytes |
| Bytes 0–1 | Vehicle Speed |
| Bytes 2–3 | Battery Voltage |
| Bytes 4–5 | Motor Temperature |
| Bytes 6–7 | Battery SOC |
| Bitrate | 500 kbps |

All values are encoded using little-endian representation.

*Note: This is a project-defined CAN message format, not a standard EV or OBD-II protocol.*

## 📂 Project Structure

```text
Smart-AI-Powered-Connected-EV-Assistant/
│
├── ESP32_Vehicle_Transmitter/
│   └── ESP32_Vehicle_Transmitter.ino
│
├── ESP32_Dashboard_Receiver/
│   └── ESP32_Dashboard_Receiver.ino
│
├── Python_Analysis/
│   └── ev_data_logger.py
│
├── docs/
│   └── system_architecture.png
│
└── README.md
```

## ⚙️ Installation and Setup

### 1. Arduino IDE Setup

Install ESP32 board support in Arduino IDE.

Install the following libraries:

- MCP2515 by autowp
- TinyGPSPlus
- LiquidCrystal_I2C

### 2. Upload Transmitter Code

- Open `ESP32_Vehicle_Transmitter.ino`.
- Select the ESP32 board and correct COM port.
- Upload the code to ESP32 #1.

### 3. Upload Receiver Code

- Open `ESP32_Dashboard_Receiver.ino`.
- Select the ESP32 board and correct COM port.
- Upload the code to ESP32 #2.

### 4. Python Setup

Install the required Python package:

```bash
pip install pyserial
```

Run the data logging script:

```bash
python ev_data_logger.py --port COM5 --baud 115200
```

Replace COM5 with the actual serial port of the receiver ESP32. Close Arduino Serial Monitor before running the Python script.

## 🔗 Hardware Connections

### MCP2515 to ESP32 (Both Boards)

| MCP2515 | ESP32 GPIO |
|---|---|
| SCK | GPIO 18 |
| SO | GPIO 19 |
| SI | GPIO 23 |
| CS | GPIO 5 |
| INT | GPIO 4 |
| GND | GND |

Connect CANH to CANH and CANL to CANL between both MCP2515 modules. Provide proper bus termination.

### NEO-6M GPS to ESP32 #2

| GPS | ESP32 |
|---|---|
| TX | GPIO 16 |
| RX | GPIO 17 |
| GND | GND |

### I2C LCD to ESP32 #2

| LCD | ESP32 |
|---|---|
| SDA | GPIO 21 |
| SCL | GPIO 22 |
| GND | GND |

**Hardware precautions:** Verify the MCP2515 oscillator frequency (8 MHz or 16 MHz) and configure both sketches accordingly. Check module logic-voltage compatibility. ESP32 GPIOs are not 5-V tolerant. Use suitable level shifting where required.

## 📊 Expected Output

The system demonstrates:

- Vehicle speed monitoring.
- Battery voltage and SOC monitoring.
- Motor temperature monitoring.
- GPS latitude and longitude acquisition.
- LCD-based dashboard display.
- Python-based telemetry logging and statistical analysis.

## 🔮 Future Enhancements

- Integration with actual EV CAN data.
- Advanced battery health monitoring.
- Predictive maintenance.
- Vehicle fault detection and alerts.
- AI/ML-based vehicle performance analysis.
- Cloud-based vehicle monitoring dashboard.

## 🖼️ Project Architecture

![Smart AI-Powered Connected EV Assistant](docs/system_architecture.png)

---

**Technologies:** ESP32 | Embedded C/C++ | CAN | MCP2515 | GPS | Arduino IDE | Python | LCD
