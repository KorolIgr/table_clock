#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

class DisplayManager {
public:
    DisplayManager(uint8_t multiplexerAddress, uint8_t channel, uint8_t sdaPin, uint8_t sclPin);
    
    void begin();
    void clear();
    void display();
    void setText(const char* text);
    void print(const char* text);
    void setCursor(uint8_t col, uint8_t row);

private:
    uint8_t _multiplexerAddress;
    uint8_t _channel;
    uint8_t _sdaPin;
    uint8_t _sclPin;
    Adafruit_SSD1306* _display;
    
    void selectChannel();
};

#endif // DISPLAY_MANAGER_H
