#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

// --- Pin configuration ---
#define DHTPIN 14        // DHT sensor data pin
#define DHTTYPE DHT11    // Change to DHT22 if using that

#define SDA_PIN 21       // I2C SDA
#define SCL_PIN 23       // I2C SCL

// --- OLED setup ---
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// --- DHT setup ---
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  Serial.println("ESP32 Temperature Display Starting...");

  // Initialize I2C
  Wire.begin(SDA_PIN, SCL_PIN);

  // Initialize OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("SSD1306 initialization failed!");
    for (;;);
  }

  // Display startup message
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Initializing...");
  display.display();

  // Start DHT sensor
  dht.begin();
  delay(2000);
}

void loop() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println("Sensor Error!");
    display.display();
    delay(2000);
    return;
  }

  // Determine temperature status
  String tempStatus;
  if (temperature >= 30) {
    tempStatus = "Status: HOT";
  } else if (temperature <= 20) {
    tempStatus = "Status: COLD";
  } else {
    tempStatus = "Status: NORMAL";
  }

  // Print to Serial Monitor
  Serial.print("Temp: ");
  Serial.print(temperature);
  Serial.print(" °C | Humidity: ");
  Serial.print(humidity);
  Serial.print(" % | ");
  Serial.println(tempStatus);

  // Display on OLED
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("ESP32 Temp Monitor");

  display.setCursor(0, 16);
  display.print("Temp: ");
  display.print(temperature);
  display.println(" C");

  display.setCursor(0, 32);
  display.print("Humidity: ");
  display.print(humidity);
  display.println(" %");

  display.setCursor(0, 48);
  display.println(tempStatus);
  display.display();

  delay(2000);
}
