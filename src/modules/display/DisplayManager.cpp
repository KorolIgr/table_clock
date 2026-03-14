#include "DisplayManager.h"

DisplayManager::DisplayManager(uint8_t address, uint8_t sdaPin, uint8_t sclPin)
    : _address(address), _sdaPin(sdaPin), _sclPin(sclPin) {
    _display = new Adafruit_SSD1306(SSD1306_LCDWIDTH, SSD1306_LCDHEIGHT, &Wire, -1);
}

void DisplayManager::begin() {
    Wire.begin(_sdaPin, _sclPin);
    if (!_display->begin(SSD1306_SWITCHCAPVCC, _address)) {
        // Display allocation failed
        return;
    }
    _display->clearDisplay();
    _display->display();
}

void DisplayManager::clear() {
    _display->clearDisplay();
}

void DisplayManager::display() {
    _display->display();
}

void DisplayManager::setText(const char* text) {
    _display->clearDisplay();
    _display->setTextSize(1);
    _display->setTextColor(SSD1306_WHITE);
    _display->setCursor(0, 0);
    _display->print(text);
    display();
}

void DisplayManager::print(const char* text) {
    _display->print(text);
    display();
}

void DisplayManager::setCursor(uint8_t col, uint8_t row) {
    _display->setCursor(col, row);
}
