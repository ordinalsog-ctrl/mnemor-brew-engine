#include "LunarProtocol.h"

namespace {
constexpr uint8_t kHeader0 = 0xEF;
constexpr uint8_t kHeader1 = 0xDD;

constexpr std::array<uint8_t, 21> kClientId = {
    0xef, 0xdd, 0x0b, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
    0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x9a, 0x6d, 0x00};

constexpr std::array<uint8_t, 14> kNotificationRequest = {
    0xef, 0xdd, 0x0c, 0x09, 0x00, 0x01, 0x01,
    0x02, 0x02, 0x05, 0x03, 0x04, 0x15, 0x06};

float applyLunarDecimal(uint16_t rawWeight, uint8_t unit) {
  float weight = static_cast<float>(rawWeight);
  switch (unit) {
    case 1:
      return weight / 10.0f;
    case 2:
      return weight / 100.0f;
    case 3:
      return weight / 1000.0f;
    case 4:
      return weight / 10000.0f;
    default:
      return weight;
  }
}
}  // namespace

void LunarProtocol::begin(BLERemoteCharacteristic* commandCharacteristic) {
  commandChar_ = commandCharacteristic;
}

bool LunarProtocol::sendId() {
  return writeBytes(kClientId.data(), kClientId.size());
}

bool LunarProtocol::requestNotifications() {
  if (!sendId()) {
    return false;
  }
  return writeBytes(kNotificationRequest.data(), kNotificationRequest.size());
}

bool LunarProtocol::sendHeartbeat() {
  const uint8_t payload[] = {2, 0};
  if (!sendId()) {
    return false;
  }
  return writeCommand(0, payload, sizeof(payload));
}

bool LunarProtocol::tare() {
  const uint8_t payload[] = {0};
  if (!sendId()) {
    return false;
  }
  return writeCommand(4, payload, sizeof(payload));
}

bool LunarProtocol::startTimer() {
  const uint8_t payload[] = {0, 0};
  if (!sendId()) {
    return false;
  }
  return writeCommand(13, payload, sizeof(payload));
}

bool LunarProtocol::stopTimer() {
  const uint8_t payload[] = {0, 2};
  if (!sendId()) {
    return false;
  }
  return writeCommand(13, payload, sizeof(payload));
}

bool LunarProtocol::resetTimer() {
  const uint8_t payload[] = {0, 1};
  if (!sendId()) {
    return false;
  }
  return writeCommand(13, payload, sizeof(payload));
}

bool LunarProtocol::requestSettings() {
  uint8_t payload[16] = {0};
  if (!sendId()) {
    return false;
  }
  return writeCommand(6, payload, sizeof(payload));
}

LunarReading LunarProtocol::decode(const uint8_t* data, size_t length) {
  LunarReading reading;
  if (data == nullptr || length < 5) {
    return reading;
  }

  size_t start = length;
  for (size_t i = 0; i + 1 < length; ++i) {
    if (data[i] == kHeader0 && data[i + 1] == kHeader1) {
      start = i;
      break;
    }
  }

  if (start == length || start + 5 > length) {
    return reading;
  }

  const uint8_t topic = data[start + 2];
  const uint8_t payloadLength = data[start + 3];
  const size_t end = start + static_cast<size_t>(payloadLength) + 5;
  if (end > length) {
    return reading;
  }

  if (topic == 0x08 && start + 4 < length) {
    reading.hasBattery = true;
    reading.batteryPercent = data[start + 4] & 0x7F;
  }

  const uint8_t messageType = data[start + 4];
  if (topic == 0x0C && messageType == 0x05 && end >= start + 11) {
    const uint8_t* payload = data + start + 5;
    const uint16_t rawWeight =
        (static_cast<uint16_t>(payload[1]) << 8) | static_cast<uint16_t>(payload[0]);
    float weight = applyLunarDecimal(rawWeight, payload[4]);
    if ((payload[5] & 0x02) == 0x02) {
      weight *= -1.0f;
    }
    reading.hasWeight = true;
    reading.weightGrams = weight;
  }

  return reading;
}

bool LunarProtocol::writeBytes(const uint8_t* data, size_t length) {
  if (commandChar_ == nullptr || data == nullptr || length == 0) {
    return false;
  }
  commandChar_->writeValue(const_cast<uint8_t*>(data), length, false);
  return true;
}

bool LunarProtocol::writeCommand(uint8_t messageType, const uint8_t* payload, size_t payloadLength) {
  if (payloadLength + 5 > 32) {
    return false;
  }

  uint8_t request[32] = {0};
  request[0] = kHeader0;
  request[1] = kHeader1;
  request[2] = messageType;

  uint8_t checksum1 = 0;
  uint8_t checksum2 = 0;
  for (size_t i = 0; i < payloadLength; ++i) {
    const uint8_t value = payload[i] & 0xFF;
    request[3 + i] = value;
    if (i % 2 == 0) {
      checksum1 += value;
    } else {
      checksum2 += value;
    }
  }

  request[payloadLength + 3] = checksum1 & 0xFF;
  request[payloadLength + 4] = checksum2 & 0xFF;
  return writeBytes(request, payloadLength + 5);
}
