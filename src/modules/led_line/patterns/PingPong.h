#ifndef PING_PONG_H
#define PING_PONG_H

#include <NeoPixelBus.h>

typedef NeoPixelBus<NeoRgbFeature, NeoEsp8266BitBang800KbpsMethod> LedStripType;

class PingPong {
public:
    PingPong(LedStripType* ledStrip, uint16_t numLEDs);
    void update(uint16_t speed, RgbColor color);

private:
    LedStripType* _ledStrip;
    uint16_t _numLEDs;
    uint16_t _currentIndex;
    uint32_t _lastUpdate;
    bool _directionForward;
    uint16_t _speed;
    RgbColor _color;
};

#endif // PING_PONG_H