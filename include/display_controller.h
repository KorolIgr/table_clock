#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "config.h"

// =============================================================================
// Display Controller for TCA9548A + 8x SSD1306
// TCA9548A is an I2C multiplexer that allows connecting up to 8 devices
// with the same I2C address on separate channels (0-7).
// =============================================================================

// Result codes for display operations
enum class DisplayResult {
    OK = 0,
    TCA_NOT_FOUND,
    DISPLAY_NOT_FOUND,
    DISPLAY_INIT_FAILED,
    INVALID_CHANNEL
};

class DisplayController {
public:
    // Constructor
    DisplayController();

    // Initialization
    // Returns true if TCA9548A is found and at least one display is initialized
    bool begin();

    // TCA9548A channel control
    // channel: 0-7, or 0xFF to disable all channels
    bool selectChannel(uint8_t channel);
    void disableAllChannels();

    // Check if TCA9548A is present on I2C bus
    bool isTCAPresent() const { return _tcaFound; }

    // Check if a specific display channel is available
    bool isDisplayPresent(uint8_t channel) const;

    // Get number of detected displays
    uint8_t getDisplayCount() const { return _displayCount; }

    // Get display object for a specific channel (nullptr if not available)
    // Automatically selects the correct TCA9548A channel
    Adafruit_SSD1306* getDisplay(uint8_t channel);

    // High-level display operations (operate on all displays)
    void clearAll();
    void showAll();

    // High-level display operations (operate on single display)
    bool clearDisplay(uint8_t channel);
    bool showDisplay(uint8_t channel);

    // Test functions
    // Test 1: Scan I2C bus and report found devices
    void testI2CScan();

    // Test 2: Test TCA9548A - select each channel and scan for devices
    void testTCAChannels();

    // Test 3: Test all displays - show channel number on each display
    void testDisplays();

    // Test 4: Show text on a specific display
    bool showText(uint8_t channel, const char* text, uint8_t textSize = 1,
                  int16_t x = 0, int16_t y = 0);

    // Test 5: Show test pattern on all displays
    void showTestPattern();

private:
    // Array of display objects, one per TCA9548A channel
    Adafruit_SSD1306 _displays[DISPLAY_COUNT];

    // Track which channels have working displays
    bool _displayPresent[DISPLAY_COUNT];

    // TCA9548A found flag
    bool _tcaFound;

    // Number of successfully initialized displays
    uint8_t _displayCount;

    // Currently selected TCA9548A channel (-1 = none)
    int8_t _currentChannel;

    // Internal: initialize a single display on given channel
    bool initDisplay(uint8_t channel);

    // Internal: write to TCA9548A control register
    bool writeTCA(uint8_t mask);
};
