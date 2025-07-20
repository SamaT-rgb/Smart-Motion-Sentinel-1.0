#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Pin assignments
#define PIR_PIN     13   // PIR OUT to GPIO13
#define LED_PIN     2    // LED to GPIO2
#define BUZZER_PIN  4    // Buzzer to GPIO4

int motionCount = 0;
bool motionPreviouslyDetected = false;

void setup() {
  pinMode(PIR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  Serial.begin(115200);

  // Initialize OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("❌ OLED initialization failed");
    while (true);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Smart Motion Sentinel");
  display.display();
  delay(1500);
  display.clearDisplay();
}

void loop() {
  int motion = digitalRead(PIR_PIN);

  if (motion == HIGH) {
    if (!motionPreviouslyDetected) {
      motionCount++;
      Serial.print("🚨 Motion Detected! Count: ");
      Serial.println(motionCount);
      motionPreviouslyDetected = true;
    }

    digitalWrite(LED_PIN, HIGH);
    digitalWrite(BUZZER_PIN, HIGH);
    showStatus("Motion Detected", motionCount);
  } else {
    if (motionPreviouslyDetected) {
      Serial.println("✅ Motion ended.");
    }

    digitalWrite(LED_PIN, LOW);
    digitalWrite(BUZZER_PIN, LOW);
    showStatus("No Motion", motionCount);
    motionPreviouslyDetected = false;
  }

  delay(200);
}

void showStatus(String status, int count) {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.println("Smart Motion Sentinel");
  display.setCursor(0, 20);
  display.print("Status: ");
  display.println(status);
  display.setCursor(0, 40);
  display.print("Count: ");
  display.println(count);
  display.display();
}
