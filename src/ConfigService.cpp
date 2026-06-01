#include "ConfigService.h"

#include "AppConfig.h"
#include "FileSystemService.h"
#include "Logger.h"

namespace {

uint16_t parsePort(const String& value, uint16_t fallback) {
  const long parsed = value.toInt();
  if (parsed <= 0 || parsed > 65535) {
    return fallback;
  }
  return static_cast<uint16_t>(parsed);
}

}  // namespace

ConfigService::ConfigService(FileSystemService& fileSystem) : fileSystem_(fileSystem) {}

bool ConfigService::begin() {
  loadDefaults();
  hasLocalOverrides_ = loadPublicOverrides();
  hasSecretOverrides_ = loadSecretOverrides();
  Logger::info(String("Konfiguration bereit: ") + (hasLocalOverrides_ ? "lokale Overrides" : "Defaults"));
  return true;
}

const ConfigService::Settings& ConfigService::settings() const { return settings_; }

const ConfigService::Secrets& ConfigService::secrets() const { return secrets_; }

ConfigService::Settings ConfigService::editableCopy() const { return settings_; }

ConfigService::Secrets ConfigService::editableSecretsCopy() const { return secrets_; }

bool ConfigService::update(const Settings& updatedSettings) {
  settings_ = updatedSettings;
  const bool saved = fileSystem_.writeFile(AppConfig::kConfigFilePath, serialize());
  if (saved) {
    hasLocalOverrides_ = true;
    Logger::info(String("Konfiguration gespeichert: ") + AppConfig::kConfigFilePath);
  } else {
    Logger::error(String("Konfiguration konnte nicht gespeichert werden: ") + AppConfig::kConfigFilePath);
  }
  return saved;
}

bool ConfigService::updateSecrets(const Secrets& updatedSecrets) {
  secrets_ = updatedSecrets;
  const bool saved = fileSystem_.writeFile(AppConfig::kSecretsFilePath, serializeSecrets());
  if (saved) {
    hasSecretOverrides_ = true;
    Logger::info(String("Secrets gespeichert: ") + AppConfig::kSecretsFilePath);
  } else {
    Logger::error(String("Secrets konnten nicht gespeichert werden: ") + AppConfig::kSecretsFilePath);
  }
  return saved;
}

bool ConfigService::hasLocalOverrides() const { return hasLocalOverrides_; }

bool ConfigService::hasSecretOverrides() const { return hasSecretOverrides_; }

String ConfigService::publicJson() const {
  String json = "{";
  json += "\"deviceName\":\"" + escapeJson(settings_.deviceName) + "\",";
  json += "\"wifiSsid\":\"" + escapeJson(settings_.wifiSsid) + "\",";
  json += "\"wifiPasswordSet\":";
  json += secrets_.wifiPassword.isEmpty() ? "false" : "true";
  json += ",\"mqttHost\":\"" + escapeJson(settings_.mqttHost) + "\",";
  json += "\"mqttPort\":" + String(settings_.mqttPort) + ",";
  json += "\"mqttUsername\":\"" + escapeJson(settings_.mqttUsername) + "\",";
  json += "\"mqttClientId\":\"" + escapeJson(settings_.mqttClientId) + "\",";
  json += "\"mqttTopicState\":\"" + escapeJson(settings_.mqttTopicState) + "\",";
  json += "\"mqttTopicCommand\":\"" + escapeJson(settings_.mqttTopicCommand) + "\",";
  json += "\"otaHostname\":\"" + escapeJson(settings_.otaHostname) + "\",";
  json += "\"otaPasswordSet\":";
  json += secrets_.otaPassword.isEmpty() ? "false" : "true";
  json += ",\"mqttPasswordSet\":";
  json += secrets_.mqttPassword.isEmpty() ? "false" : "true";
  json += ",\"timeZone\":\"" + escapeJson(settings_.timeZone) + "\",";
  json += "\"ntpServerPrimary\":\"" + escapeJson(settings_.ntpServerPrimary) + "\",";
  json += "\"ntpServerSecondary\":\"" + escapeJson(settings_.ntpServerSecondary) + "\",";
  json += "\"ntpServerTertiary\":\"" + escapeJson(settings_.ntpServerTertiary) + "\",";
  json += "\"hasLocalOverrides\":";
  json += hasLocalOverrides_ ? "true" : "false";
  json += ",\"hasSecretOverrides\":";
  json += hasSecretOverrides_ ? "true" : "false";
  json += "}";
  return json;
}

void ConfigService::loadDefaults() {
  settings_.deviceName = AppConfig::kDeviceName;
  settings_.wifiSsid = AppConfig::kWifiSsid;
  settings_.mqttHost = AppConfig::kMqttHost;
  settings_.mqttPort = AppConfig::kMqttPort;
  settings_.mqttUsername = "";
  settings_.mqttClientId = AppConfig::kMqttClientId;
  settings_.mqttTopicState = AppConfig::kMqttTopicState;
  settings_.mqttTopicCommand = AppConfig::kMqttTopicCommand;
  settings_.otaHostname = AppConfig::kOtaHostname;
  settings_.timeZone = AppConfig::kTimeZone;
  settings_.ntpServerPrimary = AppConfig::kNtpServerPrimary;
  settings_.ntpServerSecondary = AppConfig::kNtpServerSecondary;
  settings_.ntpServerTertiary = AppConfig::kNtpServerTertiary;

  secrets_.wifiPassword = AppConfig::kWifiPassword;
  secrets_.mqttPassword = "";
  secrets_.otaPassword = AppConfig::kOtaPassword;
}

bool ConfigService::loadPublicOverrides() {
  if (!fileSystem_.hasFile(AppConfig::kConfigFilePath)) {
    return false;
  }

  const String content = fileSystem_.readFile(AppConfig::kConfigFilePath);
  int start = 0;
  while (start < content.length()) {
    int end = content.indexOf('\n', start);
    if (end < 0) {
      end = content.length();
    }

    const String line = trimCopy(content.substring(start, end));
    if (!line.isEmpty() && !line.startsWith("#")) {
      const int separator = line.indexOf('=');
      if (separator > 0) {
        applyPublicEntry(trimCopy(line.substring(0, separator)),
                         trimCopy(line.substring(separator + 1)));
      }
    }

    start = end + 1;
  }

  return true;
}

bool ConfigService::loadSecretOverrides() {
  if (!fileSystem_.hasFile(AppConfig::kSecretsFilePath)) {
    return false;
  }

  const String content = fileSystem_.readFile(AppConfig::kSecretsFilePath);
  int start = 0;
  while (start < content.length()) {
    int end = content.indexOf('\n', start);
    if (end < 0) {
      end = content.length();
    }

    const String line = trimCopy(content.substring(start, end));
    if (!line.isEmpty() && !line.startsWith("#")) {
      const int separator = line.indexOf('=');
      if (separator > 0) {
        applySecretEntry(trimCopy(line.substring(0, separator)),
                         trimCopy(line.substring(separator + 1)));
      }
    }

    start = end + 1;
  }

  return true;
}

void ConfigService::applyPublicEntry(const String& key, const String& value) {
  if (key == "device_name") {
    settings_.deviceName = value;
  } else if (key == "wifi_ssid") {
    settings_.wifiSsid = value;
  } else if (key == "mqtt_host") {
    settings_.mqttHost = value;
  } else if (key == "mqtt_port") {
    settings_.mqttPort = parsePort(value, settings_.mqttPort);
  } else if (key == "mqtt_username") {
    settings_.mqttUsername = value;
  } else if (key == "mqtt_client_id") {
    settings_.mqttClientId = value;
  } else if (key == "mqtt_topic_state") {
    settings_.mqttTopicState = value;
  } else if (key == "mqtt_topic_command") {
    settings_.mqttTopicCommand = value;
  } else if (key == "ota_hostname") {
    settings_.otaHostname = value;
  } else if (key == "time_zone") {
    settings_.timeZone = value;
  } else if (key == "ntp_primary") {
    settings_.ntpServerPrimary = value;
  } else if (key == "ntp_secondary") {
    settings_.ntpServerSecondary = value;
  } else if (key == "ntp_tertiary") {
    settings_.ntpServerTertiary = value;
  }
}

void ConfigService::applySecretEntry(const String& key, const String& value) {
  if (key == "wifi_password") {
    secrets_.wifiPassword = value;
  } else if (key == "mqtt_password") {
    secrets_.mqttPassword = value;
  } else if (key == "ota_password") {
    secrets_.otaPassword = value;
  }
}

String ConfigService::serialize() const {
  String content;
  content.reserve(512);
  content += "device_name=" + settings_.deviceName + "\n";
  content += "wifi_ssid=" + settings_.wifiSsid + "\n";
  content += "mqtt_host=" + settings_.mqttHost + "\n";
  content += "mqtt_port=" + String(settings_.mqttPort) + "\n";
  content += "mqtt_username=" + settings_.mqttUsername + "\n";
  content += "mqtt_client_id=" + settings_.mqttClientId + "\n";
  content += "mqtt_topic_state=" + settings_.mqttTopicState + "\n";
  content += "mqtt_topic_command=" + settings_.mqttTopicCommand + "\n";
  content += "ota_hostname=" + settings_.otaHostname + "\n";
  content += "time_zone=" + settings_.timeZone + "\n";
  content += "ntp_primary=" + settings_.ntpServerPrimary + "\n";
  content += "ntp_secondary=" + settings_.ntpServerSecondary + "\n";
  content += "ntp_tertiary=" + settings_.ntpServerTertiary + "\n";
  return content;
}

String ConfigService::serializeSecrets() const {
  String content;
  content.reserve(192);
  content += "wifi_password=" + secrets_.wifiPassword + "\n";
  content += "mqtt_password=" + secrets_.mqttPassword + "\n";
  content += "ota_password=" + secrets_.otaPassword + "\n";
  return content;
}

String ConfigService::trimCopy(const String& value) {
  String copy = value;
  copy.trim();
  return copy;
}

String ConfigService::escapeJson(const String& value) {
  String escaped;
  escaped.reserve(value.length() + 8);
  for (size_t index = 0; index < value.length(); ++index) {
    const char ch = value[index];
    if (ch == '\\' || ch == '"') {
      escaped += '\\';
    }
    escaped += ch;
  }
  return escaped;
}
