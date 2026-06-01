#include "WebServerService.h"

#include <LittleFS.h>

#include "BoardProfile.h"
#include "ConfigService.h"
#include "DisplayService.h"
#include "FileSystemService.h"
#include "Logger.h"
#include "MqttService.h"
#include "TimeService.h"
#include "WatchdogService.h"
#include "WifiService.h"

namespace {

uint16_t parsePortArg(const String& value, uint16_t fallback) {
  const long parsed = value.toInt();
  if (parsed <= 0 || parsed > 65535) {
    return fallback;
  }
  return static_cast<uint16_t>(parsed);
}

}  // namespace

WebServerService::WebServerService(FileSystemService& fileSystem, ConfigService& config,
                                   WifiService& wifi, MqttService& mqtt, TimeService& time,
                                   WatchdogService& watchdog, DisplayService& display)
    : fileSystem_(fileSystem),
      config_(config),
      wifi_(wifi),
      mqtt_(mqtt),
      time_(time),
      watchdog_(watchdog),
      display_(display) {}

void WebServerService::begin() {
  registerRoutes();
  server_.begin();
  Logger::info("Webserver bereit");
}

void WebServerService::loop() { server_.handleClient(); }

void WebServerService::registerRoutes() {
  auto captiveRedirect = [this]() {
    server_.sendHeader("Location", String("http://") + wifi_.ipAddress() + "/", true);
    server_.send(302, "text/plain", "Portal");
  };

  server_.on("/", HTTP_GET, [this]() {
    if (fileSystem_.hasFile("/index.html")) {
      File file = LittleFS.open("/index.html", "r");
      server_.streamFile(file, "text/html; charset=utf-8");
      file.close();
      return;
    }
    server_.send(200, "text/plain", "ESP32 template online");
  });

  server_.on("/api/status", HTTP_GET, [this]() {
    String payload = "{";
    payload += "\"board\":\"";
    payload += BoardProfile::kProfile.boardName;
    payload += "\",";
    payload += "\"display\":{\"width\":";
    payload += BoardProfile::kProfile.displayWidth;
    payload += ",\"height\":";
    payload += BoardProfile::kProfile.displayHeight;
    payload += ",\"bus\":\"";
    payload += BoardProfile::kProfile.displayBus;
    payload += "\",\"touch\":";
    payload += BoardProfile::kProfile.touchSupported ? "true" : "false";
    payload += "},";
    payload += "\"wifi\":";
    payload += wifi_.isConnected() ? "true" : "false";
    payload += ",\"wifiMode\":\"";
    payload += wifi_.modeName();
    payload += "\",\"activeSsid\":\"";
    payload += wifi_.activeSsid();
    payload += "\"";
    payload += ",\"ip\":\"" + wifi_.ipAddress() + "\"";
    payload += ",\"mqtt\":";
    payload += mqtt_.isConnected() ? "true" : "false";
    payload += ",\"mqttConfigured\":";
    payload += mqtt_.isConfigured() ? "true" : "false";
    payload += ",\"timeSynchronized\":";
    payload += time_.isSynchronized() ? "true" : "false";
    payload += ",\"time\":\"";
    payload += time_.iso8601();
    payload += "\",";
    payload += "\"configOverrides\":";
    payload += config_.hasLocalOverrides() ? "true" : "false";
    payload += ",\"secretOverrides\":";
    payload += config_.hasSecretOverrides() ? "true" : "false";
    payload += ",\"displayBackend\":\"";
    payload += display_.backendName();
    payload += "\",\"displayReady\":";
    payload += display_.isReady() ? "true" : "false";
    payload += ",\"watchdog\":{\"enabled\":";
    payload += watchdog_.isEnabled() ? "true" : "false";
    payload += ",\"timeoutSeconds\":";
    payload += watchdog_.timeoutSeconds();
    payload += "}";
    payload += "}";
    server_.send(200, "application/json", payload);
  });

  server_.on("/api/config", HTTP_GET,
             [this]() { server_.send(200, "application/json", config_.publicJson()); });

  server_.on("/api/config", HTTP_POST, [this]() {
    ConfigService::Settings updated = config_.editableCopy();
    ConfigService::Secrets secrets = config_.editableSecretsCopy();

    if (server_.hasArg("deviceName")) {
      updated.deviceName = server_.arg("deviceName");
    }
    if (server_.hasArg("wifiSsid")) {
      updated.wifiSsid = server_.arg("wifiSsid");
    }
    if (server_.hasArg("wifiPassword") && !server_.arg("wifiPassword").isEmpty()) {
      secrets.wifiPassword = server_.arg("wifiPassword");
    }
    if (server_.hasArg("mqttHost")) {
      updated.mqttHost = server_.arg("mqttHost");
    }
    if (server_.hasArg("mqttUsername")) {
      updated.mqttUsername = server_.arg("mqttUsername");
    }
    if (server_.hasArg("mqttPort")) {
      updated.mqttPort = parsePortArg(server_.arg("mqttPort"), updated.mqttPort);
    }
    if (server_.hasArg("mqttClientId")) {
      updated.mqttClientId = server_.arg("mqttClientId");
    }
    if (server_.hasArg("mqttTopicState")) {
      updated.mqttTopicState = server_.arg("mqttTopicState");
    }
    if (server_.hasArg("mqttTopicCommand")) {
      updated.mqttTopicCommand = server_.arg("mqttTopicCommand");
    }
    if (server_.hasArg("otaHostname")) {
      updated.otaHostname = server_.arg("otaHostname");
    }
    if (server_.hasArg("otaPassword") && !server_.arg("otaPassword").isEmpty()) {
      secrets.otaPassword = server_.arg("otaPassword");
    }
    if (server_.hasArg("mqttPassword") && !server_.arg("mqttPassword").isEmpty()) {
      secrets.mqttPassword = server_.arg("mqttPassword");
    }
    if (server_.hasArg("timeZone")) {
      updated.timeZone = server_.arg("timeZone");
    }
    if (server_.hasArg("ntpServerPrimary")) {
      updated.ntpServerPrimary = server_.arg("ntpServerPrimary");
    }
    if (server_.hasArg("ntpServerSecondary")) {
      updated.ntpServerSecondary = server_.arg("ntpServerSecondary");
    }
    if (server_.hasArg("ntpServerTertiary")) {
      updated.ntpServerTertiary = server_.arg("ntpServerTertiary");
    }

    const bool saved = config_.update(updated) && config_.updateSecrets(secrets);
    const int code = saved ? 200 : 500;
    String payload = "{";
    payload += "\"saved\":";
    payload += saved ? "true" : "false";
    payload += ",\"restartRequired\":true}";
    server_.send(code, "application/json", payload);
  });

  server_.on("/api/time", HTTP_GET, [this]() {
    String payload = "{";
    payload += "\"synchronized\":";
    payload += time_.isSynchronized() ? "true" : "false";
    payload += ",\"value\":\"";
    payload += time_.iso8601();
    payload += "\",\"status\":\"";
    payload += time_.statusText();
    payload += "\"}";
    server_.send(200, "application/json", payload);
  });

  server_.on("/api/restart", HTTP_POST, [this]() {
    server_.send(200, "application/json", "{\"restarting\":true}");
    delay(200);
    ESP.restart();
  });

  server_.on("/generate_204", HTTP_GET, captiveRedirect);
  server_.on("/hotspot-detect.html", HTTP_GET, captiveRedirect);
  server_.on("/connecttest.txt", HTTP_GET, captiveRedirect);
  server_.on("/ncsi.txt", HTTP_GET, captiveRedirect);
  server_.on("/fwlink", HTTP_GET, captiveRedirect);

  server_.onNotFound([this, captiveRedirect]() {
    if (wifi_.isPortalActive()) {
      captiveRedirect();
      return;
    }
    server_.send(404, "text/plain", "Not found");
  });
}
