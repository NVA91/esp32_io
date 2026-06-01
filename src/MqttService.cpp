#include "MqttService.h"

#include "AppConfig.h"
#include "Logger.h"

MqttService* MqttService::instance_ = nullptr;

void MqttService::begin() {
  instance_ = this;
  client_.setServer(AppConfig::kMqttHost, AppConfig::kMqttPort);
  client_.setCallback(MqttService::mqttCallback);
}

void MqttService::loop() {
  ensureConnected();
  client_.loop();
}

void MqttService::publishStatus(const String& payload) {
  if (!client_.connected()) {
    return;
  }
  client_.publish(AppConfig::kMqttTopicState, payload.c_str(), true);
}

bool MqttService::isConnected() const { return client_.connected(); }

void MqttService::mqttCallback(char* topic, byte* payload, unsigned int length) {
  if (instance_ != nullptr) {
    instance_->handleMessage(topic, payload, length);
  }
}

void MqttService::ensureConnected() {
  if (client_.connected()) {
    return;
  }

  const unsigned long now = millis();
  if (now - lastReconnectMs_ < 5000UL) {
    return;
  }

  lastReconnectMs_ = now;
  Logger::warn("MQTT Reconnect");
  if (client_.connect(AppConfig::kMqttClientId)) {
    client_.subscribe(AppConfig::kMqttTopicCommand);
    publishStatus("online");
    Logger::info("MQTT verbunden");
  }
}

void MqttService::handleMessage(char* topic, byte* payload, unsigned int length) {
  String message;
  message.reserve(length);
  for (unsigned int index = 0; index < length; ++index) {
    message += static_cast<char>(payload[index]);
  }

  Logger::info(String("MQTT ") + topic + ": " + message);
}
