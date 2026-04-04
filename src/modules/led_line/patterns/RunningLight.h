#ifndef RUNNING_LIGHT_H
#define RUNNING_LIGHT_H

#include <NeoPixelBus.h>

typedef NeoPixelBus<NeoRgbFeature, NeoEsp8266BitBang800KbpsMethod> LedStripType;

class RunningLight {
public:
    RunningLight(LedStripType* ledStrip, uint16_t numLEDs);
    void update(bool direction, uint16_t speed, RgbColor color);

private:
    LedStripType* _ledStrip;
    uint16_t _numLEDs;
    uint16_t _currentIndex;
    uint32_t _lastUpdate;
    bool _direction;
    uint16_t _speed;
    RgbColor _color;
};

#endif // RUNNING_LIGHT_H