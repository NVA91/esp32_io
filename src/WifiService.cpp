#include "WifiService.h"

#include "AppConfig.h"
#include "ConfigService.h"
#include "Logger.h"

WifiService::WifiService(const ConfigService& config) : config_(config) {}

void WifiService::begin() {
  WiFi.mode(WIFI_STA);
  connect();
}

void WifiService::loop() {
  if (portalActive_) {
    dnsServer_.processNextRequest();
  }

  if (WiFi.status() == WL_CONNECTED) {
    connectStartedMs_ = 0;
    return;
  }

  if (!portalActive_ && connectStartedMs_ > 0 && millis() - connectStartedMs_ >= AppConfig::kWifiConnectTimeoutMs) {
    startPortal();
    return;
  }

  const unsigned long now = millis();
  if (portalActive_) {
    return;
  }

  if (now - lastAttemptMs_ < 10000UL) {
    return;
  }

  connect();
}

bool WifiService::isConnected() const { return WiFi.status() == WL_CONNECTED; }

bool WifiService::isPortalActive() const { return portalActive_; }

String WifiService::ipAddress() const {
  if (portalActive_) {
    return WiFi.softAPIP().toString();
  }
  if (!isConnected()) {
    return String("0.0.0.0");
  }
  return WiFi.localIP().toString();
}

String WifiService::activeSsid() const {
  if (portalActive_) {
    return portalSsid_;
  }
  return config_.settings().wifiSsid;
}

String WifiService::modeName() const { return portalActive_ ? String("ap") : String("station"); }

void WifiService::connect() {
  lastAttemptMs_ = millis();
  const ConfigService::Settings& settings = config_.settings();
  if (settings.wifiSsid.isEmpty()) {
    Logger::warn("WLAN nicht konfiguriert, starte Setup-AP");
    startPortal();
    return;
  }

  portalActive_ = false;
  connectStartedMs_ = millis();
  WiFi.setHostname(settings.deviceName.c_str());
  Logger::info(String("Verbinde WLAN: ") + settings.wifiSsid);
  WiFi.begin(settings.wifiSsid.c_str(), config_.secrets().wifiPassword.c_str());
}

void WifiService::startPortal() {
  if (portalActive_) {
    return;
  }

  const ConfigService::Settings& settings = config_.settings();
  WiFi.disconnect(true, true);
  WiFi.mode(WIFI_AP_STA);
  portalSsid_ = settings.deviceName + "-setup";
  WiFi.softAP(portalSsid_.c_str(), AppConfig::kPortalPassword);
  dnsServer_.start(53, "*", WiFi.softAPIP());
  portalActive_ = true;
  connectStartedMs_ = 0;
  Logger::warn(String("Setup-AP aktiv: ") + portalSsid_ + " @ " + WiFi.softAPIP().toString());
}
