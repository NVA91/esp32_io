#pragma once

#include <Arduino.h>

class ConfigService;
class WifiService;

class TimeService {
 public:
  TimeService(WifiService& wifi, const ConfigService& config);

  void begin();
  void loop();
  bool isSynchronized() const;
  String iso8601() const;
  String statusText() const;

 private:
  WifiService& wifi_;
  const ConfigService& config_;
  bool started_ = false;
  bool synchronized_ = false;
  unsigned long lastSyncAttemptMs_ = 0;

  void startNtp();
  bool updateSyncState();
};