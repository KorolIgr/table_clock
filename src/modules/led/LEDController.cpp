#include "LEDController.h"
#include "../config/ConfigManager.h"

LEDController::LEDController(uint8_t dataPin)
    : _dataStorage(nullptr), _dataPin(dataPin), _numLEDs(8), _lastUpdate(0),
      _runningLight(nullptr), _pingPong(nullptr),
      _rainbowWave(nullptr), _chase(nullptr), _blink(nullptr) {
    _ledStrip = new NeoPixelBus<NeoRgbFeature, NeoEsp8266BitBang800KbpsMethod>(_numLEDs, _dataPin);
}

void LEDController::begin() {
    _ledStrip->Begin();
    
    // Initialize pattern objects
    _runningLight = new RunningLight(_ledStrip, _numLEDs);
    _pingPong = new PingPong(_ledStrip, _numLEDs);
    _rainbowWave = new RainbowWave(_ledStrip, _numLEDs);
    _chase = new Chase(_ledStrip, _numLEDs);
    _blink = new Blink(_ledStrip, _numLEDs);
    
    // The pattern configuration will be loaded by the main application
    // using the ConfigManager, so we don't need to load it here anymore
    // Use default configuration
    _currentConfig.pattern = LEDPattern::RUNNING_LIGHT;
    _currentConfig.direction = true;
    _currentConfig.speed = 500;
    _currentConfig.color = RgbColor(255, 255, 255);
    _currentConfig.trailLength = 2;
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
        case LEDPattern::RAINBOW_WAVE:
            _rainbowWave->update(_currentConfig.speed, _currentConfig.color);
            break;
        case LEDPattern::CHASE:
            _chase->update(_currentConfig.direction, _currentConfig.speed, _currentConfig.color, _currentConfig.trailLength);
            break;
        case LEDPattern::BLINK:
            _blink->update(_currentConfig.speed, _currentConfig.color);
            break;
    }
}

 PatternConfig LEDController::getCurrentConfig() const {
     return _currentConfig;
 }
 
 void LEDController::setDataStorage(DataStorage* dataStorage) {
    _dataStorage = dataStorage;
}
