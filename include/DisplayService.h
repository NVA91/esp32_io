#pragma once

#include <Arduino.h>

class DisplayService {
 public:
  void begin();
  void showStatus(const String& headline, const String& detail);

 private:
  bool enabled_ = false;
};
