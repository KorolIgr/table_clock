#ifndef LED_CONTROLLER_H
#define LED_CONTROLLER_H

#include <NeoPixelBus.h>

class LEDController {
public:
    LEDController(uint8_t dataPin);
    
    void begin();
    void setAllLEDs(uint8_t red, uint8_t green, uint8_t blue);
    void setLED(uint8_t index, uint8_t red, uint8_t green, uint8_t blue);
    void show();
    void clear();

private:
    NeoPixelBus<NeoRgbFeature, NeoEsp8266Dma800KbpsMethod>* _ledStrip;
    uint8_t _dataPin;
    uint16_t _numLEDs;
};

#endif // LED_CONTROLLER_H
