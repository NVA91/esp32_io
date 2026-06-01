#include "TimeService.h"

#include <time.h>

#include "ConfigService.h"
#include "Logger.h"
#include "WifiService.h"

namespace {

constexpr unsigned long kRetryIntervalMs = 30000UL;
constexpr time_t kValidEpoch = 1700000000;

}  // namespace

TimeService::TimeService(WifiService& wifi, const ConfigService& config)
  : wifi_(wifi), config_(config) {}

void TimeService::begin() {
  started_ = false;
  synchronized_ = false;
  lastSyncAttemptMs_ = 0;
}

void TimeService::loop() {
  if (!wifi_.isConnected()) {
    synchronized_ = false;
    return;
  }

  if (!started_) {
    startNtp();
  }

  const bool is_synced = updateSyncState();
  if (is_synced || millis() - lastSyncAttemptMs_ < kRetryIntervalMs) {
    return;
  }

  Logger::warn("Zeitserver noch nicht synchronisiert, neuer Versuch");
  startNtp();
}

bool TimeService::isSynchronized() const { return synchronized_; }

String TimeService::iso8601() const {
  if (!synchronized_) {
    return String();
  }

  struct tm time_info;
  if (!getLocalTime(&time_info, 10)) {
    return String();
  }

  char buffer[32];
  strftime(buffer, sizeof(buffer), "%Y-%m-%dT%H:%M:%S%z", &time_info);
  return String(buffer);
}

String TimeService::statusText() const {
  if (!wifi_.isConnected()) {
    return String("offline");
  }
  if (!started_) {
    return String("wartet auf Start");
  }
  if (!synchronized_) {
    return String("synchronisiert nicht");
  }
  return iso8601();
}

void TimeService::startNtp() {
  started_ = true;
  lastSyncAttemptMs_ = millis();
  const ConfigService::Settings& settings = config_.settings();
  configTzTime(settings.timeZone.c_str(), settings.ntpServerPrimary.c_str(),
               settings.ntpServerSecondary.c_str(), settings.ntpServerTertiary.c_str());
  Logger::info(String("Zeitserver gestartet: ") + settings.ntpServerPrimary);
  updateSyncState();
}


bool TimeService::updateSyncState() {
  time_t now = time(nullptr);
  const bool was_synchronized = synchronized_;
  synchronized_ = now > kValidEpoch;
  if (synchronized_ && !was_synchronized) {
    Logger::info(String("Zeit synchronisiert: ") + iso8601());
  }
  return synchronized_;
}