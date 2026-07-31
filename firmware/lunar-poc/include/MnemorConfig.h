#pragma once

// Copy this file to MnemorConfig.local.h if you want private per-device values.
// MnemorConfig.local.h is gitignored.
#if __has_include("MnemorConfig.local.h")
#include "MnemorConfig.local.h"
#endif

#ifndef LUNAR_TARGET_MAC
// Optional. Example: "34:81:f4:e0:1b:ff". Leave empty to match by name.
#define LUNAR_TARGET_MAC ""
#endif

#ifndef LUNAR_NAME_MATCH
// Case-insensitive substring used when no target MAC is configured.
#define LUNAR_NAME_MATCH "lunar"
#endif

#ifndef SERIAL_BAUD
#define SERIAL_BAUD 115200
#endif

#ifndef BLE_SCAN_SECONDS
#define BLE_SCAN_SECONDS 3
#endif

#ifndef BLE_SCAN_LOG_ALL
#define BLE_SCAN_LOG_ALL 0
#endif

#ifndef HEARTBEAT_INTERVAL_MS
// LunarGateway notes the Lunar disconnects after roughly 3000 ms without traffic.
#define HEARTBEAT_INTERVAL_MS 2500
#endif

#ifndef CONNECTION_STALE_MS
#define CONNECTION_STALE_MS 1500
#endif

#ifndef RECONNECT_DELAY_MS
#define RECONNECT_DELAY_MS 1000
#endif

#ifndef OLED_ENABLED
#define OLED_ENABLED 1
#endif

#ifndef OLED_WIDTH
#define OLED_WIDTH 128
#endif

#ifndef OLED_HEIGHT
#define OLED_HEIGHT 64
#endif

#ifndef OLED_I2C_ADDR
#define OLED_I2C_ADDR 0x3C
#endif

#ifndef OLED_SDA_PIN
// Adjust these pins for the exact Waveshare ESP32-S3 board if needed.
#define OLED_SDA_PIN 8
#endif

#ifndef OLED_SCL_PIN
#define OLED_SCL_PIN 9
#endif
