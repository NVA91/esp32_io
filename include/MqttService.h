#pragma once

#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFiClient.h>

class ConfigService;

class MqttService {
 public:
  explicit MqttService(const ConfigService& config);

  void begin();
  void loop();
  void publishStatus(const String& payload);
  bool isConnected() const;
  bool isConfigured() const;

 private:
  const ConfigService& config_;
  static void mqttCallback(char* topic, byte* payload, unsigned int length);

  WiFiClient wifiClient_;
  PubSubClient client_{wifiClient_};
  unsigned long lastReconnectMs_ = 0;
  static MqttService* instance_;

  void ensureConnected();
  void handleMessage(char* topic, byte* payload, unsigned int length);
};
