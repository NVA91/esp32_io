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

constexpr char kTimeZone[] = "CET-1CEST,M3.5.0,M10.5.0/3";
constexpr char kNtpServerPrimary[] = "pool.ntp.org";
constexpr char kNtpServerSecondary[] = "time.nist.gov";
constexpr char kNtpServerTertiary[] = "time.google.com";
constexpr char kConfigFilePath[] = "/config.ini";
constexpr char kSecretsFilePath[] = "/secrets.ini";

constexpr unsigned long kWifiConnectTimeoutMs = 15000UL;
constexpr char kPortalPassword[] = "esp32setup";

constexpr uint32_t kWatchdogTimeoutSeconds = 10;

constexpr bool kEnableDisplay = true;
constexpr bool kEnableTouch = true;

}  // namespace AppConfig
