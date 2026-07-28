# esp32-rf-comms
This folder is about executing radio transmission commands via esp32 and Nrf24l01, showcasing increasing difficulty of data transmission
## Stages

| Folder | What it does | Key concept learned |
|--------|--------------|---------------------|
| `01-basic-tx-rx` | Sends a fixed message from one board to another | SPI to NRF24, addressing, TX/RX roles |
| `02-sensor-transmission` | Transmits a single sensor reading | Reading a sensor and sending live data |
| `03-climate-telemetry` | Multi-sensor climate station over radio with OLED display | Struct-based packet transmission, `sizeof`, multiple sensors |
| `04-bidirectional` | Two-way comms — receiver acknowledges / requests data | Request–response protocol |

*(Folders 04–05 in progress as the build continues.)*

---

## Stage 03 — Climate Telemetry (current flagship)

A wireless climate monitoring system across two independent ESP32 boards. The transmitter reads a set of environmental sensors, packs the readings into a struct, and sends them over the NRF24L01. The receiver unpacks the struct and shows live values on an OLED — no USB tether, each board runs on its own battery.

### A note on honest measurement

An earlier version of this project reported propane and methane in specific units. That was wrong: the MQ-2 is a general gas sensor and can't output calibrated ppm for a specific gas without proper calibration. The old code was effectively scaling a raw value by arbitrary numbers and presenting it as a real reading. This version reports the MQ-2 honestly — as raw gas *presence/level* via `analogRead`, not a fabricated propane/methane figure. Catching and fixing this was the point of rewriting the project rather than trusting the original.

### Hardware
- 2× ESP32 (38-pin) — one transmitter, one receiver
- 2× NRF24L01 radio modules
- DHT11 / DHT22 temperature & humidity sensor
- SSD1306 OLED display
- Flame sensor, light sensor (LDR), MQ-2 gas sensor
- Each board runs on its own battery + power switch (fully wireless)

### System overview
- **TX (Transmitter):** reads flame, light, temperature, humidity, and gas → packs into a struct → sends via NRF24L01
- **RX (Receiver):** receives the struct → displays live values on the OLED

---

### TX — Wiring

**NRF24L01 → ESP32**

| NRF24L01 | ESP32 |
|----------|-------|
| VCC | 3.3V |
| GND | GND |
| CE | GPIO 4 |
| CSN | GPIO 5 |
| SCK | GPIO 18 |
| MOSI | GPIO 23 |
| MISO | GPIO 19 |

**OLED Display**

| OLED | ESP32 |
|------|-------|
| VCC | 3.3V |
| GND | GND |
| SDA | GPIO 21 |
| SCL | GPIO 22 |

**Sensors**

| Sensor | ESP32 |
|--------|-------|
| Flame sensor (AO) | GPIO 25 |
| DHT11 data | GPIO 26 |
| Light sensor (AO) | GPIO 35 |
| MQ-2 gas sensor (AO) | GPIO 34 |

---

### RX — Wiring

**NRF24L01 → ESP32**

| NRF24L01 | ESP32 |
|----------|-------|
| VCC | 3.3V |
| GND | GND |
| CE | GPIO 2 |
| CSN | GPIO 15 |
| SCK | GPIO 18 |
| MOSI | GPIO 23 |
| MISO | GPIO 19 |

**OLED Display**

| OLED | ESP32 |
|------|-------|
| VCC | 3.3V |
| GND | GND |
| SDA | GPIO 4 |
| SCL | GPIO 5 |

---

## Why this project exists

This is the communication foundation for a larger goal: an autonomous drone that receives commands wirelessly. The radio, struct-packing, and bidirectional protocol skills built here carry directly into that system — this is where the drone learns to listen.

## Tech
`C++` · `ESP32` · `NRF24L01` · `SPI` · `SSD1306 OLED` · `PlatformIO`