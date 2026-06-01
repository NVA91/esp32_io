# Board-Profile

Die Vorlage kennt drei Waveshare-Profile:

- waveshare_s3_2inch: 240x320, IPS, SPI, kapazitiver Touch, ST7789-Hinweis
- waveshare_s3_1_47inch: 172x320, IPS, SPI, kapazitiver Touch, ST7789-Hinweis
- waveshare_s3_7inch: 800x480, IPS, RGB, kapazitiver Touch

## Auswahl in PlatformIO

```bash
pio run -e waveshare_s3_2inch
pio run -e waveshare_s3_1_47inch
pio run -e waveshare_s3_7inch
```

## Was heute schon abgedeckt ist

- Board-Auswahl ueber Build-Flags
- passende Display-Metadaten im Firmware-Status
- identische Kern-Services fuer WLAN, OTA, MQTT, LittleFS und HTTP

## Was noch boardspezifisch ergänzt werden sollte

- exakte Panel-Pinbelegung aus dem jeweiligen Waveshare-Wiki
- konkreter TFT/RGB-Backend-Treiber in DisplayService
- Touch-Controller-Initialisierung und Event-Verarbeitung

Der wichtige Punkt ist: Die Vorlage ist jetzt nicht mehr auf ein SSD1306-I2C-Display festgelegt, sondern auf die genannten Waveshare-S3-Displayklassen vorbereitet.
