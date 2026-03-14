#ifndef RUNNING_LIGHT_H
#define RUNNING_LIGHT_H

#include <NeoPixelBus.h>

class RunningLight {
public:
    RunningLight(NeoPixelBus<NeoRgbFeature, NeoEsp8266Dma800KbpsMethod>* ledStrip, uint16_t numLEDs);
    void update(bool direction, uint16_t speed, RgbColor color);

private:
    NeoPixelBus<NeoRgbFeature, NeoEsp8266Dma800KbpsMethod>* _ledStrip;
    uint16_t _numLEDs;
    uint16_t _currentIndex;
    uint32_t _lastUpdate;
    bool _direction;
    uint16_t _speed;
    RgbColor _color;
};

#endif // RUNNING_LIGHT_H