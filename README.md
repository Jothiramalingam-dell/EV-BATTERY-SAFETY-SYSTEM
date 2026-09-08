# EV Battery Active Moisture Mitigation & Multi-Tier Safety System

An ESP32-based early hazard intervention architecture for electric vehicle battery packs, engineered to prevent condensation-induced short circuits and mitigate thermal runaway risks before propagation.

---

## 📌 System Architecture & Pin Mapping

| Component | Function | Interface / Pin | Logic Level |
| :--- | :--- | :--- | :--- |
| **ESP32 NodeMCU** | Central Edge Controller | Master Unit | 3.3V / 5V VIN |
| **DHT22** | Air Humidity & Ambient Temp | GPIO 15 (Digital) | 3.3V / 5V |
| **DS18B20** | Cell Skin Surface Temperature | GPIO 4 (1-Wire Bus) | 3.3V |
| **MQ-2** | Gas Degassing & Smoke Detection | GPIO 34 (ADC Analog) | 5V (VCC to VIN) |
| **12V Relay (Fan)** | Moisture Exhaust Purge | GPIO 19 (Digital OUT) | 5V Coil |
| **Piezo Buzzer** | Acoustic Hazard Warning | GPIO 18 (Digital OUT) | 3.3V / 5V |
| **16x2 LCD** | Diagnostic Telemetry Display | GPIO 21 (SDA), 22 (SCL)| I2C Bus (0x27) |

---

## ⚡ Multi-Tier Response Logic

* **Normal Monitoring State:** Continually samples ambient dew-point indicators and cell skin temperature at 50ms intervals; displays real-time telemetry on the I2C LCD.
* **Stage 1 (Moisture Condensation Purge):** If relative humidity exceeds 70% RH, Relay 1 activates the 12V exhaust fan to purge moist air and prevent dew formation on busbars.
* **Stage 2 (Degassing & Thermal Interlock):** If MQ-2 detects volatile battery gases (H2/CO traces) or cell surface temperature crosses 50°C, the system trips the emergency relay to isolate the pack and sounds a continuous audio alarm via the piezo buzzer.

---

## 🛠️ Firmware Specification
* **Environment:** Embedded C++ (Arduino IDE / ESP-IDF)
* **Key Libraries:** `DHT sensor library`, `DallasTemperature`, `OneWire`, `LiquidCrystal_I2C`
* **Status:** Simulation verified on Wokwi; ready for benchtop hardware deployment.
*
