#include "LEDController.h"

LEDController::LEDController(uint8_t dataPin) 
    : _dataPin(dataPin), _numLEDs(8) {
    _ledStrip = new NeoPixelBus<NeoRgbFeature, NeoEsp8266Dma800KbpsMethod>(_numLEDs, _dataPin);
}

void LEDController::begin() {
    _ledStrip->Begin();
}

void LEDController::setAllLEDs(uint8_t red, uint8_t green, uint8_t blue) {
    for (uint8_t i = 0; i < _numLEDs; i++) {
        _ledStrip->SetPixelColor(i, RgbColor(red, green, blue));
    }
}

void LEDController::setLED(uint8_t index, uint8_t red, uint8_t green, uint8_t blue) {
    if (index < _numLEDs) {
        _ledStrip->SetPixelColor(index, RgbColor(red, green, blue));
    }
}

void LEDController::show() {
    _ledStrip->Show();
}

void LEDController::clear() {
    setAllLEDs(0, 0, 0);
    show();
}
