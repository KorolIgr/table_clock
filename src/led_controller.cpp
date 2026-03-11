#include "led_controller.h"

// =============================================================================
// LEDController Implementation
// APA106 LEDs controlled via Adafruit NeoPixel library
// APA106 uses GRB color order (same as WS2812/NeoPixel)
// =============================================================================

LEDController::LEDController(uint8_t pin, uint8_t count)
    : _strip(count, pin, NEO_GRB + NEO_KHZ800),
      _pin(pin),
      _count(count),
      _brightness(LED_DEFAULT_BRIGHTNESS),
      _initialized(false)
{
}

bool LEDController::begin() {
    Serial.println(F("[LED] Initializing APA106 LED controller..."));
    Serial.printf("[LED] Pin: %d, Count: %d\n", _pin, _count);

    _strip.begin();
    _strip.setBrightness(_brightness);
    _strip.clear();
    _strip.show();

    _initialized = true;
    Serial.println(F("[LED] APA106 LED controller initialized OK"));
    return true;
}

void LEDController::setPixel(uint8_t index, RGBColor color) {
    if (!_initialized || index >= _count) return;
    _strip.setPixelColor(index, _strip.Color(color.r, color.g, color.b));
}

void LEDController::setPixel(uint8_t index, uint8_t r, uint8_t g, uint8_t b) {
    if (!_initialized || index >= _count) return;
    _strip.setPixelColor(index, _strip.Color(r, g, b));
}

void LEDController::setAll(RGBColor color) {
    for (uint8_t i = 0; i < _count; i++) {
        _strip.setPixelColor(i, _strip.Color(color.r, color.g, color.b));
    }
}

void LEDController::setAll(uint8_t r, uint8_t g, uint8_t b) {
    for (uint8_t i = 0; i < _count; i++) {
        _strip.setPixelColor(i, _strip.Color(r, g, b));
    }
}

void LEDController::clear() {
    _strip.clear();
}

void LEDController::show() {
    _strip.show();
}

void LEDController::setBrightness(uint8_t brightness) {
    _brightness = brightness;
    _strip.setBrightness(brightness);
}

uint8_t LEDController::getBrightness() const {
    return _brightness;
}

// =============================================================================
// Test Functions
// =============================================================================

/**
 * @brief Sequential test: lights each LED one by one in white color
 *        Useful to verify all 8 LEDs are connected and working
 */
void LEDController::testSequential(uint32_t delayMs) {
    Serial.println(F("[LED] Test: Sequential (each LED one by one)"));

    // Turn off all LEDs first
    clear();
    show();
    delay(delayMs);

    // Light each LED individually
    for (uint8_t i = 0; i < _count; i++) {
        clear();
        setPixel(i, Colors::WHITE);
        show();
        Serial.printf("[LED] LED %d ON\n", i + 1);
        delay(delayMs);
    }

    // Turn off all
    clear();
    show();
    Serial.println(F("[LED] Sequential test complete"));
}

/**
 * @brief Rainbow test: cycles through rainbow colors across all LEDs
 *        Useful to verify RGB color channels work correctly
 */
void LEDController::testRainbow(uint32_t delayMs) {
    Serial.println(F("[LED] Test: Rainbow effect"));

    for (uint16_t j = 0; j < 256; j++) {
        for (uint8_t i = 0; i < _count; i++) {
            uint32_t color = colorWheel((i * 256 / _count + j) & 255);
            _strip.setPixelColor(i, color);
        }
        show();
        delay(delayMs / 10);  // faster cycle for rainbow
    }

    clear();
    show();
    Serial.println(F("[LED] Rainbow test complete"));
}

/**
 * @brief All colors test: cycles through R, G, B, W, Y, C, M, O for all LEDs
 *        Useful to verify color mixing works correctly
 */
void LEDController::testAllColors(uint32_t delayMs) {
    Serial.println(F("[LED] Test: All colors"));

    const RGBColor testColors[] = {
        Colors::RED,
        Colors::GREEN,
        Colors::BLUE,
        Colors::WHITE,
        Colors::YELLOW,
        Colors::CYAN,
        Colors::MAGENTA,
        Colors::ORANGE
    };
    const char* colorNames[] = {
        "RED", "GREEN", "BLUE", "WHITE", "YELLOW", "CYAN", "MAGENTA", "ORANGE"
    };

    uint8_t numColors = sizeof(testColors) / sizeof(testColors[0]);

    for (uint8_t c = 0; c < numColors; c++) {
        setAll(testColors[c]);
        show();
        Serial.printf("[LED] All LEDs: %s\n", colorNames[c]);
        delay(delayMs);
    }

    clear();
    show();
    Serial.println(F("[LED] All colors test complete"));
}

/**
 * @brief Blink test: blinks all LEDs in white color N times
 *        Quick visual confirmation that LEDs are working
 */
void LEDController::testBlink(uint8_t times, uint32_t delayMs) {
    Serial.printf("[LED] Test: Blink x%d\n", times);

    for (uint8_t i = 0; i < times; i++) {
        setAll(Colors::WHITE);
        show();
        delay(delayMs);
        clear();
        show();
        delay(delayMs);
    }

    Serial.println(F("[LED] Blink test complete"));
}

// =============================================================================
// Private Helpers
// =============================================================================

/**
 * @brief Color wheel helper for rainbow effect
 * @param pos Position on color wheel (0-255)
 * @return 32-bit packed color value
 */
uint32_t LEDController::colorWheel(uint8_t pos) {
    pos = 255 - pos;
    if (pos < 85) {
        return _strip.Color(255 - pos * 3, 0, pos * 3);
    }
    if (pos < 170) {
        pos -= 85;
        return _strip.Color(0, pos * 3, 255 - pos * 3);
    }
    pos -= 170;
    return _strip.Color(pos * 3, 255 - pos * 3, 0);
}
