#include <Arduino.h>

#include "AppConfig.h"
#include "BoardProfile.h"
#include "ConfigService.h"
#include "DisplayService.h"
#include "FileSystemService.h"
#include "Logger.h"
#include "MqttService.h"
#include "OtaService.h"
#include "TimeService.h"
#include "WatchdogService.h"
#include "WebServerService.h"
#include "WifiService.h"

namespace {

FileSystemService fileSystem;
ConfigService config(fileSystem);
DisplayService display;
WifiService wifi(config);
OtaService ota(config);
MqttService mqtt(config);
TimeService timeService(wifi, config);
WatchdogService watchdog;
WebServerService webServer(fileSystem, config, wifi, mqtt, timeService, watchdog, display);

void publishHeartbeat() {
  static unsigned long lastPublishMs = 0;
  const unsigned long now = millis();
  if (now - lastPublishMs < 30000UL) {
    return;
  }

  lastPublishMs = now;
  mqtt.publishStatus(String("{\"device\":\"") + config.settings().deviceName +
                     "\",\"ip\":\"" + wifi.ipAddress() +
                     "\",\"time\":\"" + timeService.iso8601() + "\"}");
}

}  // namespace

void setup() {
  Logger::begin();
  Logger::info(String("Board-Profil: ") + BoardProfile::kProfile.boardName);
  fileSystem.begin();
  config.begin();
  display.begin();

  wifi.begin();
  timeService.begin();
  ota.begin();
  mqtt.begin();
  watchdog.begin();
  webServer.begin();

  display.showStatus("WiFi", "Verbinde...");
}

void loop() {
  wifi.loop();
  timeService.loop();
  ota.loop();
  mqtt.loop();
  webServer.loop();
  publishHeartbeat();
  watchdog.feed();

  if (wifi.isConnected()) {
    display.showStatus("Online", timeService.isSynchronized() ? timeService.iso8601() : wifi.ipAddress());
  }

  delay(10);
}
