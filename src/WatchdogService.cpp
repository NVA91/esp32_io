#include "WatchdogService.h"

#include "AppConfig.h"
#include "Logger.h"

#if __has_include(<esp_task_wdt.h>)
#include <esp_task_wdt.h>
#define APP_HAS_ESP_TASK_WDT 1
#else
#define APP_HAS_ESP_TASK_WDT 0
#endif

void WatchdogService::begin() {
#if APP_HAS_ESP_TASK_WDT
#if defined(ESP_IDF_VERSION_MAJOR) && ESP_IDF_VERSION_MAJOR >= 5
  esp_task_wdt_config_t config = {};
  config.timeout_ms = AppConfig::kWatchdogTimeoutSeconds * 1000UL;
#if defined(CONFIG_FREERTOS_NUMBER_OF_CORES)
  config.idle_core_mask = (1U << CONFIG_FREERTOS_NUMBER_OF_CORES) - 1U;
#else
  config.idle_core_mask = 1U;
#endif
  config.trigger_panic = true;
  esp_err_t result = esp_task_wdt_init(&config);
#else
  esp_err_t result = esp_task_wdt_init(AppConfig::kWatchdogTimeoutSeconds, true);
#endif

  if (result == ESP_OK || result == ESP_ERR_INVALID_STATE) {
    esp_task_wdt_add(nullptr);
    enabled_ = true;
    Logger::info(String("Watchdog aktiv: ") + AppConfig::kWatchdogTimeoutSeconds + "s");
    return;
  }

  Logger::warn(String("Watchdog konnte nicht aktiviert werden, Fehlercode ") + static_cast<int>(result));
#else
  Logger::warn("Kein ESP-Task-Watchdog verfuegbar");
#endif
}

void WatchdogService::feed() {
#if APP_HAS_ESP_TASK_WDT
  if (enabled_) {
    esp_task_wdt_reset();
  }
#endif
}

bool WatchdogService::isEnabled() const { return enabled_; }

uint32_t WatchdogService::timeoutSeconds() const { return AppConfig::kWatchdogTimeoutSeconds; }
