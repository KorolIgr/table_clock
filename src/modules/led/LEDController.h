#ifndef LED_CONTROLLER_H
#define LED_CONTROLLER_H

#include <NeoPixelBus.h>
#include "patterns/RunningLight.h"
#include "patterns/PingPong.h"
#include "patterns/RainbowWave.h"
#include "patterns/Chase.h"
#include "patterns/Blink.h"
#include "../data_storage/DataStorage.h"

// Enum for LED patterns
enum class LEDPattern {
    RUNNING_LIGHT,
    PING_PONG,
    RAINBOW_WAVE,
    CHASE,
    BLINK
};

// Struct for pattern configuration
struct PatternConfig {
    LEDPattern pattern = LEDPattern::RUNNING_LIGHT;
    bool direction = true;  // true = forward, false = reverse
    uint16_t speed = 500;   // in milliseconds
    RgbColor color = RgbColor(20, 20, 20);  // white by default
    uint8_t trailLength = 2;  // for Chase pattern (default 2)
};

class LEDController {
public:
    LEDController(uint8_t dataPin);
    
    // Dependency injection for data storage
    void setDataStorage(DataStorage* dataStorage);
    
    void begin();
    void setAllLEDs(uint8_t red, uint8_t green, uint8_t blue);
    void setLED(uint8_t index, uint8_t red, uint8_t green, uint8_t blue);
    void show();
    void clear();
    void setPattern(const PatternConfig& config);
    void updatePattern();
    // Removed savePatternConfig and loadPatternConfig - now handled by ConfigManager
    PatternConfig getCurrentConfig() const;

private:
    DataStorage* _dataStorage;            // Pointer to shared data storage
    NeoPixelBus<NeoRgbFeature, NeoEsp8266BitBang800KbpsMethod>* _ledStrip;
    uint8_t _dataPin;
    uint16_t _numLEDs;
    PatternConfig _currentConfig;
    uint32_t _lastUpdate;
    
    // Pattern instances
    RunningLight* _runningLight;
    PingPong* _pingPong;
    RainbowWave* _rainbowWave;
    Chase* _chase;
    Blink* _blink;
};

#endif // LED_CONTROLLER_H
