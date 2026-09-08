#include <Wire.h>
#include <DHT.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal_I2C.h>

// Pin Definitions
#define DHTPIN 15
#define DHTTYPE DHT22

#define ONE_WIRE_BUS 4
#define MQ2_PIN 34
#define BUZZER_PIN 18
#define RELAY_FAN_PIN 19

// Object Initialization
DHT dht(DHTPIN, DHTTYPE);
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature ds18b20(&oneWire);
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Threshold Constants
const float HUMIDITY_THRESHOLD = 70.0;    // Dew-point / high moisture limit (%)
const float CELL_TEMP_THRESHOLD = 50.0;   // Cell overheating limit (°C)
const int GAS_THRESHOLD = 1800;           // MQ-2 early degassing ADC limit (0-4095)

void setup() {
  Serial.begin(115200);

  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(RELAY_FAN_PIN, OUTPUT);

  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(RELAY_FAN_PIN, LOW); // Relay OFF initially

  dht.begin();
  ds18b20.begin();

  // LCD Setup
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("EV Battery Safety");
  lcd.setCursor(0, 1);
  lcd.print("System Booting...");
  delay(1500);
  lcd.clear();
}

void loop() {
  // 1. Read DHT22
  float humidity = dht.readHumidity();
  float ambientTemp = dht.readTemperature();

  // 2. Read DS18B20 (Cell Temperature)
  ds18b20.requestTemperatures();
  float cellTemp = ds18b20.getTempCByIndex(0);

  // 3. Read MQ-2 (Gas/Smoke Sensor)
  int gasLevel = analogRead(MQ2_PIN);

  // Sensor reading validation check
  if (isnan(humidity) || isnan(ambientTemp) || cellTemp == DEVICE_DISCONNECTED_C) {
    Serial.println("Sensor Read Error!");
    lcd.setCursor(0, 0);
    lcd.print("Sensor Read Err ");
    delay(1000);
    return;
  }

  // Serial Monitor Output for debugging
  Serial.print("Amb Temp: "); Serial.print(ambientTemp); Serial.print("C | ");
  Serial.print("Cell Temp: "); Serial.print(cellTemp); Serial.print("C | ");
  Serial.print("Humidity: "); Serial.print(humidity); Serial.print("% | ");
  Serial.print("Gas: "); Serial.println(gasLevel);

  // Safety Logic Evaluation
  bool isGasLeak = (gasLevel > GAS_THRESHOLD);
  bool isCellOverheat = (cellTemp > CELL_TEMP_THRESHOLD);
  bool isMoistureRisk = (humidity > HUMIDITY_THRESHOLD);

  if (isGasLeak || isCellOverheat) {
    // Stage 2: Critical Thermal Runaway / Degassing Hazard
    digitalWrite(RELAY_FAN_PIN, HIGH); // Exhaust/Purge active
    digitalWrite(BUZZER_PIN, HIGH);     // Continuous Alarm

    lcd.setCursor(0, 0);
    if (isGasLeak) lcd.print("! GAS DETECTED !");
    else lcd.print("! CELL OVERHEAT!");

    lcd.setCursor(0, 1);
    lcd.print("T:"); lcd.print(cellTemp, 1);
    lcd.print("C EV TRIP! ");

  } else if (isMoistureRisk) {
    // Stage 1: Moisture Condensation Risk
    digitalWrite(RELAY_FAN_PIN, HIGH); // Trigger Exhaust Fan
    digitalWrite(BUZZER_PIN, LOW);

    lcd.setCursor(0, 0);
    lcd.print("MOISTURE PURGE  ");
    lcd.setCursor(0, 1);
    lcd.print("H:"); lcd.print(humidity, 1);
    lcd.print("% Fan: ON  ");

  } else {
    // Normal Safe Operation
    digitalWrite(RELAY_FAN_PIN, LOW);  // Fan OFF
    digitalWrite(BUZZER_PIN, LOW);     // Buzzer OFF

    lcd.setCursor(0, 0);
    lcd.print("C-T:"); lcd.print(cellTemp, 1);
    lcd.print("C H:"); lcd.print(humidity, 0); lcd.print("%  ");

    lcd.setCursor(0, 1);
    lcd.print("Gas:"); lcd.print(gasLevel);
    lcd.print("  SAFE ");
  }

  delay(1000);
}
