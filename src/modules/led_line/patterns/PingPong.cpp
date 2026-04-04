#include "PingPong.h"

#include "PingPong.h"

PingPong::PingPong(LedStripType* ledStrip, uint16_t numLEDs)
    : _ledStrip(ledStrip), _numLEDs(numLEDs), _currentIndex(0), _lastUpdate(0), _directionForward(true), _speed(500) {
}

void PingPong::update(uint16_t speed, RgbColor color) {
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
        
        // Move the index and change direction when reaching ends
        if (_directionForward) {
            _currentIndex++;
            if (_currentIndex >= _numLEDs - 1) {
                _currentIndex = _numLEDs > 0 ? _numLEDs - 1 : 0;
                _directionForward = false;
            }
        } else {
            if (_currentIndex == 0) {
                _directionForward = true;
            } else {
                _currentIndex--;
            }
        }
    }
}