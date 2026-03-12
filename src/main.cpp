#include <Arduino.h>
#include <Wire.h>
#include "config.h"
#include "led_controller.h"
#include "display_controller.h"
#include "wifi_manager.h"

// =============================================================================
// Table Clock - Main Entry Point
// Stage 4: WiFi and Network Functionality
//   - Hybrid AP+STA mode
//   - Web server for WiFi configuration
//   - EEPROM credential storage
//   - Auto-connect and reconnect
// =============================================================================

// Global controller instances
LEDController    leds(LED_PIN, LED_COUNT);
DisplayController displays;
WiFiManager      wifiMgr;

// =============================================================================
// Forward declarations
// =============================================================================
void printSystemInfo();
void runLEDTests();
void runDisplayTests();
void setupWiFiCallbacks();
void printWiFiStatus();

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

    // ==========================================================================
    // Stage 4: WiFi Initialization
    // ==========================================================================
    Serial.println(F("\n[WiFi] ========================================"));
    Serial.println(F("[WiFi] Stage 4: WiFi Initialization"));
    Serial.println(F("[WiFi] ========================================"));

    // Setup WiFi event callbacks
    setupWiFiCallbacks();

    // Initialize WiFi Manager
    // This will:
    //  1. Start AP mode (TableClock-Setup)
    //  2. Load saved credentials from EEPROM
    //  3. Attempt STA connection if credentials exist
    //  4. Start web server on port 80
    wifiMgr.begin();

    // Print initial WiFi status
    printWiFiStatus();

    Serial.println(F("\n[MAIN] Setup complete. Entering main loop."));
    Serial.println(F("========================================\n"));
}

// =============================================================================
// Main Loop
// =============================================================================
void loop() {
    // --- WiFi Manager loop (MUST be called every iteration) ---
    // Handles: DNS captive portal, web server requests, reconnection logic
    wifiMgr.loop();

    // --- Heartbeat LED ---
    // Blink pattern depends on WiFi state:
    //   CONNECTED:    slow green pulse (5s)
    //   CONNECTING:   fast orange blink (1s)
    //   AP_ONLY:      slow blue pulse (5s)
    //   RECONNECTING: medium yellow blink (2s)
    static uint32_t lastHeartbeat = 0;
    static bool ledsOn = false;

    uint32_t now = millis();
    WifiMgrState wifiState = wifiMgr.getState();

    uint32_t heartbeatInterval = 5000;
    RGBColor heartbeatColor = Colors::BLUE;

    switch (wifiState) {
        case WifiMgrState::CONNECTED:
            heartbeatInterval = 5000;
            heartbeatColor = Colors::GREEN;
            break;
        case WifiMgrState::CONNECTING:
            heartbeatInterval = 500;
            heartbeatColor = Colors::ORANGE;
            break;
        case WifiMgrState::RECONNECTING:
            heartbeatInterval = 1000;
            heartbeatColor = Colors::YELLOW;
            break;
        case WifiMgrState::AP_ONLY:
        case WifiMgrState::DISCONNECTED:
        default:
            heartbeatInterval = 3000;
            heartbeatColor = Colors::BLUE;
            break;
    }

    if (now - lastHeartbeat >= heartbeatInterval) {
        lastHeartbeat = now;
        ledsOn = !ledsOn;

        if (ledsOn) {
            leds.setBrightness(20);
            leds.setAll(heartbeatColor);
            leds.show();
        } else {
            leds.clear();
            leds.show();
        }
    }

    // --- Periodic WiFi status print (every 30 seconds) ---
    static uint32_t lastStatusPrint = 0;
    if (now - lastStatusPrint >= 30000) {
        lastStatusPrint = now;
        printWiFiStatus();
    }

    // Yield to allow ESP8266 background tasks (WiFi stack, watchdog, etc.)
    yield();
}

// =============================================================================
// WiFi Callbacks Setup
// =============================================================================

/**
 * @brief Setup WiFi event callbacks for logging and display updates
 */
void setupWiFiCallbacks() {
    // Called when STA connection is established
    wifiMgr.onConnected([](const String& ssid, IPAddress ip) {
        Serial.println(F("\n[WiFi] *** CONNECTED ***"));
        Serial.printf("[WiFi] Network: %s\n", ssid.c_str());
        Serial.printf("[WiFi] IP Address: %s\n", ip.toString().c_str());
        Serial.printf("[WiFi] AP still active at: %s\n",
                      WiFi.softAPIP().toString().c_str());
        Serial.println(F("[WiFi] Web interface: http://") );
        Serial.println(ip.toString());
        Serial.println();

        // TODO (Stage 5): Update display to show IP address
        // TODO (Stage 6): Start NTP sync
    });

    // Called when STA connection is lost
    wifiMgr.onDisconnected([]() {
        Serial.println(F("\n[WiFi] *** DISCONNECTED ***"));
        Serial.println(F("[WiFi] Will attempt reconnection..."));
        Serial.println();

        // TODO (Stage 5): Update display to show disconnected status
    });

    // Called when new credentials are saved via web interface
    wifiMgr.onCredentialsSaved([](const String& ssid) {
        Serial.printf("\n[WiFi] New credentials saved for: %s\n", ssid.c_str());
        Serial.println(F("[WiFi] Attempting connection..."));
        Serial.println();
    });
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
 * @brief Print current WiFi status to serial
 */
void printWiFiStatus() {
    Serial.println(F("\n[WiFi] --- Status ---"));
    Serial.printf("[WiFi] State: %s\n", wifiMgr.getStateString().c_str());
    Serial.printf("[WiFi] AP: %s (IP: %s)\n",
                  WIFI_AP_SSID,
                  wifiMgr.getAPIP().toString().c_str());
    if (wifiMgr.isConnected()) {
        Serial.printf("[WiFi] STA: Connected to %s\n", wifiMgr.getConnectedSSID().c_str());
        Serial.printf("[WiFi] STA IP: %s\n", wifiMgr.getSTAIP().toString().c_str());
        Serial.printf("[WiFi] RSSI: %d dBm\n", wifiMgr.getRSSI());
    } else {
        Serial.println(F("[WiFi] STA: Not connected"));
        if (wifiMgr.hasCredentials()) {
            Serial.println(F("[WiFi] Saved credentials: YES"));
        } else {
            Serial.println(F("[WiFi] Saved credentials: NO"));
        }
    }
    Serial.printf("[WiFi] Free Heap: %d bytes\n", ESP.getFreeHeap());
    Serial.println(F("[WiFi] ---"));
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
