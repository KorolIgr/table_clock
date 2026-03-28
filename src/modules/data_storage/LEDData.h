#ifndef LED_DATA_H
#define LED_DATA_H

#include <Arduino.h>
#include "../led/LEDTypes.h"  // For PatternConfig and LEDPattern

// LED data structure
struct LEDData {
    bool enabled = true;
    uint8_t brightness = 100;  // 0-100 percentage
    int builtin_led_pattern = 1;  // 0=off, 1=slow blink, 2=fast blink, 3=on
    
    // Additional LED controller settings
    PatternConfig pattern_config;
    
    LEDData() : pattern_config() {}
};

#endif // LED_DATA_H
