#pragma once

#include "Arduino.h"

#include <ArduinoJson.h>
#include "Limits.hpp"
#include "../HomieSetting.hpp"

namespace HomieInternals {
struct ConfigValidationResult {
  bool valid;
  String reason;
};

class Helpers {
 public:
  static void generateDeviceId();
  static const char* getDeviceId();
  static uint8_t rssiToPercentage(int32_t rssi);
  static ConfigValidationResult validateConfig(const JsonObject& object);

 private:
  static char _deviceId[8 + 1];

  static ConfigValidationResult _validateConfigRoot(const JsonObject& object);
  static ConfigValidationResult _validateConfigWifi(const JsonObject& object);
  static ConfigValidationResult _validateConfigMqtt(const JsonObject& object);
  static ConfigValidationResult _validateConfigOta(const JsonObject& object);
  static ConfigValidationResult _validateConfigSettings(const JsonObject& object);
};
}  // namespace HomieInternals
