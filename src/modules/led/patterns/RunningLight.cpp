#include "RunningLight.h"

RunningLight::RunningLight(NeoPixelBus<NeoRgbFeature, NeoEsp8266Dma800KbpsMethod>* ledStrip, uint16_t numLEDs)
    : _ledStrip(ledStrip), _numLEDs(numLEDs), _currentIndex(0), _lastUpdate(0), _direction(true), _speed(500) {
}

void RunningLight::update(bool direction, uint16_t speed, RgbColor color) {
    _direction = direction;
    _speed = speed;
    _color = color;
    
    if (millis() - _lastUpdate >= _speed) {
        _lastUpdate = millis();
        
        // Turn off all LEDs
        for (uint16_t i = 0; i < _numLEDs; i++) {
            _ledStrip->SetPixelColor(i, RgbColor(0, 0, 0));
        }
        
        // Light up the current LED
        _ledStrip->SetPixelColor(_currentIndex, _color);
        _ledStrip->Show();
        
        // Update index based on direction
        if (_direction) {
            // Forward direction
            _currentIndex++;
            if (_currentIndex >= _numLEDs) {
                _currentIndex = 0;
            }
        } else {
            // Reverse direction
            if (_currentIndex == 0) {
                _currentIndex = _numLEDs - 1;
            } else {
                _currentIndex--;
            }
        }
    }
}