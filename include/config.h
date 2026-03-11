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
#define FW_VERSION_MINOR 1
#define FW_VERSION_PATCH 0
#define FW_VERSION_STR   "0.1.0"
