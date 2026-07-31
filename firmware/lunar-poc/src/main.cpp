#include <Arduino.h>
#include <BLEDevice.h>
#include <cstring>
#include <map>

#include "LunarProtocol.h"
#include "MnemorConfig.h"
#include "OledStatus.h"

namespace {
BLEUUID serviceUuid("49535343-FE7D-4AE5-8FA9-9FAFD205E455");
BLEUUID commandUuid("49535343-8841-43f4-a8d4-ecbe34729bb3");
BLEUUID dataUuid("49535343-1e4d-4bd9-ba61-23c647249616");
BLEUUID legacyReadWriteUuid((uint16_t)0x2A80);

constexpr uint8_t kLunarHeader0 = 0xEF;
constexpr uint8_t kLunarHeader1 = 0xDD;
constexpr size_t kLunarRxBufferSize = 128;

BLEAdvertisedDevice* targetDevice = nullptr;
BLEClient* client = nullptr;
BLERemoteCharacteristic* commandChar = nullptr;
BLERemoteCharacteristic* dataChar = nullptr;
uint8_t lunarRxBuffer[kLunarRxBufferSize] = {0};
size_t lunarRxLength = 0;

LunarProtocol lunar;
OledStatus oled;

bool shouldConnect = false;
bool connected = false;
bool scanning = false;
bool csvHeaderPrinted = false;
bool debugPayloads = BLE_PAYLOAD_LOG_ALL;
bool scanDebug = BLE_SCAN_LOG_ALL;

unsigned long lastHeartbeatMs = 0;
unsigned long lastDataMs = 0;
unsigned long lastReconnectAttemptMs = 0;
unsigned long lastDisplayMs = 0;
unsigned long lastStatusLogMs = 0;

float latestWeightGrams = 0.0f;
int latestBatteryPercent = -1;
volatile bool hasNewWeight = false;

String lowerCopy(String value) {
  value.toLowerCase();
  return value;
}

bool targetMacConfigured() {
  return String(LUNAR_TARGET_MAC).length() > 0;
}

String uuidToLowerString(BLEUUID uuid) {
  return lowerCopy(String(uuid.toString().c_str()));
}

bool uuidMatches(BLERemoteCharacteristic* characteristic, BLEUUID uuid) {
  return characteristic != nullptr &&
         uuidToLowerString(characteristic->getUUID()) == uuidToLowerString(uuid);
}

bool matchesTarget(BLEAdvertisedDevice& advertisedDevice) {
  const String address = advertisedDevice.getAddress().toString().c_str();

  if (targetMacConfigured()) {
    return lowerCopy(address) == lowerCopy(String(LUNAR_TARGET_MAC));
  }

  String name = "";
  if (advertisedDevice.haveName()) {
    name = advertisedDevice.getName().c_str();
  }
  const String lowerName = lowerCopy(name);
  return lowerName.indexOf(lowerCopy(String(LUNAR_NAME_MATCH))) >= 0 ||
         lowerName.indexOf("acaia") >= 0;
}

void printAdvertisedDevice(const char* prefix, const String& name, const String& address, int rssi) {
  Serial.print(prefix);
  Serial.print(" name=");
  Serial.print(name.length() ? name : "(no name)");
  Serial.print(" address=");
  Serial.print(address);
  Serial.print(" rssi=");
  Serial.println(rssi);
}

void printCharacteristicSummary(BLERemoteCharacteristic* characteristic) {
  if (characteristic == nullptr) {
    return;
  }

  Serial.print("[BLE] characteristic=");
  Serial.print(characteristic->getUUID().toString().c_str());
  Serial.print(" props=");
  Serial.print(characteristic->canRead() ? "r" : "-");
  Serial.print(characteristic->canWrite() ? "w" : "-");
  Serial.print(characteristic->canWriteNoResponse() ? "W" : "-");
  Serial.print(characteristic->canNotify() ? "n" : "-");
  Serial.println(characteristic->canIndicate() ? "i" : "-");
}

bool discoverLunarCharacteristics() {
  commandChar = nullptr;
  dataChar = nullptr;

  std::map<std::string, BLERemoteService*>* services = client->getServices();
  if (services == nullptr || services->empty()) {
    Serial.println("[BLE] no services discovered");
    return false;
  }

  Serial.print("[BLE] service count=");
  Serial.println(services->size());

  for (const auto& serviceEntry : *services) {
    BLERemoteService* service = serviceEntry.second;
    if (service == nullptr) {
      continue;
    }

    const String serviceId = uuidToLowerString(service->getUUID());
    Serial.print("[BLE] service=");
    Serial.println(service->getUUID().toString().c_str());

    std::map<std::string, BLERemoteCharacteristic*>* characteristics =
        service->getCharacteristics();
    if (characteristics == nullptr) {
      continue;
    }

    for (const auto& characteristicEntry : *characteristics) {
      BLERemoteCharacteristic* characteristic = characteristicEntry.second;
      printCharacteristicSummary(characteristic);

      if (uuidMatches(characteristic, commandUuid)) {
        commandChar = characteristic;
        Serial.println("[BLE] command characteristic matched");
      }

      if (uuidMatches(characteristic, dataUuid)) {
        dataChar = characteristic;
        Serial.println("[BLE] data characteristic matched");
      }

      if (uuidMatches(characteristic, legacyReadWriteUuid)) {
        commandChar = characteristic;
        dataChar = characteristic;
        Serial.println("[BLE] legacy read/write characteristic matched");
      }
    }

    if (serviceId == uuidToLowerString(serviceUuid) && commandChar != nullptr &&
        dataChar != nullptr) {
      Serial.println("[BLE] primary service characteristics resolved");
    }
  }

  return commandChar != nullptr && dataChar != nullptr;
}

void printPayloadHex(const uint8_t* data, size_t length) {
  Serial.print("[BLE] payload=");
  for (size_t i = 0; i < length; ++i) {
    if (data[i] < 0x10) {
      Serial.print("0");
    }
    Serial.print(data[i], HEX);
    if (i + 1 < length) {
      Serial.print(" ");
    }
  }
  Serial.println();
}

void printFrameHex(const uint8_t* data, size_t length) {
  Serial.print("[BLE] frame=");
  for (size_t i = 0; i < length; ++i) {
    if (data[i] < 0x10) {
      Serial.print("0");
    }
    Serial.print(data[i], HEX);
    if (i + 1 < length) {
      Serial.print(" ");
    }
  }
  Serial.println();
}

void printCsv(float weightGrams) {
  if (!csvHeaderPrinted) {
    Serial.println("timestamp_ms,weight_g,battery_percent,connected");
    csvHeaderPrinted = true;
  }
  Serial.print(millis());
  Serial.print(",");
  Serial.print(weightGrams, 2);
  Serial.print(",");
  Serial.print(latestBatteryPercent);
  Serial.print(",");
  Serial.println(connected ? 1 : 0);
}

void handleLunarReading(const LunarReading& reading) {
  if (reading.hasBattery) {
    latestBatteryPercent = reading.batteryPercent;
    Serial.print("[SCALE] battery=");
    Serial.println(latestBatteryPercent);
  }
  if (reading.hasWeight) {
    latestWeightGrams = reading.weightGrams;
    hasNewWeight = true;
  }
}

void dropRxBytes(size_t count) {
  if (count >= lunarRxLength) {
    lunarRxLength = 0;
    return;
  }

  const size_t remaining = lunarRxLength - count;
  std::memmove(lunarRxBuffer, lunarRxBuffer + count, remaining);
  lunarRxLength = remaining;
}

void consumeLunarRxBuffer() {
  while (lunarRxLength >= 2) {
    size_t headerOffset = 0;
    while (headerOffset + 1 < lunarRxLength &&
           !(lunarRxBuffer[headerOffset] == kLunarHeader0 &&
             lunarRxBuffer[headerOffset + 1] == kLunarHeader1)) {
      ++headerOffset;
    }

    if (headerOffset > 0) {
      dropRxBytes(headerOffset);
    }

    if (lunarRxLength < 5) {
      return;
    }

    if (lunarRxBuffer[0] != kLunarHeader0 || lunarRxBuffer[1] != kLunarHeader1) {
      dropRxBytes(1);
      continue;
    }

    const size_t frameLength = static_cast<size_t>(lunarRxBuffer[3]) + 5;
    if (frameLength > kLunarRxBufferSize) {
      Serial.println("[BLE] invalid frame length; resyncing");
      dropRxBytes(1);
      continue;
    }

    if (lunarRxLength < frameLength) {
      return;
    }

    if (debugPayloads) {
      printFrameHex(lunarRxBuffer, frameLength);
    }

    handleLunarReading(lunar.decode(lunarRxBuffer, frameLength));
    dropRxBytes(frameLength);
  }
}

void appendLunarBytes(const uint8_t* data, size_t length) {
  if (data == nullptr || length == 0) {
    return;
  }

  if (length > kLunarRxBufferSize) {
    data += length - kLunarRxBufferSize;
    length = kLunarRxBufferSize;
    lunarRxLength = 0;
  }

  if (lunarRxLength + length > kLunarRxBufferSize) {
    Serial.println("[BLE] rx buffer overflow; resetting");
    lunarRxLength = 0;
  }

  std::memcpy(lunarRxBuffer + lunarRxLength, data, length);
  lunarRxLength += length;
  consumeLunarRxBuffer();
}

void notifyCallback(
    BLERemoteCharacteristic*,
    uint8_t* data,
    size_t length,
    bool) {
  lastDataMs = millis();
  if (debugPayloads) {
    printPayloadHex(data, length);
  }

  appendLunarBytes(data, length);
}

class AdvertisedCallbacks : public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) override {
    String name = advertisedDevice.haveName() ? advertisedDevice.getName().c_str() : "";
    String address = advertisedDevice.getAddress().toString().c_str();

    const bool matches = matchesTarget(advertisedDevice);
    if (scanDebug && !matches) {
      printAdvertisedDevice("[BLE] seen", name, address, advertisedDevice.getRSSI());
    }

    if (matches) {
      printAdvertisedDevice("[BLE] found", name, address, advertisedDevice.getRSSI());

      oled.showFound(name.length() ? name : address, advertisedDevice.getRSSI());
      BLEDevice::getScan()->stop();

      if (targetDevice != nullptr) {
        delete targetDevice;
      }
      targetDevice = new BLEAdvertisedDevice(advertisedDevice);
      shouldConnect = true;
      scanning = false;
    }
  }
};

AdvertisedCallbacks advertisedCallbacks;

class ClientCallbacks : public BLEClientCallbacks {
  void onConnect(BLEClient*) override {}

  void onDisconnect(BLEClient*) override {
    connected = false;
    commandChar = nullptr;
    dataChar = nullptr;
    lunarRxLength = 0;
    Serial.println("[BLE] disconnected");
    oled.showReconnect();
  }
};

void startScan() {
  if (scanning || shouldConnect || connected) {
    return;
  }

  Serial.println("[BLE] scan started");
  Serial.println("[BLE] Searching...");
  oled.showSearching();

  BLEScan* scan = BLEDevice::getScan();
  scan->setAdvertisedDeviceCallbacks(&advertisedCallbacks, false);
  scan->setInterval(1349);
  scan->setWindow(449);
  scan->setActiveScan(true);
  scanning = true;
  scan->start(BLE_SCAN_SECONDS, false);
  scanning = false;
}

bool connectToLunar() {
  if (targetDevice == nullptr) {
    Serial.println("[BLE] connect skipped: no target");
    return false;
  }

  Serial.print("[BLE] connecting address=");
  Serial.println(targetDevice->getAddress().toString().c_str());

  if (client != nullptr) {
    delete client;
    client = nullptr;
  }

  client = BLEDevice::createClient();
  client->setClientCallbacks(new ClientCallbacks());

  if (!client->connect(targetDevice)) {
    Serial.println("[BLE] connect failed");
    return false;
  }

  Serial.println("[BLE] connected");
  client->setMTU(517);

  if (!discoverLunarCharacteristics()) {
    Serial.println("[BLE] required characteristics not found");
    client->disconnect();
    return false;
  }
  Serial.println("[BLE] required characteristics found");

  lunar.begin(commandChar);

  if (dataChar->canNotify() || dataChar->canIndicate()) {
    dataChar->registerForNotify(notifyCallback, dataChar->canNotify());
    Serial.println("[BLE] notifications subscribed");
  } else {
    Serial.println("[BLE] data characteristic cannot notify");
    client->disconnect();
    return false;
  }

  connected = true;
  lunarRxLength = 0;
  lastDataMs = millis();
  lastHeartbeatMs = 0;

  lunar.requestNotifications();
  delay(100);
  lunar.requestSettings();
  oled.showConnected(latestWeightGrams, latestBatteryPercent);
  return true;
}

void handleSerialCommands() {
  if (!Serial.available()) {
    return;
  }

  const String command = Serial.readStringUntil('\n');
  const String trimmed = lowerCopy(command);

  if (trimmed == "t") {
    Serial.println(lunar.tare() ? "[SCALE] tare sent" : "[SCALE] tare failed");
  } else if (trimmed == "n") {
    Serial.println(lunar.requestNotifications() ? "[SCALE] notification request sent"
                                                : "[SCALE] notification request failed");
  } else if (trimmed == "b") {
    Serial.println(lunar.requestSettings() ? "[SCALE] settings request sent"
                                           : "[SCALE] settings request failed");
  } else if (trimmed == "s") {
    Serial.println(lunar.startTimer() ? "[SCALE] timer start sent"
                                      : "[SCALE] timer start failed");
  } else if (trimmed == "h") {
    Serial.println(lunar.stopTimer() ? "[SCALE] timer stop sent"
                                     : "[SCALE] timer stop failed");
  } else if (trimmed == "r") {
    Serial.println(lunar.resetTimer() ? "[SCALE] timer reset sent"
                                      : "[SCALE] timer reset failed");
  } else if (trimmed == "m") {
    debugPayloads = !debugPayloads;
    Serial.print("[BLE] debug_payloads=");
    Serial.println(debugPayloads ? 1 : 0);
  } else if (trimmed == "a") {
    scanDebug = !scanDebug;
    Serial.print("[BLE] scan_debug=");
    Serial.println(scanDebug ? 1 : 0);
  } else if (trimmed == "?") {
    Serial.println("[CMD] t=tare n=notifications b=battery/settings s=start h=stop r=reset m=payload debug a=scan debug");
  }
}

void maintainConnection() {
  if (shouldConnect) {
    shouldConnect = false;
    if (!connectToLunar()) {
      connected = false;
      lastReconnectAttemptMs = millis();
      oled.showReconnect();
    }
    return;
  }

  if (connected) {
    if (millis() - lastHeartbeatMs >= HEARTBEAT_INTERVAL_MS) {
      lunar.requestNotifications();
      delay(50);
      lunar.sendHeartbeat();
      lastHeartbeatMs = millis();
      Serial.println("[BLE] heartbeat sent");
    }

    if (millis() - lastDataMs >= CONNECTION_STALE_MS &&
        millis() - lastStatusLogMs >= CONNECTION_STALE_MS) {
      Serial.println("[BLE] stale stream: waiting for next notification");
      lastStatusLogMs = millis();
    }
    return;
  }

  if (millis() - lastReconnectAttemptMs >= RECONNECT_DELAY_MS) {
    lastReconnectAttemptMs = millis();
    startScan();
  }
}
}  // namespace

void setup() {
  Serial.begin(SERIAL_BAUD);
  delay(300);

  Serial.println();
  Serial.println("[BOOT] Mnemor Brew Engine Lunar PoC");
  Serial.println("[BOOT] M1 target: Lunar BLE weight stream only");
  Serial.print("[BOOT] target_mac=");
  Serial.println(targetMacConfigured() ? LUNAR_TARGET_MAC : "(name scan)");

  oled.begin();

  BLEDevice::init("MnemorBrewEngine");
  startScan();
}

void loop() {
  handleSerialCommands();
  maintainConnection();

  if (hasNewWeight) {
    hasNewWeight = false;
    printCsv(latestWeightGrams);
    Serial.print("[SCALE] weight=");
    Serial.print(latestWeightGrams, 2);
    Serial.println(" g");
  }

  if (connected && millis() - lastDisplayMs >= 250) {
    oled.showConnected(latestWeightGrams, latestBatteryPercent);
    lastDisplayMs = millis();
  }

  delay(10);
}
