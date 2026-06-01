#include "WebServerService.h"

#include <LittleFS.h>

#include "FileSystemService.h"
#include "Logger.h"
#include "MqttService.h"
#include "WifiService.h"

WebServerService::WebServerService(FileSystemService& fileSystem, WifiService& wifi,
                                   MqttService& mqtt)
    : fileSystem_(fileSystem), wifi_(wifi), mqtt_(mqtt) {}

void WebServerService::begin() {
  registerRoutes();
  server_.begin();
  Logger::info("Webserver bereit");
}

void WebServerService::loop() { server_.handleClient(); }

void WebServerService::registerRoutes() {
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
    payload += "\"wifi\":";
    payload += wifi_.isConnected() ? "true" : "false";
    payload += ",\"ip\":\"" + wifi_.ipAddress() + "\"";
    payload += ",\"mqtt\":";
    payload += mqtt_.isConnected() ? "true" : "false";
    payload += "}";
    server_.send(200, "application/json", payload);
  });

  server_.onNotFound([this]() { server_.send(404, "text/plain", "Not found"); });
}
