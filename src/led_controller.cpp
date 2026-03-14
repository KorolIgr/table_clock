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

// =============================================================================
// Pattern Functions
// =============================================================================

/**
 * @brief Running Fire (Ping-Pong) pattern: LED pattern moves back and forth
 * @param red Red component intensity (0-255)
 * @param green Green component intensity (0-255)
 * @param blue Blue component intensity (0-255)
 * @param delayMs Delay between steps in milliseconds
 */
void LEDController::patternRunningFire(uint8_t red, uint8_t green, uint8_t blue, uint32_t delayMs) {
    static uint8_t position = 0;
    static bool directionForward = true; // true for moving forward, false for backward
    
    // Clear all LEDs first
    clear();
    
    // Set the current LED to the fire color
    setPixel(position, red, green, blue);
    
    // Show the pattern
    show();
    
    // Move to next position based on direction
    if (directionForward) {
        position++;
        if (position >= _count - 1) {
            directionForward = false; // Change direction at the end
        }
    } else {
        position--;
        if (position == 0) {
            directionForward = true; // Change direction at the start
        }
    }
    
    delay(delayMs);
}

/**
 * @brief Wave pattern: LED brightness shifts like a sine wave
 * @param red Red component intensity (0-255)
 * @param green Green component intensity (0-255)
 * @param blue Blue component intensity (0-255)
 * @param delayMs Delay between steps in milliseconds
 */
void LEDController::patternWave(uint8_t red, uint8_t green, uint8_t blue, uint32_t delayMs) {
    static uint8_t wavePosition = 0;
    
    // Calculate brightness for each LED based on distance from wave position
    for (uint8_t i = 0; i < _count; i++) {
        // Calculate distance from current wave position (with wrap-around)
        int8_t distance;
        if (wavePosition >= i) {
            distance = wavePosition - i;
        } else {
            // Handle wrap-around by checking both directions
            int8_t distForward = (_count - i) + wavePosition;
            int8_t distBackward = i - wavePosition;
            distance = (distForward < distBackward) ? distForward : distBackward;
        }
        
        // Calculate brightness based on sine wave (maximum brightness at wave position)
        float angle = (float)(distance % 8) * (2 * PI / 8); // 8 positions per wave cycle
        float brightnessFactor = sin(angle);
        if (brightnessFactor < 0) brightnessFactor = -brightnessFactor; // Make it positive (half wave)
        
        // Scale the color by the brightness factor
        uint8_t scaledRed = (uint8_t)(red * brightnessFactor);
        uint8_t scaledGreen = (uint8_t)(green * brightnessFactor);
        uint8_t scaledBlue = (uint8_t)(blue * brightnessFactor);
        
        setPixel(i, scaledRed, scaledGreen, scaledBlue);
    }
    
    show();
    
    // Move wave position
    wavePosition++;
    if (wavePosition >= _count) {
        wavePosition = 0;
    }
    
    delay(delayMs);
}
