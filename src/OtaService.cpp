#include "OtaService.h"

#include <ArduinoOTA.h>

#include "ConfigService.h"
#include "Logger.h"

OtaService::OtaService(const ConfigService& config) : config_(config) {}

void OtaService::begin() {
  const ConfigService::Settings& settings = config_.settings();
  ArduinoOTA.setHostname(settings.otaHostname.c_str());
  if (!config_.secrets().otaPassword.isEmpty()) {
    ArduinoOTA.setPassword(config_.secrets().otaPassword.c_str());
  }
  ArduinoOTA.onStart([]() { Logger::info("OTA Start"); });
  ArduinoOTA.onEnd([]() { Logger::info("OTA Ende"); });
  ArduinoOTA.onError([](ota_error_t error) {
    Logger::error(String("OTA Fehler: ") + static_cast<int>(error));
  });
  ArduinoOTA.begin();
  Logger::info("OTA bereit");
}

void OtaService::loop() { ArduinoOTA.handle(); }
