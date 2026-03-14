#include "main.h"
#include "modules/led/LEDController.h"

MainApplication::MainApplication()
    : _ledController(nullptr), _displayManager(nullptr), _wifiManager(nullptr) {
}

void MainApplication::begin() {
    // Initialize serial communication first
    Serial.begin(115200);
    delay(500); // Brief delay to allow serial to stabilize
    
    Serial.println("Initializing Table Clock Application...");
    
    initHardware();
    delay(100); // Small delay after hardware init
    
    //initWiFi();
    delay(100); // Small delay after WiFi init
    
    //initDisplay();
    delay(100); // Small delay after display init
    
    initLED();
    delay(100); // Small delay after LED init
    
    //connectLEDControllerToWiFi();
    
    Serial.println("Table Clock Application started successfully.");
}

void MainApplication::appLoop() {
    //_wifiManager->update();
    _ledController->updatePattern();
    
    // Periodic heartbeat to indicate the device is running
    static unsigned long lastHeartbeat = 0;
    if (millis() - lastHeartbeat > 5000) { // Every 5 seconds
        Serial.print("Device running... Heartbeat: ");
        Serial.println(millis());  // Using separate print calls to reduce potential memory issues
        lastHeartbeat = millis();
    }
}

void MainApplication::initHardware() {
    // Hardware initialization
}

void MainApplication::initWiFi() {
    _wifiManager = new WiFiManager();
    
    // Check if we have stored WiFi credentials
    String storedSsid, storedPassword;
    if (_wifiManager->loadStoredCredentials(storedSsid, storedPassword)) {
        // Use stored credentials for both AP and STA modes
        _wifiManager->begin(storedSsid.c_str(), storedPassword.c_str(), WIFI_AP_STA);
    } else {
        // Use default AP credentials, but don't try to connect to STA without stored credentials
        _wifiManager->begin(DEFAULT_AP_SSID, DEFAULT_AP_PASSWORD, WIFI_AP_STA);
    }
}

void MainApplication::initDisplay() {
    // Initialize display manager with TCA9548A channel 0
    _displayManager = new DisplayManager(0x3C, I2C_SDA_PIN, I2C_SCL_PIN);
    _displayManager->begin();
}

void MainApplication::initLED() {
    _ledController = new LEDController(LED_DATA_PIN);
    _ledController->begin();
    
    // Set a default pattern to run immediately after boot
    PatternConfig defaultConfig;
    defaultConfig.pattern = LEDPattern::RUNNING_LIGHT;
    defaultConfig.direction = true;
    defaultConfig.speed = 500;
    defaultConfig.color = RgbColor(255, 0, 0); // Red color for visibility
    
    _ledController->setPattern(defaultConfig);
}

void MainApplication::connectLEDControllerToWiFi() {
    if (_wifiManager && _ledController) {
        _wifiManager->setLEDController(_ledController);
    }
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
