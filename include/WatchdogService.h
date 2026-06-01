#pragma once

#include <Arduino.h>

class WatchdogService {
 public:
  void begin();
  void feed();
  bool isEnabled() const;
  uint32_t timeoutSeconds() const;

 private:
  bool enabled_ = false;
};
