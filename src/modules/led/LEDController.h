#ifndef LED_CONTROLLER_H
#define LED_CONTROLLER_H

#include <NeoPixelBus.h>
#include "patterns/RunningLight.h"
#include "patterns/PingPong.h"

// Enum for LED patterns
enum class LEDPattern {
    RUNNING_LIGHT,
    PING_PONG
};

// Struct for pattern configuration
struct PatternConfig {
    LEDPattern pattern = LEDPattern::RUNNING_LIGHT;
    bool direction = true;  // true = forward, false = reverse
    uint16_t speed = 500;   // in milliseconds
    RgbColor color = RgbColor(255, 255, 255);  // white by default
};

class LEDController {
public:
    LEDController(uint8_t dataPin);
    
    void begin();
    void setAllLEDs(uint8_t red, uint8_t green, uint8_t blue);
    void setLED(uint8_t index, uint8_t red, uint8_t green, uint8_t blue);
    void show();
    void clear();
    void setPattern(const PatternConfig& config);
    void updatePattern();

private:
    NeoPixelBus<NeoRgbFeature, NeoEsp8266Dma800KbpsMethod>* _ledStrip;
    uint8_t _dataPin;
    uint16_t _numLEDs;
    PatternConfig _currentConfig;
    uint32_t _lastUpdate;
    
    // Pattern instances
    RunningLight* _runningLight;
    PingPong* _pingPong;
};

#endif // LED_CONTROLLER_H
