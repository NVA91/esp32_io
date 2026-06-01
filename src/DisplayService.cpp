#include "DisplayService.h"

#include <U8g2lib.h>

#include "AppConfig.h"
#include "Logger.h"

namespace {

U8G2_SSD1306_128X64_NONAME_F_HW_I2C display(U8G2_R0, U8X8_PIN_NONE);

}  // namespace

void DisplayService::begin() {
  enabled_ = AppConfig::kEnableDisplay;
  if (!enabled_) {
    Logger::info("Display deaktiviert");
    return;
  }

  display.begin();
  display.setFont(u8g2_font_6x12_tf);
  showStatus("Boot", AppConfig::kDeviceName);
  Logger::info("Display bereit");
}

void DisplayService::showStatus(const String& headline, const String& detail) {
  if (!enabled_) {
    return;
  }

  display.clearBuffer();
  display.drawStr(0, 14, headline.c_str());
  display.drawUTF8(0, 30, detail.c_str());
  display.sendBuffer();
}
