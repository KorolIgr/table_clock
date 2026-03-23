#include "DisplayManager.h"

DisplayManager::DisplayManager(uint8_t multiplexerAddress, uint8_t channel, uint8_t sdaPin, uint8_t sclPin)
    : _multiplexerAddress(multiplexerAddress), _channel(channel), _sdaPin(sdaPin), _sclPin(sclPin) {
    _display = new Adafruit_SSD1306(SSD1306_LCDWIDTH, SSD1306_LCDHEIGHT, &Wire, -1);
}

void DisplayManager::begin() {
    Wire.begin(_sdaPin, _sclPin);
    Wire.setClock(10000);
    // Initialize the display with the default I2C address (0x3C for SSD1306)
    if (!_display->begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        // Display allocation failed
        return;
    }
    _display->clearDisplay();
    _display->display();
}

void DisplayManager::selectChannel() {
    // Select the appropriate channel on the TCA9548A multiplexer
    Wire.beginTransmission(_multiplexerAddress);
    Wire.write(1 << _channel);  // Enable only the specified channel
    Wire.endTransmission();
}

void DisplayManager::clear() {
    selectChannel();  // Select the correct channel before sending commands
    _display->clearDisplay();
}

void DisplayManager::display() {
    selectChannel();  // Select the correct channel before sending commands
    _display->display();
}

void DisplayManager::setText(const char* text) {
    selectChannel();  // Select the correct channel before sending commands
    delay(10);
    _display->clearDisplay();
    _display->setTextSize(1);
    _display->setTextColor(SSD1306_WHITE);
    _display->setCursor(0, 0);
    _display->print(text);
    display();
}

void DisplayManager::print(const char* text) {
    selectChannel();  // Select the correct channel before sending commands
    _display->print(text);
    display();
}

void DisplayManager::setCursor(uint8_t col, uint8_t row) {
    selectChannel();  // Select the correct channel before sending commands
    _display->setCursor(col, row);
}
