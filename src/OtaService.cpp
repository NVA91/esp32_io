#include "OtaService.h"

#include <ArduinoOTA.h>

#include "AppConfig.h"
#include "Logger.h"

void OtaService::begin() {
  ArduinoOTA.setHostname(AppConfig::kOtaHostname);
  ArduinoOTA.setPassword(AppConfig::kOtaPassword);
  ArduinoOTA.onStart([]() { Logger::info("OTA Start"); });
  ArduinoOTA.onEnd([]() { Logger::info("OTA Ende"); });
  ArduinoOTA.onError([](ota_error_t error) {
    Logger::error(String("OTA Fehler: ") + static_cast<int>(error));
  });
  ArduinoOTA.begin();
  Logger::info("OTA bereit");
}

void OtaService::loop() { ArduinoOTA.handle(); }
