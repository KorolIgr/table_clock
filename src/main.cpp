#include "main.h"

MainApplication::MainApplication()
    : _ledController(nullptr), _displayManager(nullptr), _wifiManager(nullptr) {
}

void MainApplication::begin() {
    initHardware();
    initWiFi();
    initDisplay();
    initLED();
}

void MainApplication::appLoop() {
    _wifiManager->update();
    _ledController->updatePattern();
}

void MainApplication::initHardware() {
    // Hardware initialization
}

void MainApplication::initWiFi() {
    _wifiManager = new WiFiManager();
    _wifiManager->begin(DEFAULT_AP_SSID, DEFAULT_AP_PASSWORD, WIFI_AP_STA);
}

void MainApplication::initDisplay() {
    // Initialize display manager with TCA9548A channel 0
    _displayManager = new DisplayManager(0x3C, I2C_SDA_PIN, I2C_SCL_PIN);
    _displayManager->begin();
}

void MainApplication::initLED() {
    _ledController = new LEDController(LED_DATA_PIN);
    _ledController->begin();
}

// Global application instance
MainApplication app;

// Standard Arduino setup function
void setup() {
    app.begin();
}

// Standard Arduino loop function
void loop() {
    app.appLoop();
}
