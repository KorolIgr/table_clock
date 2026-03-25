#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <Wire.h>
#include <U8g2lib.h>
#include "../data_storage/DataStorage.h"
#include "PageManager.h"

class DisplayManager {
public:
    DisplayManager(uint8_t multiplexerAddress, uint8_t channel, uint8_t sdaPin, uint8_t sclPin);
    
    void begin();
    void clear();
    void display();
    void setText(const char* text);
    void print(const char* text);
    void setCursor(uint8_t col, uint8_t row);
    void setDataStorage(DataStorage* dataStorage);
    void updateDisplay();
    void updatePageDisplay();
    void updateAllDisplays(DisplayManager** displays, uint8_t count);
    U8G2_SSD1306_128X64_NONAME_F_HW_I2C* getU8g2() const { return _display; }
    uint8_t getChannel() const { return _channel; }

private:
    uint8_t _multiplexerAddress;
    uint8_t _channel;
    uint8_t _sdaPin;
    uint8_t _sclPin;
    U8G2_SSD1306_128X64_NONAME_F_HW_I2C* _display;
    uint8_t _cursorX;
    uint8_t _cursorY;
    DataStorage* _dataStorage;
    PageManager* _pageManager;
    
    void selectChannel();
};

#endif // DISPLAY_MANAGER_H
