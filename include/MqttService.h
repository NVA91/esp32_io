#pragma once

#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFiClient.h>

class MqttService {
 public:
  void begin();
  void loop();
  void publishStatus(const String& payload);
  bool isConnected() const;

 private:
  static void mqttCallback(char* topic, byte* payload, unsigned int length);

  WiFiClient wifiClient_;
  PubSubClient client_{wifiClient_};
  unsigned long lastReconnectMs_ = 0;
  static MqttService* instance_;

  void ensureConnected();
  void handleMessage(char* topic, byte* payload, unsigned int length);
};
