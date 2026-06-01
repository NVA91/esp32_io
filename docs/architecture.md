# Architektur

Die Vorlage trennt die Kernfunktionen in kleine Services:

- ConfigService laedt Default-Werte aus dem Code und optionale Overrides aus LittleFS.
- WifiService verbindet den ESP32 mit dem WLAN, startet Reconnects und faellt bei Bedarf auf einen Setup-AP mit DNS-Captive-Portal zurueck.
- OtaService kapselt ArduinoOTA.
- TimeService kapselt NTP-Synchronisierung und lokale Zeit.
- WebServerService stellt die HTTP-Endpunkte und LittleFS-Inhalte bereit.
- MqttService verwaltet Broker-Verbindung, Subscription und Statusmeldungen.
- FileSystemService mountet LittleFS und liest Dateien.
- WatchdogService fuettert den Task-Watchdog im Hauptloop.
- BoardProfile kapselt die Auswahl zwischen den Waveshare-S3-Zielboards.
- DisplayService kapselt die profilabhaengige Display-Schicht und kann fuer das 1.47-Zoll-SPI-Board bereits ein Arduino_GFX-Backend initialisieren.
- Logger schreibt strukturierte Meldungen auf die serielle Konsole.
