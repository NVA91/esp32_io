#pragma once

#include <Arduino.h>
#include <WiFi.h>

class WifiService {
 public:
  void begin();
  void loop();
  bool isConnected() const;
  String ipAddress() const;

 private:
  unsigned long lastAttemptMs_ = 0;
  void connect();
};
