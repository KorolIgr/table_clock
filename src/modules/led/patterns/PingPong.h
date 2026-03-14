#ifndef PING_PONG_H
#define PING_PONG_H

#include <NeoPixelBus.h>

class PingPong {
public:
    PingPong(NeoPixelBus<NeoRgbFeature, NeoEsp8266Dma800KbpsMethod>* ledStrip, uint16_t numLEDs);
    void update(uint16_t speed, RgbColor color);

private:
    NeoPixelBus<NeoRgbFeature, NeoEsp8266Dma800KbpsMethod>* _ledStrip;
    uint16_t _numLEDs;
    uint16_t _currentIndex;
    uint32_t _lastUpdate;
    bool _directionForward;
    uint16_t _speed;
    RgbColor _color;
};

#endif // PING_PONG_H