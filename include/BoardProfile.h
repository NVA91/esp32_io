#pragma once

#include <Arduino.h>

namespace BoardProfile {

struct Profile {
  const char* id;
  const char* boardName;
  uint16_t displayWidth;
  uint16_t displayHeight;
  const char* displayBus;
  const char* displayDriverHint;
  bool touchSupported;
  const char* touchType;
    const char* touchControllerHint;
    bool displayBackendSupported;
    int8_t lcdDcPin;
    int8_t lcdCsPin;
    int8_t lcdSclkPin;
    int8_t lcdMosiPin;
    int8_t lcdMisoPin;
    int8_t lcdRstPin;
    int8_t lcdBlPin;
    uint8_t rotation;
    uint16_t colOffset1;
    uint16_t rowOffset1;
    uint16_t colOffset2;
    uint16_t rowOffset2;
  const char* notes;
};

#if defined(BOARD_WAVESHARE_S3_2INCH)
inline constexpr Profile kProfile = {
    "waveshare_s3_2inch",
    "Waveshare ESP32-S3 Touch LCD 2",
    240,
    320,
    "SPI",
    "ST7789",
    true,
    "kapazitiv",
    "CST816D",
    false,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    0,
    0,
    0,
    0,
    0,
    "2.0 Zoll IPS, USB-A, S3 Dual-Core LX7"};
#elif defined(BOARD_WAVESHARE_S3_1_47INCH)
inline constexpr Profile kProfile = {
    "waveshare_s3_1_47inch",
    "Waveshare ESP32-S3 Touch LCD 1.47",
    172,
    320,
    "SPI",
    "ST7789",
    true,
    "kapazitiv",
    "AXS5106L",
    true,
    45,
    21,
    38,
    39,
    -1,
    47,
    46,
    0,
    34,
    0,
    34,
    0,
    "1.47 Zoll IPS, kompakter S3-Touch-Traeger"};
#elif defined(BOARD_WAVESHARE_S3_7INCH)
inline constexpr Profile kProfile = {
    "waveshare_s3_7inch",
    "Waveshare ESP32-S3 Touch LCD 7",
    800,
    480,
    "RGB",
    "RGB-PANEL",
    true,
    "kapazitiv",
    "unbekannt",
    false,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    0,
    0,
    0,
    0,
    0,
    "7.0 Zoll IPS, grosses RGB-Panel mit Touch"};
#else
inline constexpr Profile kProfile = {
    "generic_esp32",
    "Generisches ESP32-Board",
    0,
    0,
    "unbekannt",
    "keiner",
    false,
    "keiner",
    "keiner",
    false,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    0,
    0,
    0,
    0,
    0,
    "Kein Waveshare-S3-Profil aktiv"};
#endif

inline constexpr bool kHasDisplay = kProfile.displayWidth > 0 && kProfile.displayHeight > 0;

}  // namespace BoardProfile
