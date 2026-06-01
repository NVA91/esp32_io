#pragma once

#include <WebServer.h>

class FileSystemService;
class MqttService;
class WifiService;

class WebServerService {
 public:
  WebServerService(FileSystemService& fileSystem, WifiService& wifi, MqttService& mqtt);
  void begin();
  void loop();

 private:
  FileSystemService& fileSystem_;
  WifiService& wifi_;
  MqttService& mqtt_;
  WebServer server_{80};

  void registerRoutes();
};
