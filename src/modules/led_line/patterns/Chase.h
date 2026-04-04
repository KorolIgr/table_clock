#ifndef CHASE_H
#define CHASE_H

#include <NeoPixelBus.h>

typedef NeoPixelBus<NeoRgbFeature, NeoEsp8266BitBang800KbpsMethod> LedStripType;

class Chase {
public:
    Chase(LedStripType* ledStrip, uint16_t numLEDs);
    void update(bool direction, uint16_t speed, RgbColor color, uint8_t trailLength);

private:
    LedStripType* _ledStrip;
    uint16_t _numLEDs;
    uint32_t _lastUpdate;
    uint16_t _speed;
    RgbColor _color;
    bool _direction;
    uint8_t _trailLength;
    uint16_t _currentIndex;
};

#endif // CHASE_H
