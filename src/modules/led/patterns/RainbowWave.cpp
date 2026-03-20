#include "RainbowWave.h"

RainbowWave::RainbowWave(LedStripType* ledStrip, uint16_t numLEDs)
    : _ledStrip(ledStrip), _numLEDs(numLEDs), _lastUpdate(0), _speed(100), _wavePosition(0), _saturation(255), _value(255) {
}

void RainbowWave::update(uint16_t speed, RgbColor color) {
    _speed = speed;

    if (millis() - _lastUpdate >= _speed) {
        _lastUpdate = millis();

        // Update wave position
        _wavePosition = (_wavePosition + 1) % 256;

        // Set each LED to a color based on its position in the wave
        for (uint16_t i = 0; i < _numLEDs; i++) {
            // Calculate hue for this LED: offset by index to spread rainbow
            uint16_t hue = (_wavePosition + (i * 256 / _numLEDs)) % 256;
            RgbColor rgb = hsvToRgb(hue, _saturation, _value);
            _ledStrip->SetPixelColor(i, rgb);
        }
        _ledStrip->Show();
    }
}

RgbColor RainbowWave::hsvToRgb(uint16_t hue, uint8_t saturation, uint8_t value) {
    // HSV to RGB conversion
    // hue: 0-255, saturation: 0-255, value: 0-255
    uint8_t r, g, b;

    uint8_t region = hue / 43; // 0-5
    uint8_t remainder = (hue - (region * 43)) * 6; // 0-255

    uint8_t p = (value * (255 - saturation)) >> 8;
    uint8_t q = (value * (255 - ((saturation * remainder) >> 8))) >> 8;
    uint8_t t = (value * (255 - ((saturation * (255 - remainder)) >> 8))) >> 8;

    switch (region) {
        case 0: r = value; g = t; b = p; break;
        case 1: r = q; g = value; b = p; break;
        case 2: r = p; g = value; b = t; break;
        case 3: r = p; g = q; b = value; break;
        case 4: r = t; g = p; b = value; break;
        default: r = value; g = p; b = q; break;
    }

    return RgbColor(r, g, b);
}
