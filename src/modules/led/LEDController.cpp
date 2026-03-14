#include "LEDController.h"

LEDController::LEDController(uint8_t dataPin) 
    : _dataPin(dataPin), _numLEDs(8) {
    _ledStrip = new APA106(_dataPin, _numLEDs);
}

void LEDController::begin() {
    _ledStrip->begin();
}

void LEDController::setAllLEDs(uint8_t red, uint8_t green, uint8_t blue) {
    for (uint8_t i = 0; i < _numLEDs; i++) {
        _ledStrip->setRGB(i, red, green, blue);
    }
}

void LEDController::setLED(uint8_t index, uint8_t red, uint8_t green, uint8_t blue) {
    if (index < _numLEDs) {
        _ledStrip->setRGB(index, red, green, blue);
    }
}

void LEDController::show() {
    _ledStrip->show();
}

void LEDController::clear() {
    setAllLEDs(0, 0, 0);
    show();
}
