#include "LEDController.h"

LEDController::LEDController(uint8_t dataPin)
    : _dataPin(dataPin), _numLEDs(8), _lastUpdate(0), _runningLight(nullptr), _pingPong(nullptr) {
    _ledStrip = new NeoPixelBus<NeoRgbFeature, NeoEsp8266BitBang800KbpsMethod>(_numLEDs, _dataPin);
}

void LEDController::begin() {
    _ledStrip->Begin();
    
    // Initialize pattern objects
    _runningLight = new RunningLight(_ledStrip, _numLEDs);
    _pingPong = new PingPong(_ledStrip, _numLEDs);
    
    // Load saved pattern configuration or use default if none exists
    loadPatternConfig();
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

void LEDController::savePatternConfig() {
    // Initialize EEPROM
    EEPROM.begin(512);
    
    // Save pattern configuration to EEPROM
    int addr = 0;
    EEPROM.put(addr, _currentConfig.pattern);
    addr += sizeof(_currentConfig.pattern);
    EEPROM.put(addr, _currentConfig.direction);
    addr += sizeof(_currentConfig.direction);
    EEPROM.put(addr, _currentConfig.speed);
    addr += sizeof(_currentConfig.speed);
    
    // Save color separately since RgbColor is a class
    uint32_t colorValue = (_currentConfig.color.R << 16) | (_currentConfig.color.G << 8) | _currentConfig.color.B;
    EEPROM.put(addr, colorValue);
    addr += sizeof(colorValue);
    
    // Commit to EEPROM
    EEPROM.commit();
}

PatternConfig LEDController::getCurrentConfig() const {
    return _currentConfig;
}

void LEDController::loadPatternConfig() {
    // Initialize EEPROM
    EEPROM.begin(512);
    
    // Load pattern configuration from EEPROM
    int addr = 0;
    LEDPattern savedPattern;
    EEPROM.get(addr, savedPattern);
    addr += sizeof(savedPattern);
    
    bool savedDirection;
    EEPROM.get(addr, savedDirection);
    addr += sizeof(savedDirection);
    
    uint16_t savedSpeed;
    EEPROM.get(addr, savedSpeed);
    addr += sizeof(savedSpeed);
    
    uint32_t savedColorValue;
    EEPROM.get(addr, savedColorValue);
    addr += sizeof(savedColorValue);
    
    // Check if the loaded values are valid (not uninitialized EEPROM values)
    if (savedPattern >= LEDPattern::RUNNING_LIGHT && savedPattern <= LEDPattern::PING_PONG) {
        _currentConfig.pattern = savedPattern;
        _currentConfig.direction = savedDirection;
        _currentConfig.speed = savedSpeed;
        
        // Restore color from packed value
        uint8_t r = (savedColorValue >> 16) & 0xFF;
        uint8_t g = (savedColorValue >> 8) & 0xFF;
        uint8_t b = savedColorValue & 0xFF;
        _currentConfig.color = RgbColor(r, g, b);
    } else {
        // If invalid values, use default configuration
        _currentConfig.pattern = LEDPattern::RUNNING_LIGHT;
        _currentConfig.direction = true;
        _currentConfig.speed = 500;
        _currentConfig.color = RgbColor(255, 255, 255);
    }
    
    // Clear all LEDs and reset pattern
    clear();
}
