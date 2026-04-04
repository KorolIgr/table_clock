#include "BuiltInLED.h"
#include "../../app_config.h"

BuiltInLED::BuiltInLED()
    : _dataStorage(nullptr), _lastToggle(0), _state(false), _pattern(1) {
}

void BuiltInLED::setDataStorage(DataStorage* dataStorage) {
    _dataStorage = dataStorage;
}

void BuiltInLED::begin() {
    pinMode(BUILTIN_LED_PIN, OUTPUT);
    digitalWrite(BUILTIN_LED_PIN, HIGH); // Start with LED off (active low)
}

void BuiltInLED::update() {
    unsigned long currentTime = millis();
    
    switch (_pattern) {
        case 0: // Off
            digitalWrite(BUILTIN_LED_PIN, HIGH); // Turn off built-in LED (active low)
            break;
        case 1: // Slow blink (5s interval)
            if (currentTime - _lastToggle >= 5000) {
                _state = !_state;
                digitalWrite(BUILTIN_LED_PIN, _state ? LOW : HIGH); // Active low
                _lastToggle = currentTime;
            }
            break;
        case 2: // Fast blink (4Hz / 250ms)
            if (currentTime - _lastToggle >= 250) {
                _state = !_state;
                digitalWrite(BUILTIN_LED_PIN, _state ? LOW : HIGH); // Active low
                _lastToggle = currentTime;
            }
            break;
        case 3: // On
            digitalWrite(BUILTIN_LED_PIN, LOW); // Turn on built-in LED (active low)
            break;
    }
}

void BuiltInLED::setPattern(int pattern) {
    _pattern = pattern;
    _lastToggle = millis(); // Reset timer to avoid immediate toggle
}
