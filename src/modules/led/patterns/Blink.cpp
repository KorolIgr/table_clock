#include "Blink.h"

Blink::Blink(LedStripType* ledStrip, uint16_t numLEDs)
    : _ledStrip(ledStrip), _numLEDs(numLEDs), _lastUpdate(0), _speed(500), _color(255, 255, 255), _ledState(false) {
}

void Blink::update(uint16_t speed, RgbColor color) {
    _speed = speed;
    _color = color;

    if (millis() - _lastUpdate >= _speed) {
        _lastUpdate = millis();
        _ledState = !_ledState;

        RgbColor outputColor = _ledState ? _color : RgbColor(0, 0, 0);
        for (uint16_t i = 0; i < _numLEDs; i++) {
            _ledStrip->SetPixelColor(i, outputColor);
        }
        _ledStrip->Show();
    }
}
