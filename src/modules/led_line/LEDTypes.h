#ifndef LED_TYPES_H
#define LED_TYPES_H

#include <NeoPixelBus.h>

// Enum for LED patterns
enum class LEDPattern {
    RUNNING_LIGHT,
    PING_PONG,
    RAINBOW_WAVE,
    CHASE,
    BLINK
};

// Struct for pattern configuration
struct PatternConfig {
    LEDPattern pattern = LEDPattern::RUNNING_LIGHT;
    bool direction = true;  // true = forward, false = reverse
    uint16_t speed = 500;   // in milliseconds
    RgbColor color = RgbColor(20, 20, 20);  // white by default
    uint8_t trailLength = 2;  // for Chase pattern (default 2)
};

#endif // LED_TYPES_H
