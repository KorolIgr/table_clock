#include "LEDController.h"

LEDController::LEDController(uint8_t dataPin)
    : _dataPin(dataPin), _numLEDs(8), _lastUpdate(0), _runningLight(nullptr), _pingPong(nullptr) {
    _ledStrip = new NeoPixelBus<NeoRgbFeature, NeoEsp8266Dma800KbpsMethod>(_numLEDs, _dataPin);
}

void LEDController::begin() {
    _ledStrip->Begin();
    
    // Initialize pattern objects
    _runningLight = new RunningLight(_ledStrip, _numLEDs);
    _pingPong = new PingPong(_ledStrip, _numLEDs);
    
    // Set default pattern to Running Light
    PatternConfig defaultConfig;
    defaultConfig.pattern = LEDPattern::RUNNING_LIGHT;
    defaultConfig.direction = true;
    defaultConfig.speed = 500;
    defaultConfig.color = RgbColor(255, 255, 255);
    _currentConfig = defaultConfig;
}

void LEDController::setAllLEDs(uint8_t red, uint8_t green, uint8_t blue) {
    for (uint8_t i = 0; i < _numLEDs; i++) {
        _ledStrip->SetPixelColor(i, RgbColor(red, green, blue));
    }
}

void LEDController::setLED(uint8_t index, uint8_t red, uint8_t green, uint8_t blue) {
    if (index < _numLEDs) {
        _ledStrip->SetPixelColor(index, RgbColor(red, green, blue));
    }
}

void LEDController::show() {
    _ledStrip->Show();
}

void LEDController::clear() {
    setAllLEDs(0, 0, 0);
    show();
}

void LEDController::setPattern(const PatternConfig& config) {
    _currentConfig = config;
    // Clear all LEDs first
    clear();
}

void LEDController::updatePattern() {
    switch (_currentConfig.pattern) {
        case LEDPattern::RUNNING_LIGHT:
            _runningLight->update(_currentConfig.direction, _currentConfig.speed, _currentConfig.color);
            break;
        case LEDPattern::PING_PONG:
            _pingPong->update(_currentConfig.speed, _currentConfig.color);
            break;
    }
}
