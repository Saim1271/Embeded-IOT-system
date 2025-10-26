/*
  --- Task B: Button Press Detection ---
  Name: M.Saim Mustafa
  Reg. No: 23-NTU-CS-1271
  Description: Short press toggles LED, long press triggers buzzer
*/

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// --- Pin Configuration ---
#define LED_PIN 18
#define BUTTON_PIN 35
#define BUZZER_PIN 19  // connect buzzer here

unsigned long buttonPressTime = 0;
bool buttonHeld = false;
bool ledState = false;

// --- Helper Function to show text on OLED ---
void showEvent(String text) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 25);
  display.println(text);
  display.display();
}

// --- Setup ---
void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    for (;;); // halt if OLED not found
  }

  display.clearDisplay();
  showEvent("Waiting for input...");
}

// --- Main Loop ---
void loop() {
  int buttonState = digitalRead(BUTTON_PIN);

  if (buttonState == HIGH && !buttonHeld) {
    // Button just pressed
    buttonPressTime = millis();
    buttonHeld = true;
  }
  
  if (buttonState == LOW && buttonHeld) {
    // Button released
    unsigned long pressDuration = millis() - buttonPressTime;
    buttonHeld = false;

    if (pressDuration < 1500) {
      // Short press → toggle LED
      ledState = !ledState;
      digitalWrite(LED_PIN, ledState ? HIGH : LOW);
      showEvent("Short Press: LED " + String(ledState ? "ON" : "OFF"));
    } else {
      // Long press → buzzer tone
      showEvent("Long Press: Buzzer");
      tone(BUZZER_PIN, 1000, 500); // 1 kHz tone for 0.5s
    }
  }
}
