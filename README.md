# esp32-template

ESP32-Template auf Basis von PlatformIO und Arduino-Framework, jetzt auf Waveshare ESP32-S3 Display-Boards ausgerichtet.

Abgedeckte Zielboards:

- Waveshare ESP32-S3 Touch LCD 2, 240x320, IPS, kapazitiver Touch
- Waveshare ESP32-S3 Touch LCD 1.47, 172x320, IPS, kapazitiver Touch
- Waveshare ESP32-S3 Touch LCD 7, 800x480, IPS, kapazitiver Touch

Enthaltene Bausteine:

- WLAN
- OTA via ArduinoOTA
- HTTP-Webserver
- MQTT via PubSubClient
- Zeitserver via NTP
- LittleFS
- Serielles Logging
- Board-Profil-Schicht fuer Display- und Touch-Metadaten
- Lokaler Web-Konfigurator mit persistenter Konfiguration in LittleFS
- Watchdog-Grundschutz fuer den Main-Loop
- Setup-AP mit lokalem Konfigurationsportal bei WLAN-Ausfall
- Getrennte Ablage von Konfiguration und Secrets in LittleFS

## Projektstruktur

```text
esp32-template
├── src
├── include
├── data
├── docs
├── platformio.ini
└── README.md
```

## Schnellstart

1. Zugangsdaten und Endpunkte in `include/AppConfig.h` anpassen.
2. Passendes Zielboard in `platformio.ini` oder per `-e` auswaehlen.
3. Firmware bauen und flashen:

   ```bash
   pio run -e waveshare_s3_2inch
   pio run -e waveshare_s3_2inch --target upload
   ```

4. LittleFS-Inhalte hochladen:

   ```bash
   pio run -e waveshare_s3_2inch --target uploadfs
   ```

5. Seriellen Monitor oeffnen:

   ```bash
   pio device monitor
   ```

6. Web-Konfigurator im Browser aufrufen und Laufzeitwerte anpassen.

## PlatformIO-Targets

- `waveshare_s3_2inch`
- `waveshare_s3_1_47inch`
- `waveshare_s3_7inch`

## Endpunkte

- `/` liefert die LittleFS-Startseite.
- `/api/status` liefert JSON mit WLAN- und MQTT-Status.
- `/api/config` liefert und speichert die lokale Konfiguration.
- `/api/time` liefert den NTP-Synchronisationsstatus und die aktuelle lokale Zeit.
- `/api/restart` startet das Geraet nach gespeicherter Konfiguration neu.

## Hinweise

- Die Vorlage verwendet jetzt Board-Profile statt einer festen SSD1306-Annahme.
- Die exakte panel- und touch-spezifische Initialisierung pro Waveshare-Board ist als naechster Schritt im Display-Backend zu hinterlegen.
- MQTT und OTA verwenden Beispielwerte und muessen fuer produktive Nutzung angepasst werden.
- Zeitserver und Zeitzone sind in [include/AppConfig.h](include/AppConfig.h) konfigurierbar.
- Lokale Overrides liegen in [include/AppConfig.h](include/AppConfig.h) als Defaults und werden bei Bedarf durch `/config.ini` aus LittleFS ersetzt.
- Secrets werden getrennt in `/secrets.ini` abgelegt und nicht per GET-API ausgespielt.
- Wenn kein WLAN verfuegbar ist, startet das Geraet einen Setup-AP mit dem Suffix `-setup` und liefert den Web-Konfigurator lokal aus.
- Fuer das 1.47-Zoll-Waveshare-Profil ist ein erstes Arduino_GFX-ST7789-Backend vorbereitet; die 2-Zoll- und 7-Zoll-Profile bleiben als naechste Hardware-Slices offen.
- Der Web-Konfigurator ist fuer eine solide Projektbasis gedacht: Defaults im Code, projektspezifische Werte im Flash.
- Weitere Details stehen in `docs/architecture.md` und `docs/boards.md`.
