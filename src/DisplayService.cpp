#include "DisplayService.h"

#if defined(BOARD_WAVESHARE_S3_1_47INCH) && __has_include(<Arduino_GFX_Library.h>)
#include <Arduino_GFX_Library.h>
#define APP_HAS_REAL_DISPLAY_BACKEND 1
#else
#define APP_HAS_REAL_DISPLAY_BACKEND 0
#endif

#include "AppConfig.h"
#include "BoardProfile.h"
#include "Logger.h"

#if APP_HAS_REAL_DISPLAY_BACKEND
namespace {

Arduino_DataBus* g_displayBus = new Arduino_ESP32SPI(BoardProfile::kProfile.lcdDcPin,
                                                     BoardProfile::kProfile.lcdCsPin,
                                                     BoardProfile::kProfile.lcdSclkPin,
                                                     BoardProfile::kProfile.lcdMosiPin,
                                                     BoardProfile::kProfile.lcdMisoPin);
Arduino_GFX* g_gfx = new Arduino_ST7789(g_displayBus, BoardProfile::kProfile.lcdRstPin,
                                        BoardProfile::kProfile.rotation, false,
                                        BoardProfile::kProfile.displayWidth,
                                        BoardProfile::kProfile.displayHeight,
                                        BoardProfile::kProfile.colOffset1,
                                        BoardProfile::kProfile.rowOffset1,
                                        BoardProfile::kProfile.colOffset2,
                                        BoardProfile::kProfile.rowOffset2);

void drawLines(const String& headline, const String& detail) {
  g_gfx->fillScreen(BLACK);
  g_gfx->setTextColor(WHITE);
  g_gfx->setTextSize(2);
  g_gfx->setCursor(12, 32);
  g_gfx->println(headline);
  g_gfx->setCursor(12, 72);
  g_gfx->println(detail);
}

}  // namespace
#endif

void DisplayService::begin() {
  enabled_ = AppConfig::kEnableDisplay && BoardProfile::kHasDisplay;
  if (!enabled_) {
    Logger::info("Display deaktiviert");
    return;
  }

  backendReady_ = false;
  backendName_ = String(BoardProfile::kProfile.displayDriverHint) + "/" + BoardProfile::kProfile.touchControllerHint;
  Logger::info(String("Display-Profil: ") + summary());

#if APP_HAS_REAL_DISPLAY_BACKEND
  if (BoardProfile::kProfile.displayBackendSupported) {
    pinMode(BoardProfile::kProfile.lcdBlPin, OUTPUT);
    digitalWrite(BoardProfile::kProfile.lcdBlPin, HIGH);
    g_gfx->begin();
    backendReady_ = true;
    backendName_ = "Arduino_GFX/ST7789";
  }
#endif

  if (!backendReady_) {
    Logger::warn("Kein konkreter TFT-Treiber aktiviert; DisplayService arbeitet als Profil-Stellehalter");
  }
  showStatus("Boot", AppConfig::kDeviceName);
  Logger::info("Display-Profil bereit");
}

void DisplayService::showStatus(const String& headline, const String& detail) {
  if (!enabled_) {
    return;
  }

  lastHeadline_ = headline;
  lastDetail_ = detail;

  if (backendReady_) {
#if APP_HAS_REAL_DISPLAY_BACKEND
    drawLines(headline, detail);
#endif
  } else {
    Logger::info(String("Display-Status: ") + headline + " | " + detail);
  }
}

String DisplayService::summary() const {
  String text = BoardProfile::kProfile.boardName;
  text += " (";
  text += BoardProfile::kProfile.displayWidth;
  text += "x";
  text += BoardProfile::kProfile.displayHeight;
  text += ", ";
  text += BoardProfile::kProfile.displayBus;
  text += ")";
  return text;
}

bool DisplayService::isReady() const { return backendReady_; }

String DisplayService::backendName() const { return backendName_; }
