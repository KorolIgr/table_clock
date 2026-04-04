#include "Chase.h"

Chase::Chase(LedStripType* ledStrip, uint16_t numLEDs)
    : _ledStrip(ledStrip), _numLEDs(numLEDs), _lastUpdate(0), _speed(500), _color(255, 255, 255),
      _direction(true), _trailLength(2), _currentIndex(0) {
}

void Chase::update(bool direction, uint16_t speed, RgbColor color, uint8_t trailLength) {
    _direction = direction;
    _speed = speed;
    _color = color;
    if (trailLength > 0) {
        _trailLength = trailLength;
        if (_trailLength > _numLEDs) {
            _trailLength = _numLEDs;
        }
    }

    if (millis() - _lastUpdate >= _speed) {
        _lastUpdate = millis();

        // Turn off all LEDs
        for (uint16_t i = 0; i < _numLEDs; i++) {
            _ledStrip->SetPixelColor(i, RgbColor(0, 0, 0));
        }

        // Light up the trail: currentIndex is the head, then trailLength-1 LEDs behind
        for (uint8_t t = 0; t < _trailLength; t++) {
            uint16_t ledIndex;
            if (_direction) {
                // Forward: head is _currentIndex, trail goes backwards
                if (_currentIndex >= t) {
                    ledIndex = _currentIndex - t;
                } else {
                    ledIndex = _numLEDs - (t - _currentIndex);
                }
            } else {
                // Reverse: head is _currentIndex, trail goes forwards
                ledIndex = (_currentIndex + t) % _numLEDs;
            }

            // Dim the LED based on its position in the trail
            uint8_t brightness = 255 - (t * (255 / _trailLength));
            if (brightness > 255) brightness = 255;
            RgbColor trailColor = RgbColor(
                (uint8_t)(_color.R * brightness / 255),
                (uint8_t)(_color.G * brightness / 255),
                (uint8_t)(_color.B * brightness / 255)
            );
            _ledStrip->SetPixelColor(ledIndex, trailColor);
        }

        _ledStrip->Show();

        // Move the head
        if (_direction) {
            _currentIndex++;
            if (_currentIndex >= _numLEDs) {
                _currentIndex = 0;
            }
        } else {
            if (_currentIndex == 0) {
                _currentIndex = _numLEDs - 1;
            } else {
                _currentIndex--;
            }
        }
    }
}
