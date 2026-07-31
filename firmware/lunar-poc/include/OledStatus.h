#pragma once

#include <Arduino.h>

class OledStatus {
public:
  bool begin();
  void showBoot();
  void showSearching();
  void showFound(const String& name, int rssi);
  void showConnected(float weightGrams, int batteryPercent);
  void showReconnect();
  void showError(const String& message);

private:
  bool ready_ = false;

  void drawTwoLines(const String& line1, const String& line2);
};
