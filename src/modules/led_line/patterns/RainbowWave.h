#ifndef RAINBOW_WAVE_H
#define RAINBOW_WAVE_H

#include <NeoPixelBus.h>

typedef NeoPixelBus<NeoRgbFeature, NeoEsp8266BitBang800KbpsMethod> LedStripType;

class RainbowWave {
public:
    RainbowWave(LedStripType* ledStrip, uint16_t numLEDs);
    void update(uint16_t speed, RgbColor color); // color parameter ignored

private:
    LedStripType* _ledStrip;
    uint16_t _numLEDs;
    uint32_t _lastUpdate;
    uint16_t _speed;
    uint16_t _wavePosition; // position of the wave
    uint8_t _saturation;    // color saturation
    uint8_t _value;         // color brightness/value

    // Helper function to convert HSV to RGB
    RgbColor hsvToRgb(uint16_t hue, uint8_t saturation, uint8_t value);
};

#endif // RAINBOW_WAVE_H
