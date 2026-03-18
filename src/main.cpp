#include "main.h"
#include "modules/led/LEDController.h"
#include <functional>

MainApplication::MainApplication()
    : _ledController(nullptr), _displayManager(nullptr), _wifiManager(nullptr), _configManager(nullptr) {
}

void MainApplication::begin() {
    // Initialize serial communication first
    Serial.begin(115200);
    delay(500); // Brief delay to allow serial to stabilize
    
    Serial.println("Initializing Table Clock Application...");
    
    initConfig();
    delay(100); // Small delay after config init
    
    initHardware();
    delay(100); // Small delay after hardware init
    
    initWiFi();
    delay(100); // Small delay after WiFi init
    
    //initDisplay();
    delay(100); // Small delay after display init
    
    initLED();
    delay(100); // Small delay after LED init
    
    connectLEDControllerToWiFi();
    
    Serial.println("Table Clock Application started successfully.");
}

void MainApplication::appLoop() {
    // Add null pointer checks before calling update methods
    if (_wifiManager) {
        _wifiManager->update();
    }
    
    if (_ledController) {
        _ledController->updatePattern();
    }
    
    // Periodic heartbeat to indicate the device is running
    static unsigned long lastHeartbeat = 0;
    if (millis() - lastHeartbeat > 5000) { // Every 5 seconds
        Serial.print("Device running... Heartbeat: ");
        Serial.println(millis());  // Using separate print calls to reduce potential memory issues
        lastHeartbeat = millis();
    }
}

void MainApplication::initConfig() {
    _configManager = new ConfigManager();
    
    if (_configManager) {
        // Load configuration from EEPROM
        if (_configManager->loadConfig(_deviceConfig)) {
            Serial.println("Configuration loaded successfully from EEPROM");
        } else {
            Serial.println("Using default configuration (failed to load from EEPROM)");
            _deviceConfig = _configManager->getDefaultConfig();
        }
    }
}

void MainApplication::initHardware() {
    // Hardware initialization
}

void MainApplication::initWiFi() {
    _wifiManager = new WiFiManager();
    
    if (_wifiManager) {
        // Use configuration from the config manager
        if (strlen(_deviceConfig.wifi.sta_ssid) > 0 && strlen(_deviceConfig.wifi.sta_password) >= 8) {
            // Use stored credentials for both AP and STA modes
            _wifiManager->begin(_deviceConfig.wifi.sta_ssid, _deviceConfig.wifi.sta_password, WIFI_AP_STA);
        } else {
            // Use configured AP credentials, but don't try to connect to STA without stored credentials
            _wifiManager->begin(_deviceConfig.wifi.ap_ssid, _deviceConfig.wifi.ap_password, WIFI_AP_STA);
        }
        
        // Small delay after WiFi initialization
        delay(200);
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
    
    // Load LED configuration from the device config
    _ledController->setPattern(_deviceConfig.led);
}

void MainApplication::connectLEDControllerToWiFi() {
    if (_wifiManager && _ledController) {
        _wifiManager->setLEDController(_ledController);
        // Set up callbacks for configuration management
        // Set up static callback functions that will call the instance methods
        _wifiManager->setCallbacks(
            [](const char* ssid, const char* password) {
                if (appInstance) appInstance->saveWifiStaConfig(ssid, password);
            },
            [](const char* ssid, const char* password) {
                if (appInstance) appInstance->saveWifiApConfig(ssid, password);
            },
            []() {
                if (appInstance) appInstance->forgetWifiConfig();
            }
        );
    }
}

// Global application instance
MainApplication app;
MainApplication* appInstance = &app;  // Global pointer to access the instance from callbacks

// Standard Arduino setup function
void setup() {
    app.begin();
}

// Standard Arduino loop function
void loop() {
    app.appLoop();
}

// Implementation of configuration management methods
void MainApplication::saveWifiStaConfig(const char* ssid, const char* password) {
    if (_configManager && _wifiManager) {
        // Update the configuration
        strncpy(_deviceConfig.wifi.sta_ssid, ssid, sizeof(_deviceConfig.wifi.sta_ssid) - 1);
        _deviceConfig.wifi.sta_ssid[sizeof(_deviceConfig.wifi.sta_ssid) - 1] = '\0';
        
        strncpy(_deviceConfig.wifi.sta_password, password, sizeof(_deviceConfig.wifi.sta_password) - 1);
        _deviceConfig.wifi.sta_password[sizeof(_deviceConfig.wifi.sta_password) - 1] = '\0';
        
        // Save to EEPROM
        _configManager->saveConfig(_deviceConfig);
        
        // Restart WiFi with new credentials
        _wifiManager->begin(_deviceConfig.wifi.sta_ssid, _deviceConfig.wifi.sta_password, WIFI_AP_STA);
    }
}

void MainApplication::saveWifiApConfig(const char* ssid, const char* password) {
    if (_configManager && _wifiManager) {
        // Update the configuration
        strncpy(_deviceConfig.wifi.ap_ssid, ssid, sizeof(_deviceConfig.wifi.ap_ssid) - 1);
        _deviceConfig.wifi.ap_ssid[sizeof(_deviceConfig.wifi.ap_ssid) - 1] = '\0';
        
        strncpy(_deviceConfig.wifi.ap_password, password, sizeof(_deviceConfig.wifi.ap_password) - 1);
        _deviceConfig.wifi.ap_password[sizeof(_deviceConfig.wifi.ap_password) - 1] = '\0';
        
        // Save to EEPROM
        _configManager->saveConfig(_deviceConfig);
        
        // Restart WiFi with new credentials
        _wifiManager->begin(_deviceConfig.wifi.ap_ssid, _deviceConfig.wifi.ap_password, WIFI_AP_STA);
    }
}

void MainApplication::forgetWifiConfig() {
    if (_configManager && _wifiManager) {
        // Clear the configuration
        strcpy(_deviceConfig.wifi.sta_ssid, "");
        strcpy(_deviceConfig.wifi.sta_password, "");
        
        // Save to EEPROM
        _configManager->saveConfig(_deviceConfig);
        
        // Restart WiFi with default credentials
        _wifiManager->begin(DEFAULT_AP_SSID, DEFAULT_AP_PASSWORD, WIFI_AP_STA);
    }
}

void MainApplication::saveLedConfig(const PatternConfig& config) {
    if (_configManager) {
        // Update the configuration
        _deviceConfig.led = config;
        
        // Save to EEPROM
        _configManager->saveConfig(_deviceConfig);
        
        // Update the LED controller
        if (_ledController) {
            _ledController->setPattern(_deviceConfig.led);
        }
    }
}
