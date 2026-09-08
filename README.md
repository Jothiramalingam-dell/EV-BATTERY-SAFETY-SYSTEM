# AI Based Moisture Detection and Safety Monitoring for Lithium-Ion Batteries

An intelligent, edge-computing hazard intervention architecture for Li-ion battery enclosures, designed to predict condensation, identify pre-thermal runaway degassing, and execute multi-tier protective interlocks.

---

## 📌 System Architecture & Pin Mapping

| Component | Function | Interface / Pin | Logic Level |
| :--- | :--- | :--- | :--- |
| **ESP32 NodeMCU** | Central Edge Processing Unit | Master Controller | 3.3V / 5V VIN |
| **DHT22** | Enclosure RH% & Ambient Temperature | GPIO 15 (Digital) | 3.3V / 5V |
| **DS18B20** | Direct Li-ion Cell Skin Temperature | GPIO 4 (1-Wire Bus) | 3.3V |
| **MQ-2** | Electrolyte Degassing & Smoke Detection | GPIO 34 (ADC Analog) | 5V (VCC to VIN) |
| **12V Relay (Fan)** | Active Moisture Purging Actuator | GPIO 19 (Digital OUT) | 5V Coil |
| **Piezo Buzzer** | Acoustic Hazard Warning | GPIO 18 (Digital OUT) | 3.3V / 5V |
| **16x2 LCD** | Real-Time Telemetry & State Display | GPIO 21 (SDA), 22 (SCL)| I2C Bus (0x27) |

---

## ⚡ AI Predictive & Multi-Tier Safety Logic

* **Predictive Dew-Point & Condensation Estimation:** Uses edge regression logic based on ambient temperature and relative humidity to predict moisture accumulation before dew formation on Li-ion busbars.
* **Stage 1 (Active Moisture Purge):** Automatically engages the 12V exhaust ventilation relay when humidity exceeds critical thresholds (70% RH) to eliminate condensation risks.
* **Stage 2 (Degassing & Thermal Interlock):** Isolates the battery pack via relay trip and sounds a continuous audio alarm if MQ-2 detects venting electrolyte gases ($H_2/CO$) or cell temperature crosses 50°C.

---

## 🛠️ Firmware Specification
* **Environment:** Embedded C++ (Arduino IDE / ESP-IDF)
* **Libraries:** `DHT sensor library`, `DallasTemperature`, `OneWire`, `LiquidCrystal_I2C`
* **Status:** Verified on Wokwi edge simulation; benchmarked for benchtop Li-ion test bench integration.
*
