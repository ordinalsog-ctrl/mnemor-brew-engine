#pragma once

#include <Arduino.h>
#include <BLEDevice.h>
#include <array>

struct LunarReading {
  bool hasWeight = false;
  bool hasBattery = false;
  float weightGrams = 0.0f;
  int batteryPercent = -1;
};

class LunarProtocol {
public:
  void begin(BLERemoteCharacteristic* commandCharacteristic);

  bool sendId();
  bool requestNotifications();
  bool sendHeartbeat();
  bool tare();
  bool startTimer();
  bool stopTimer();
  bool resetTimer();
  bool requestSettings();

  LunarReading decode(const uint8_t* data, size_t length);

private:
  BLERemoteCharacteristic* commandChar_ = nullptr;

  bool writeBytes(const uint8_t* data, size_t length);
  bool writeCommand(uint8_t messageType, const uint8_t* payload, size_t payloadLength);
};
