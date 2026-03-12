#pragma once

// =============================================================================
// Table Clock Project - Configuration
// ESP8266 + TCA9548A + 8x SSD1306 + 8x APA106
// =============================================================================

// --- Hardware Pin Configuration ---

// I2C pins (for TCA9548A and SSD1306)
#ifndef SDA_PIN
  #define SDA_PIN 4   // D2 on Wemos D1 Mini / NodeMCU
#endif

#ifndef SCL_PIN
  #define SCL_PIN 5   // D1 on Wemos D1 Mini / NodeMCU
#endif

// APA106 LED strip data pin
#ifndef LED_PIN
  #define LED_PIN 15  // D8 on Wemos D1 Mini / NodeMCU
#endif

// Number of APA106 LEDs
#ifndef LED_COUNT
  #define LED_COUNT 8
#endif

// --- I2C Device Addresses ---

// TCA9548A I2C multiplexer address (A0=A1=A2=GND => 0x70)
#ifndef TCA9548A_ADDR
  #define TCA9548A_ADDR 0x70
#endif

// SSD1306 display I2C address
#ifndef SSD1306_ADDR
  #define SSD1306_ADDR 0x3C
#endif

// Number of SSD1306 displays (one per TCA9548A channel)
#define DISPLAY_COUNT 8

// SSD1306 display resolution
#define DISPLAY_WIDTH  128
#define DISPLAY_HEIGHT 64

// --- Serial Debug ---
#define SERIAL_BAUD_RATE 115200

// --- LED Default Settings ---
#define LED_DEFAULT_BRIGHTNESS 50   // 0-255
#define LED_TEST_DELAY_MS      200  // delay between LED test steps (ms)

// --- Firmware Version ---
#define FW_VERSION_MAJOR 0
#define FW_VERSION_MINOR 2
#define FW_VERSION_PATCH 0
#define FW_VERSION_STR   "0.2.0"

// =============================================================================
// WiFi Configuration (Stage 4)
// =============================================================================

// --- Access Point (AP) Settings ---
// SSID точки доступа для первоначальной настройки
#define WIFI_AP_SSID        "TableClock-Setup"
// Пароль AP (минимум 8 символов, или "" для открытой сети)
#define WIFI_AP_PASSWORD    "12345678"
// IP адрес точки доступа
#define WIFI_AP_IP          IPAddress(192, 168, 4, 1)
// Маска подсети AP
#define WIFI_AP_SUBNET      IPAddress(255, 255, 255, 0)
// Канал WiFi для AP
#define WIFI_AP_CHANNEL     6

// --- STA (Station) Connection Settings ---
// Таймаут подключения к WiFi (мс)
#define WIFI_CONNECT_TIMEOUT_MS   15000
// Интервал повторных попыток подключения (мс)
#define WIFI_RECONNECT_INTERVAL_MS 30000
// Максимальное количество попыток подключения
#define WIFI_MAX_RETRIES          5

// --- EEPROM / Flash Storage ---
// Адрес начала хранения WiFi настроек в EEPROM
#define WIFI_EEPROM_START_ADDR    0
// Размер EEPROM для хранения настроек
#define WIFI_EEPROM_SIZE          512
// Магическое число для проверки валидности данных
#define WIFI_EEPROM_MAGIC         0xA5B6C7D8UL

// --- Web Server ---
// Порт веб-сервера
#define WIFI_WEB_SERVER_PORT      80

// --- DNS ---
// Порт DNS сервера (для captive portal в режиме AP)
#define WIFI_DNS_PORT             53
