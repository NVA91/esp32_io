#include <Arduino.h>

#include "AppConfig.h"
#include "DisplayService.h"
#include "FileSystemService.h"
#include "Logger.h"
#include "MqttService.h"
#include "OtaService.h"
#include "WebServerService.h"
#include "WifiService.h"

namespace {

FileSystemService fileSystem;
DisplayService display;
WifiService wifi;
OtaService ota;
MqttService mqtt;
WebServerService webServer(fileSystem, wifi, mqtt);

void publishHeartbeat() {
  static unsigned long lastPublishMs = 0;
  const unsigned long now = millis();
  if (now - lastPublishMs < 30000UL) {
    return;
  }

  lastPublishMs = now;
  mqtt.publishStatus(String("{\"device\":\"") + AppConfig::kDeviceName +
                     "\",\"ip\":\"" + wifi.ipAddress() + "\"}");
}

}  // namespace

void setup() {
  Logger::begin();
  display.begin();
  fileSystem.begin();

  wifi.begin();
  ota.begin();
  mqtt.begin();
  webServer.begin();

  display.showStatus("WiFi", "Verbinde...");
}

void loop() {
  wifi.loop();
  ota.loop();
  mqtt.loop();
  webServer.loop();
  publishHeartbeat();

  if (wifi.isConnected()) {
    display.showStatus("Online", wifi.ipAddress());
  }

  delay(10);
}
