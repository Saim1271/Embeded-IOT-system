/*
  --- Task A: LED Mode Controller with Fade ---
  Name: Ibtisam Butt
  Reg. No: 23-NTU-CS-1269
  Description: Two push buttons control LED modes and show the state on OLED
*/

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// --- Pin Configuration ---
#define LED1 18
#define LED2 19
#define BTN_MODE 35
#define BTN_RESET 34
// SDA → 21, SCL → 22

int mode = 0;
unsigned long previousMillis = 0;
bool ledState = false;

// --- Helper Function to show text on OLED ---
void showMode(String text) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 25);
  display.print("Mode: ");
  display.println(text);
  display.display();
}

// --- Setup ---
void setup() {
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(BTN_MODE, INPUT);
  pinMode(BTN_RESET, INPUT);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    for (;;); // halt if OLED not found
  }

  display.clearDisplay();
  showMode("OFF");
}

// --- Main Loop ---
void loop() {
  // --- Button Handling ---
  if (digitalRead(BTN_MODE) == HIGH) {
    mode = (mode + 1) % 4;  // 0–3 modes
    delay(300);             // debounce
  }

  if (digitalRead(BTN_RESET) == HIGH) {
    mode = 0;
    delay(300);
  }

  // --- LED Modes ---
  switch (mode) {
    case 0: // Both OFF
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, LOW);
      showMode("OFF");
      break;

    case 1: // Alternate Blink
      showMode("ALT BLINK");
      if (millis() - previousMillis > 500) { // toggle every 0.5s
        previousMillis = millis();
        ledState = !ledState;
        digitalWrite(LED1, ledState ? HIGH : LOW);
        digitalWrite(LED2, ledState ? LOW : HIGH);
      }
      break;

    case 2: // Both ON
      digitalWrite(LED1, HIGH);
      digitalWrite(LED2, HIGH);
      showMode("BOTH ON");
      break;

    case 3: // PWM Fade
      showMode("FADE");
      for (int brightness = 0; brightness <= 255; brightness += 5) {
        analogWrite(LED1, brightness);
        analogWrite(LED2, brightness);
        delay(20);
      }
      for (int brightness = 255; brightness >= 0; brightness -= 5) {
        analogWrite(LED1, brightness);
        analogWrite(LED2, brightness);
        delay(20);
      }
      break;
  }
}
