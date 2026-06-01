#pragma once

#include <Arduino.h>
#include <DNSServer.h>
#include <WiFi.h>

class ConfigService;

class WifiService {
 public:
  explicit WifiService(const ConfigService& config);

  void begin();
  void loop();
  bool isConnected() const;
  bool isPortalActive() const;
  String ipAddress() const;
  String activeSsid() const;
  String modeName() const;

 private:
  const ConfigService& config_;
  DNSServer dnsServer_;
  bool portalActive_ = false;
  String portalSsid_;
  unsigned long lastAttemptMs_ = 0;
  unsigned long connectStartedMs_ = 0;

  void connect();
  void startPortal();
};
