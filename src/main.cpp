#include <Arduino.h>
#include <Wire.h>
#include "config.h"
#include "led_controller.h"
#include "display_controller.h"

// =============================================================================
// Table Clock - Main Entry Point
// Stage 3: Basic ESP8266 initialization + APA106 LED testing + SSD1306 displays
// =============================================================================

// Global controller instances
LEDController leds(LED_PIN, LED_COUNT);
DisplayController displays;

// =============================================================================
// Forward declarations
// =============================================================================
void printSystemInfo();
void runLEDTests();
void runDisplayTests();

// =============================================================================
// Setup
// =============================================================================
void setup() {
    // Initialize serial communication
    Serial.begin(SERIAL_BAUD_RATE);
    delay(100);  // Wait for serial to stabilize

    Serial.println();
    Serial.println(F("========================================"));
    Serial.println(F("  Table Clock - ESP8266 Firmware"));
    Serial.printf( "  Version: %s\n", FW_VERSION_STR);
    Serial.println(F("========================================"));

    // Print system information
    printSystemInfo();

    // Initialize I2C bus
    Serial.println(F("\n[I2C] Initializing I2C bus..."));
    Wire.begin(SDA_PIN, SCL_PIN);
    Wire.setClock(400000);  // 400 kHz Fast Mode
    Serial.printf("[I2C] SDA: GPIO%d, SCL: GPIO%d, Speed: 400kHz\n", SDA_PIN, SCL_PIN);
    Serial.println(F("[I2C] I2C bus initialized OK"));

    // Initialize LED controller
    Serial.println(F("\n[LED] Initializing LED subsystem..."));
    if (!leds.begin()) {
        Serial.println(F("[LED] ERROR: Failed to initialize LED controller!"));
    }

    // Run LED tests to verify all 8 LEDs are working
    Serial.println(F("\n[TEST] Starting LED tests..."));
    runLEDTests();

    // Initialize Display controller (TCA9548A + SSD1306)
    Serial.println(F("\n[DISP] Initializing Display subsystem..."));
    if (!displays.begin()) {
        Serial.println(F("[DISP] WARNING: Display initialization failed or no displays found!"));
        Serial.println(F("[DISP] Continuing without displays..."));
    }

    // Run Display tests
    Serial.println(F("\n[TEST] Starting Display tests..."));
    runDisplayTests();

    Serial.println(F("\n[MAIN] Setup complete. Entering main loop."));
    Serial.println(F("========================================\n"));
}

// =============================================================================
// Main Loop
// =============================================================================
void loop() {
    // Simple heartbeat: pulse all LEDs in blue every 5 seconds
    static uint32_t lastHeartbeat = 0;
    static bool ledsOn = false;

    uint32_t now = millis();

    if (now - lastHeartbeat >= 5000) {
        lastHeartbeat = now;
        ledsOn = !ledsOn;

        if (ledsOn) {
            // Dim blue pulse to indicate the device is alive
            leds.setBrightness(20);
            leds.setAll(Colors::BLUE);
            leds.show();
        } else {
            leds.clear();
            leds.show();
        }
    }

    // Yield to allow ESP8266 background tasks (WiFi stack, watchdog, etc.)
    yield();
}

// =============================================================================
// Helper Functions
// =============================================================================

/**
 * @brief Print ESP8266 system information to serial
 */
void printSystemInfo() {
    Serial.println(F("\n[SYS] System Information:"));
    Serial.printf("[SYS] CPU Frequency: %d MHz\n", ESP.getCpuFreqMHz());
    Serial.printf("[SYS] Flash Size: %d KB\n", ESP.getFlashChipSize() / 1024);
    Serial.printf("[SYS] Free Heap: %d bytes\n", ESP.getFreeHeap());
    Serial.printf("[SYS] SDK Version: %s\n", ESP.getSdkVersion());
    Serial.printf("[SYS] Chip ID: 0x%08X\n", ESP.getChipId());
    Serial.printf("[SYS] Reset Reason: %s\n", ESP.getResetReason().c_str());
}

/**
 * @brief Run all LED tests to verify all 8 APA106 LEDs are working
 *        Tests are run sequentially with serial output for debugging
 */
void runLEDTests() {
    Serial.println(F("[TEST] ---- LED Test Suite ----"));
    Serial.printf("[TEST] Testing %d APA106 LEDs on GPIO%d\n", LED_COUNT, LED_PIN);

    // Test 1: Blink all LEDs (quick sanity check)
    Serial.println(F("\n[TEST] Test 1/4: Blink test"));
    leds.setBrightness(LED_DEFAULT_BRIGHTNESS);
    leds.testBlink(3, 300);
    delay(500);

    // Test 2: Sequential - light each LED one by one
    Serial.println(F("\n[TEST] Test 2/4: Sequential test (each LED individually)"));
    leds.testSequential(LED_TEST_DELAY_MS);
    delay(500);

    // Test 3: All colors - verify RGB channels
    Serial.println(F("\n[TEST] Test 3/4: All colors test (R, G, B, W, Y, C, M, O)"));
    leds.testAllColors(LED_TEST_DELAY_MS * 2);
    delay(500);

    // Test 4: Rainbow effect
    Serial.println(F("\n[TEST] Test 4/4: Rainbow effect"));
    leds.testRainbow(LED_TEST_DELAY_MS);
    delay(500);

    // Final state: all off
    leds.clear();
    leds.show();

    Serial.println(F("\n[TEST] ---- LED Test Suite Complete ----"));
    Serial.println(F("[TEST] If all 8 LEDs lit up correctly, hardware is OK."));
    Serial.println(F("[TEST] Check serial output above for any errors."));
}

/**
 * @brief Run all display tests to verify TCA9548A and SSD1306 displays
 *        Tests are run sequentially with serial output for debugging
 */
void runDisplayTests() {
    Serial.println(F("[TEST] ---- Display Test Suite ----"));

    // Test 1: I2C bus scan (before TCA channel selection)
    Serial.println(F("\n[TEST] Test 1/4: I2C bus scan"));
    displays.testI2CScan();

    // Test 2: TCA9548A channel scan
    Serial.println(F("\n[TEST] Test 2/4: TCA9548A channel scan"));
    displays.testTCAChannels();

    // Test 3: Display test (show channel number on each display)
    Serial.println(F("\n[TEST] Test 3/4: Display channel number test"));
    displays.testDisplays();

    // Test 4: Test pattern on all displays
    Serial.println(F("\n[TEST] Test 4/4: Test pattern (border + diagonals)"));
    displays.showTestPattern();
    delay(3000);

    // Final state: clear all displays
    displays.clearAll();
    displays.showAll();

    Serial.println(F("\n[TEST] ---- Display Test Suite Complete ----"));
    Serial.printf("[TEST] Displays found: %d/%d\n", displays.getDisplayCount(), DISPLAY_COUNT);
    Serial.println(F("[TEST] Check serial output above for any errors."));
}
