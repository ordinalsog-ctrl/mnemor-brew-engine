#include "OledStatus.h"

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

#include "MnemorConfig.h"

namespace {
Adafruit_SSD1306 display(OLED_WIDTH, OLED_HEIGHT, &Wire, -1);
}

bool OledStatus::begin() {
#if OLED_ENABLED
  Wire.begin(OLED_SDA_PIN, OLED_SCL_PIN);
  ready_ = display.begin(SSD1306_SWITCHCAPVCC, OLED_I2C_ADDR);
  if (!ready_) {
    Serial.println("[OLED] init failed");
    return false;
  }
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  showBoot();
  Serial.println("[OLED] initialized");
  return true;
#else
  ready_ = false;
  return false;
#endif
}

void OledStatus::showBoot() {
  drawTwoLines("Mnemor Brew", "Booting...");
}

void OledStatus::showSearching() {
  drawTwoLines("Searching...", "Acaia Lunar");
}

void OledStatus::showFound(const String& name, int rssi) {
  drawTwoLines("Found:", name + " " + String(rssi));
}

void OledStatus::showConnected(float weightGrams, int batteryPercent) {
  if (!ready_) {
    return;
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("Connected");
  if (batteryPercent >= 0) {
    display.print(" ");
    display.print(batteryPercent);
    display.print("%");
  }

  display.setTextSize(3);
  display.setCursor(0, 24);
  display.print(weightGrams, 2);
  display.setTextSize(1);
  display.print(" g");
  display.display();
}

void OledStatus::showReconnect() {
  drawTwoLines("Reconnect", "Waiting...");
}

void OledStatus::showError(const String& message) {
  drawTwoLines("Error", message);
}

void OledStatus::drawTwoLines(const String& line1, const String& line2) {
  if (!ready_) {
    return;
  }
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print(line1);
  display.setTextSize(2);
  display.setCursor(0, 24);
  display.print(line2);
  display.display();
}
