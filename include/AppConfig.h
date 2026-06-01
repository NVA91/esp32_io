#pragma once

#include <Arduino.h>

namespace AppConfig {

constexpr char kDeviceName[] = "esp32-template";

constexpr char kWifiSsid[] = "WIFI_SSID";
constexpr char kWifiPassword[] = "WIFI_PASSWORD";

constexpr char kMqttHost[] = "192.168.178.10";
constexpr uint16_t kMqttPort = 1883;
constexpr char kMqttClientId[] = "esp32-template";
constexpr char kMqttTopicState[] = "devices/esp32-template/state";
constexpr char kMqttTopicCommand[] = "devices/esp32-template/cmd";

constexpr char kOtaHostname[] = "esp32-template";
constexpr char kOtaPassword[] = "ota-password";

constexpr bool kEnableDisplay = true;

}  // namespace AppConfig
