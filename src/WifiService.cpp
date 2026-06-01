#include "WifiService.h"

#include "AppConfig.h"
#include "Logger.h"

void WifiService::begin() {
  WiFi.mode(WIFI_STA);
  connect();
}

void WifiService::loop() {
  if (WiFi.status() == WL_CONNECTED) {
    return;
  }

  const unsigned long now = millis();
  if (now - lastAttemptMs_ < 10000UL) {
    return;
  }

  connect();
}

bool WifiService::isConnected() const { return WiFi.status() == WL_CONNECTED; }

String WifiService::ipAddress() const {
  if (!isConnected()) {
    return String("0.0.0.0");
  }
  return WiFi.localIP().toString();
}

void WifiService::connect() {
  lastAttemptMs_ = millis();
  Logger::info(String("Verbinde WLAN: ") + AppConfig::kWifiSsid);
  WiFi.begin(AppConfig::kWifiSsid, AppConfig::kWifiPassword);
}
