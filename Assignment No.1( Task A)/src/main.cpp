#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// --- Pin Definitions ---
#define LED1 18
#define LED2 19
#define BTN_MODE 35
#define BTN_RESET 34

// --- LEDC Settings ---
#define LEDC_CHANNEL1 0
#define LEDC_CHANNEL2 1
#define LEDC_TIMER_BIT 8       // 8-bit resolution (0–255)
#define LEDC_BASE_FREQ 5000    // 5 kHz PWM frequency

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
  pinMode(BTN_MODE, INPUT);
  pinMode(BTN_RESET, INPUT);

  // Initialize OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    for (;;); // halt if OLED not found
  }

  // Initialize LEDC (PWM)
  ledcSetup(LEDC_CHANNEL1, LEDC_BASE_FREQ, LEDC_TIMER_BIT);
  ledcSetup(LEDC_CHANNEL2, LEDC_BASE_FREQ, LEDC_TIMER_BIT);
  ledcAttachPin(LED1, LEDC_CHANNEL1);
  ledcAttachPin(LED2, LEDC_CHANNEL2);

  display.clearDisplay();
  showMode("OFF");
}

// --- Main Loop ---
void loop() {
  // --- Button Handling ---
  if (digitalRead(BTN_MODE) == HIGH) {
    mode = (mode + 1) % 4;  // Cycle through 0–3
    delay(300);             // debounce
  }

  if (digitalRead(BTN_RESET) == HIGH) {
    mode = 0;
    delay(300);
  }

  // --- LED Modes ---
  switch (mode) {
    case 0: // Both OFF
      ledcWrite(LEDC_CHANNEL1, 0);
      ledcWrite(LEDC_CHANNEL2, 0);
      showMode("OFF");
      break;

    case 1: // Alternate Blink
      showMode("ALT BLINK");
      if (millis() - previousMillis > 500) { // toggle every 0.5s
        previousMillis = millis();
        ledState = !ledState;
        ledcWrite(LEDC_CHANNEL1, ledState ? 255 : 0);
        ledcWrite(LEDC_CHANNEL2, ledState ? 0 : 255);
      }
      break;

    case 2: // Both ON
      ledcWrite(LEDC_CHANNEL1, 255);
      ledcWrite(LEDC_CHANNEL2, 255);
      showMode("BOTH ON");
      break;

    case 3: // PWM Fade
      showMode("FADE");
      for (int brightness = 0; brightness <= 255; brightness += 5) {
        ledcWrite(LEDC_CHANNEL1, brightness);
        ledcWrite(LEDC_CHANNEL2, brightness);
        delay(20);
      }
      for (int brightness = 255; brightness >= 0; brightness -= 5) {
        ledcWrite(LEDC_CHANNEL1, brightness);
        ledcWrite(LEDC_CHANNEL2, brightness);
        delay(20);
      }
      break;
  }
}
