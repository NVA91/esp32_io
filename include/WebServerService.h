#pragma once

#include <WebServer.h>

class FileSystemService;
class ConfigService;
class DisplayService;
class MqttService;
class TimeService;
class WatchdogService;
class WifiService;

class WebServerService {
 public:
  WebServerService(FileSystemService& fileSystem, ConfigService& config, WifiService& wifi,
                   MqttService& mqtt, TimeService& time, WatchdogService& watchdog,
                   DisplayService& display);
  void begin();
  void loop();

 private:
  FileSystemService& fileSystem_;
  ConfigService& config_;
  WifiService& wifi_;
  MqttService& mqtt_;
  TimeService& time_;
  WatchdogService& watchdog_;
  DisplayService& display_;
  WebServer server_{80};

  void registerRoutes();
};
