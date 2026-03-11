#include "display_controller.h"

// =============================================================================
// DisplayController Implementation
// Manages TCA9548A I2C multiplexer and 8x SSD1306 OLED displays
// =============================================================================

DisplayController::DisplayController()
    : _tcaFound(false)
    , _displayCount(0)
    , _currentChannel(-1)
{
    // Initialize all display objects with the correct dimensions
    for (uint8_t i = 0; i < DISPLAY_COUNT; i++) {
        _displays[i] = Adafruit_SSD1306(DISPLAY_WIDTH, DISPLAY_HEIGHT, &Wire, -1);
        _displayPresent[i] = false;
    }
}

// =============================================================================
// Initialization
// =============================================================================

bool DisplayController::begin() {
    Serial.println(F("[DISP] Initializing Display Controller..."));
    Serial.printf("[DISP] TCA9548A address: 0x%02X\n", TCA9548A_ADDR);
    Serial.printf("[DISP] SSD1306 address: 0x%02X\n", SSD1306_ADDR);
    Serial.printf("[DISP] Expected displays: %d\n", DISPLAY_COUNT);

    // Step 1: Check if TCA9548A is present
    Serial.println(F("[DISP] Scanning for TCA9548A..."));
    Wire.beginTransmission(TCA9548A_ADDR);
    uint8_t error = Wire.endTransmission();

    if (error == 0) {
        _tcaFound = true;
        Serial.printf("[DISP] TCA9548A found at 0x%02X OK\n", TCA9548A_ADDR);
    } else {
        _tcaFound = false;
        Serial.printf("[DISP] ERROR: TCA9548A NOT found at 0x%02X (error=%d)\n",
                      TCA9548A_ADDR, error);
        Serial.println(F("[DISP] Check wiring: SDA, SCL, VCC, GND, A0/A1/A2 to GND"));
        return false;
    }

    // Step 2: Disable all channels initially
    disableAllChannels();

    // Step 3: Initialize each display channel
    _displayCount = 0;
    for (uint8_t ch = 0; ch < DISPLAY_COUNT; ch++) {
        Serial.printf("[DISP] Initializing display on channel %d...\n", ch);
        if (initDisplay(ch)) {
            _displayPresent[ch] = true;
            _displayCount++;
            Serial.printf("[DISP] Display %d: OK\n", ch);
        } else {
            _displayPresent[ch] = false;
            Serial.printf("[DISP] Display %d: NOT FOUND or FAILED\n", ch);
        }
    }

    // Disable all channels after initialization
    disableAllChannels();

    Serial.printf("[DISP] Initialization complete: %d/%d displays found\n",
                  _displayCount, DISPLAY_COUNT);

    return (_displayCount > 0);
}

// =============================================================================
// TCA9548A Channel Control
// =============================================================================

bool DisplayController::selectChannel(uint8_t channel) {
    if (channel >= DISPLAY_COUNT) {
        Serial.printf("[DISP] ERROR: Invalid channel %d (max %d)\n", channel, DISPLAY_COUNT - 1);
        return false;
    }

    if (!_tcaFound) {
        return false;
    }

    // Write channel bitmask to TCA9548A
    // Bit N = 1 enables channel N
    uint8_t mask = (1 << channel);
    bool ok = writeTCA(mask);

    if (ok) {
        _currentChannel = (int8_t)channel;
    }

    return ok;
}

void DisplayController::disableAllChannels() {
    writeTCA(0x00);
    _currentChannel = -1;
}

bool DisplayController::writeTCA(uint8_t mask) {
    Wire.beginTransmission(TCA9548A_ADDR);
    Wire.write(mask);
    uint8_t error = Wire.endTransmission();
    return (error == 0);
}

// =============================================================================
// Display Access
// =============================================================================

bool DisplayController::isDisplayPresent(uint8_t channel) const {
    if (channel >= DISPLAY_COUNT) return false;
    return _displayPresent[channel];
}

Adafruit_SSD1306* DisplayController::getDisplay(uint8_t channel) {
    if (channel >= DISPLAY_COUNT) return nullptr;
    if (!_displayPresent[channel]) return nullptr;

    // Select the correct TCA9548A channel
    if (!selectChannel(channel)) return nullptr;

    return &_displays[channel];
}

// =============================================================================
// High-level Display Operations
// =============================================================================

void DisplayController::clearAll() {
    for (uint8_t ch = 0; ch < DISPLAY_COUNT; ch++) {
        if (_displayPresent[ch]) {
            selectChannel(ch);
            _displays[ch].clearDisplay();
        }
    }
    disableAllChannels();
}

void DisplayController::showAll() {
    for (uint8_t ch = 0; ch < DISPLAY_COUNT; ch++) {
        if (_displayPresent[ch]) {
            selectChannel(ch);
            _displays[ch].display();
        }
    }
    disableAllChannels();
}

bool DisplayController::clearDisplay(uint8_t channel) {
    if (!isDisplayPresent(channel)) return false;
    selectChannel(channel);
    _displays[channel].clearDisplay();
    return true;
}

bool DisplayController::showDisplay(uint8_t channel) {
    if (!isDisplayPresent(channel)) return false;
    selectChannel(channel);
    _displays[channel].display();
    return true;
}

// =============================================================================
// Test Functions
// =============================================================================

/**
 * @brief Scan I2C bus (0x00-0x7F) and print all found devices
 */
void DisplayController::testI2CScan() {
    Serial.println(F("\n[I2C] ---- I2C Bus Scan ----"));
    Serial.println(F("[I2C] Scanning addresses 0x01 to 0x7F..."));

    uint8_t found = 0;
    for (uint8_t addr = 1; addr < 128; addr++) {
        Wire.beginTransmission(addr);
        uint8_t error = Wire.endTransmission();

        if (error == 0) {
            Serial.printf("[I2C] Device found at 0x%02X", addr);
            // Identify known devices
            if (addr == TCA9548A_ADDR) {
                Serial.print(F(" <- TCA9548A (I2C Multiplexer)"));
            } else if (addr == SSD1306_ADDR || addr == 0x3D) {
                Serial.print(F(" <- SSD1306 (OLED Display)"));
            }
            Serial.println();
            found++;
        }
    }

    if (found == 0) {
        Serial.println(F("[I2C] No devices found! Check wiring."));
    } else {
        Serial.printf("[I2C] Total devices found: %d\n", found);
    }
    Serial.println(F("[I2C] ---- Scan Complete ----\n"));
}

/**
 * @brief Test TCA9548A by selecting each channel and scanning for devices
 */
void DisplayController::testTCAChannels() {
    Serial.println(F("\n[TCA] ---- TCA9548A Channel Test ----"));

    if (!_tcaFound) {
        Serial.println(F("[TCA] ERROR: TCA9548A not found, skipping test"));
        return;
    }

    for (uint8_t ch = 0; ch < DISPLAY_COUNT; ch++) {
        Serial.printf("[TCA] Channel %d: ", ch);
        selectChannel(ch);
        delay(10);

        // Scan for SSD1306 on this channel
        Wire.beginTransmission(SSD1306_ADDR);
        uint8_t error = Wire.endTransmission();

        if (error == 0) {
            Serial.printf("SSD1306 found at 0x%02X OK\n", SSD1306_ADDR);
        } else {
            Serial.printf("No device at 0x%02X (error=%d)\n", SSD1306_ADDR, error);
        }
    }

    disableAllChannels();
    Serial.println(F("[TCA] ---- Channel Test Complete ----\n"));
}

/**
 * @brief Test all displays by showing channel number on each
 */
void DisplayController::testDisplays() {
    Serial.println(F("\n[DISP] ---- Display Test Suite ----"));
    Serial.printf("[DISP] Testing %d displays...\n", _displayCount);

    if (_displayCount == 0) {
        Serial.println(F("[DISP] ERROR: No displays initialized, skipping test"));
        return;
    }

    // Test 1: Show channel number on each display
    Serial.println(F("[DISP] Test 1: Show channel number on each display"));
    for (uint8_t ch = 0; ch < DISPLAY_COUNT; ch++) {
        if (!_displayPresent[ch]) continue;

        selectChannel(ch);
        _displays[ch].clearDisplay();
        _displays[ch].setTextColor(SSD1306_WHITE);

        // Large channel number in center
        _displays[ch].setTextSize(4);
        _displays[ch].setCursor(48, 16);
        _displays[ch].print(ch);

        // Small label at top
        _displays[ch].setTextSize(1);
        _displays[ch].setCursor(0, 0);
        _displays[ch].print(F("Channel:"));

        _displays[ch].display();
        Serial.printf("[DISP] Display %d: showing channel number\n", ch);
        delay(500);
    }

    delay(2000);

    // Test 2: Show "HELLO" on all displays simultaneously
    Serial.println(F("[DISP] Test 2: Show HELLO on all displays"));
    for (uint8_t ch = 0; ch < DISPLAY_COUNT; ch++) {
        if (!_displayPresent[ch]) continue;

        selectChannel(ch);
        _displays[ch].clearDisplay();
        _displays[ch].setTextColor(SSD1306_WHITE);
        _displays[ch].setTextSize(2);
        _displays[ch].setCursor(20, 24);
        _displays[ch].print(F("HELLO!"));
        _displays[ch].display();
    }
    disableAllChannels();
    Serial.println(F("[DISP] All displays showing HELLO"));
    delay(2000);

    // Test 3: Clear all displays
    Serial.println(F("[DISP] Test 3: Clear all displays"));
    clearAll();
    showAll();
    Serial.println(F("[DISP] All displays cleared"));

    Serial.println(F("[DISP] ---- Display Test Suite Complete ----\n"));
}

/**
 * @brief Show text on a specific display
 */
bool DisplayController::showText(uint8_t channel, const char* text,
                                  uint8_t textSize, int16_t x, int16_t y) {
    if (!isDisplayPresent(channel)) {
        Serial.printf("[DISP] ERROR: Display %d not available\n", channel);
        return false;
    }

    selectChannel(channel);
    _displays[channel].clearDisplay();
    _displays[channel].setTextColor(SSD1306_WHITE);
    _displays[channel].setTextSize(textSize);
    _displays[channel].setCursor(x, y);
    _displays[channel].print(text);
    _displays[channel].display();

    return true;
}

/**
 * @brief Show test pattern (border + cross) on all displays
 */
void DisplayController::showTestPattern() {
    Serial.println(F("[DISP] Showing test pattern on all displays..."));

    for (uint8_t ch = 0; ch < DISPLAY_COUNT; ch++) {
        if (!_displayPresent[ch]) continue;

        selectChannel(ch);
        _displays[ch].clearDisplay();

        // Draw border
        _displays[ch].drawRect(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT, SSD1306_WHITE);

        // Draw diagonals
        _displays[ch].drawLine(0, 0, DISPLAY_WIDTH - 1, DISPLAY_HEIGHT - 1, SSD1306_WHITE);
        _displays[ch].drawLine(DISPLAY_WIDTH - 1, 0, 0, DISPLAY_HEIGHT - 1, SSD1306_WHITE);

        // Channel number in center
        _displays[ch].setTextColor(SSD1306_WHITE);
        _displays[ch].setTextSize(2);
        _displays[ch].setCursor(56, 24);
        _displays[ch].print(ch);

        _displays[ch].display();
    }

    disableAllChannels();
    Serial.println(F("[DISP] Test pattern displayed on all available displays"));
}

// =============================================================================
// Private: Initialize single display
// =============================================================================

bool DisplayController::initDisplay(uint8_t channel) {
    if (!selectChannel(channel)) {
        return false;
    }

    delay(10);  // Short delay after channel switch

    // Check if SSD1306 is present on this channel
    Wire.beginTransmission(SSD1306_ADDR);
    uint8_t error = Wire.endTransmission();

    if (error != 0) {
        // No device found on this channel
        return false;
    }

    // Initialize the display
    if (!_displays[channel].begin(SSD1306_SWITCHCAPVCC, SSD1306_ADDR)) {
        Serial.printf("[DISP] ERROR: SSD1306 init failed on channel %d\n", channel);
        return false;
    }

    // Clear display and show blank screen
    _displays[channel].clearDisplay();
    _displays[channel].display();

    return true;
}
