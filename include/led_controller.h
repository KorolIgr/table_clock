#pragma once

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "config.h"

// =============================================================================
// LED Controller for APA106 LEDs
// APA106 is compatible with NeoPixel (WS2812) protocol
// =============================================================================

// Color structure for convenience
struct RGBColor {
    uint8_t r;
    uint8_t g;
    uint8_t b;

    RGBColor(uint8_t r = 0, uint8_t g = 0, uint8_t b = 0) : r(r), g(g), b(b) {}
};

// Predefined colors
namespace Colors {
    const RGBColor RED     = {255, 0,   0};
    const RGBColor GREEN   = {0,   255, 0};
    const RGBColor BLUE    = {0,   0,   255};
    const RGBColor WHITE   = {255, 255, 255};
    const RGBColor YELLOW  = {255, 255, 0};
    const RGBColor CYAN    = {0,   255, 255};
    const RGBColor MAGENTA = {255, 0,   255};
    const RGBColor ORANGE  = {255, 128, 0};
    const RGBColor OFF     = {0,   0,   0};
}

class LEDController {
public:
    // Constructor
    LEDController(uint8_t pin = LED_PIN, uint8_t count = LED_COUNT);

    // Initialization
    bool begin();

    // Basic control
    void setPixel(uint8_t index, RGBColor color);
    void setPixel(uint8_t index, uint8_t r, uint8_t g, uint8_t b);
    void setAll(RGBColor color);
    void setAll(uint8_t r, uint8_t g, uint8_t b);
    void clear();
    void show();

    // Brightness control
    void setBrightness(uint8_t brightness);
    uint8_t getBrightness() const;

    // Test functions
    void testSequential(uint32_t delayMs = LED_TEST_DELAY_MS);
    void testRainbow(uint32_t delayMs = LED_TEST_DELAY_MS);
    void testAllColors(uint32_t delayMs = LED_TEST_DELAY_MS);
    void testBlink(uint8_t times = 3, uint32_t delayMs = 300);

    // Pattern functions
    void patternRunningFire(uint8_t red, uint8_t green, uint8_t blue, uint32_t delayMs = 100);
    void patternWave(uint8_t red, uint8_t green, uint8_t blue, uint32_t delayMs = 100);

    // Utility
    uint8_t getCount() const { return _count; }
    bool isInitialized() const { return _initialized; }

private:
    Adafruit_NeoPixel _strip;
    uint8_t _pin;
    uint8_t _count;
    uint8_t _brightness;
    bool _initialized;

    // Helper: wheel function for rainbow effect
    uint32_t colorWheel(uint8_t pos);
};
