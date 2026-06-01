#pragma once

#include <Arduino.h>

class FileSystemService;

class ConfigService {
 public:
  struct Settings {
    String deviceName;
    String wifiSsid;
    String mqttHost;
    uint16_t mqttPort;
    String mqttUsername;
    String mqttClientId;
    String mqttTopicState;
    String mqttTopicCommand;
    String otaHostname;
    String timeZone;
    String ntpServerPrimary;
    String ntpServerSecondary;
    String ntpServerTertiary;
  };

  struct Secrets {
    String wifiPassword;
    String mqttPassword;
    String otaPassword;
  };

  explicit ConfigService(FileSystemService& fileSystem);

  bool begin();
  const Settings& settings() const;
  const Secrets& secrets() const;
  Settings editableCopy() const;
  Secrets editableSecretsCopy() const;
  bool update(const Settings& updatedSettings);
  bool updateSecrets(const Secrets& updatedSecrets);
  bool hasLocalOverrides() const;
  bool hasSecretOverrides() const;
  String publicJson() const;

 private:
  FileSystemService& fileSystem_;
  Settings settings_;
  Secrets secrets_;
  bool hasLocalOverrides_ = false;
  bool hasSecretOverrides_ = false;

  void loadDefaults();
  bool loadPublicOverrides();
  bool loadSecretOverrides();
  void applyPublicEntry(const String& key, const String& value);
  void applySecretEntry(const String& key, const String& value);
  String serialize() const;
  String serializeSecrets() const;
  static String trimCopy(const String& value);
  static String escapeJson(const String& value);
};
