# esp32-template

ESP32-Template auf Basis von PlatformIO und Arduino-Framework.

Enthaltene Bausteine:

- WLAN
- OTA via ArduinoOTA
- HTTP-Webserver
- MQTT via PubSubClient
- LittleFS
- Serielles Logging
- Display-Unterstuetzung via U8g2

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
2. Firmware bauen und flashen:

	```bash
	pio run
	pio run --target upload
	```

3. LittleFS-Inhalte hochladen:

	```bash
	pio run --target uploadfs
	```

4. Seriellen Monitor oeffnen:

	```bash
	pio device monitor
	```

## Endpunkte

- `/` liefert die LittleFS-Startseite.
- `/api/status` liefert JSON mit WLAN- und MQTT-Status.

## Hinweise

- Die Display-Komponente ist fuer ein SSD1306-Display vorbereitet.
- MQTT und OTA verwenden Beispielwerte und muessen fuer produktive Nutzung angepasst werden.
- Weitere Details stehen in `docs/architecture.md`.
