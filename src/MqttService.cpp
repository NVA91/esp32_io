#include "MqttService.h"

#include "AppConfig.h"
#include "ConfigService.h"
#include "Logger.h"

MqttService* MqttService::instance_ = nullptr;

MqttService::MqttService(const ConfigService& config) : config_(config) {}

void MqttService::begin() {
  instance_ = this;
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
  client_.publish(config_.settings().mqttTopicState.c_str(), payload.c_str(), true);
}

bool MqttService::isConnected() const { return client_.connected(); }

bool MqttService::isConfigured() const { return !config_.settings().mqttHost.isEmpty(); }

void MqttService::mqttCallback(char* topic, byte* payload, unsigned int length) {
  if (instance_ != nullptr) {
    instance_->handleMessage(topic, payload, length);
  }
}

void MqttService::ensureConnected() {
  if (client_.connected()) {
    return;
  }

  const ConfigService::Settings& settings = config_.settings();
  if (settings.mqttHost.isEmpty()) {
    return;
  }

  client_.setServer(settings.mqttHost.c_str(), settings.mqttPort);

  const unsigned long now = millis();
  if (now - lastReconnectMs_ < 5000UL) {
    return;
  }

  lastReconnectMs_ = now;
  Logger::warn("MQTT Reconnect");
  const bool connected = settings.mqttUsername.isEmpty()
                             ? client_.connect(settings.mqttClientId.c_str())
                             : client_.connect(settings.mqttClientId.c_str(), settings.mqttUsername.c_str(),
                                               config_.secrets().mqttPassword.c_str());
  if (connected) {
    client_.subscribe(settings.mqttTopicCommand.c_str());
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
