# Architektur

Die Vorlage trennt die Kernfunktionen in kleine Services:

- WifiService verbindet den ESP32 mit dem WLAN und startet Reconnects.
- OtaService kapselt ArduinoOTA.
- WebServerService stellt die HTTP-Endpunkte und LittleFS-Inhalte bereit.
- MqttService verwaltet Broker-Verbindung, Subscription und Statusmeldungen.
- FileSystemService mountet LittleFS und liest Dateien.
- DisplayService kapselt ein SSD1306-kompatibles U8g2-Display.
- Logger schreibt strukturierte Meldungen auf die serielle Konsole.
