#pragma once

#include <Arduino.h>

class ConfigService;

class OtaService {
 public:
  explicit OtaService(const ConfigService& config);

  void begin();
  void loop();

 private:
  const ConfigService& config_;
};
