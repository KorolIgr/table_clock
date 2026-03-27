#include "DisplayManager.h"
#include <U8g2lib.h>
#include "PageManager.h"

DisplayManager::DisplayManager(uint8_t multiplexerAddress, uint8_t channel, uint8_t sdaPin, uint8_t sclPin)
    : _multiplexerAddress(multiplexerAddress), _channel(channel), _sdaPin(sdaPin), _sclPin(sclPin), _dataStorage(nullptr), _pageManager(nullptr) {
    _display = new U8G2_SSD1306_128X64_NONAME_F_HW_I2C(U8G2_R1, U8X8_PIN_NONE);
    _cursorX = 0;
    _cursorY = 0;
}

void DisplayManager::begin() {
    Wire.begin(_sdaPin, _sclPin);
    Wire.setClock(10000);
    
    // Configure U8g2 for the specific display
    selectChannel();  // Select the correct channel before initializing
    _display->setI2CAddress(0x3C << 1); // U8g2 uses shifted address
    _display->begin();
    _display->clearBuffer();
    _display->sendBuffer();
}

void DisplayManager::selectChannel() {
    // Select the appropriate channel on the TCA9548A multiplexer
    Wire.beginTransmission(_multiplexerAddress);
    Wire.write(1 << _channel);  // Enable only the specified channel
    Wire.endTransmission();
}

void DisplayManager::clear() {
    selectChannel();  // Select the correct channel before sending commands
    _display->clearBuffer();
}

void DisplayManager::display() {
    selectChannel();  // Select the correct channel before sending commands
    _display->sendBuffer();
}

void DisplayManager::setText(const char* text) {
    selectChannel();  // Select the correct channel before sending commands
    _display->clearBuffer();
    _display->setFont(u8g2_font_fub30_tf);  // Use a basic font
    _display->drawStr(0, 40, text);  // Draw text at x=0, y=12 (y position is font-dependent)
    display();
}

void DisplayManager::print(const char* text) {
    selectChannel();  // Select the correct channel before sending commands
    // For U8g2, we'll draw the text at the current cursor position
    // Since U8g2 doesn't have a print function like Adafruit, we'll use drawStr
    _display->drawStr(_cursorX, _cursorY, text);
    display();
}

void DisplayManager::setCursor(uint8_t col, uint8_t row) {
    selectChannel();  // Select the correct channel before sending commands
    _cursorX = col;
    _cursorY = row;
}

void DisplayManager::setDataStorage(DataStorage* dataStorage) {
    _dataStorage = dataStorage;
    // Create PageManager when DataStorage is set
    if (_dataStorage && !_pageManager) {
        _pageManager = new PageManager(_dataStorage);
    }
}

/*
void DisplayManager::updateDisplay() {
    if (_dataStorage) {
        updatePageDisplay();
    }
}

void DisplayManager::updatePageDisplay() {
    if (_pageManager && _display) {
        selectChannel();
        _pageManager->updatePageDisplay(_display);
    }
}

*/
void DisplayManager::updateAllDisplays(DisplayManager** displays, uint8_t count) {
    if (_pageManager) {
        _pageManager->updateAllDisplays(displays, count);
    }
}
