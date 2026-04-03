#ifndef APP_CONFIG_H
#define APP_CONFIG_H

// WiFi Configuration
#define DEFAULT_AP_SSID "TableClock_AP"
#define DEFAULT_AP_PASSWORD "12345678"
#define DEFAULT_AP_IP "192.168.4.1"


// Hardware Configuration
#define LED_DATA_PIN 15  // D8 on NodeMCU
#define I2C_SDA_PIN 4    // D2 on NodeMCU
#define I2C_SCL_PIN 5    // D1 on NodeMCU
#define BUILTIN_LED_PIN 2  // D4 (GPIO2) on NodeMCU - built-in LED

// LoRa Module (ASR6601) - SoftwareSerial pins
#define LORA_RX_PIN 12   // D5 on NodeMCU - connected to ASR6601 TX
#define LORA_TX_PIN 14   // D6 on NodeMCU - connected to ASR6601 RX

// TCA9548A I2C Multiplexer Address
#define TCA9548A_ADDRESS 0x70

// Display Configuration
#define SSD1306_WIDTH 128
#define SSD1306_HEIGHT 64

// Time Configuration
#define NTP_SERVER "pool.ntp.org"

#endif // APP_CONFIG_H
