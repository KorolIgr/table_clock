#include "main.h"
#include "modules/led/LEDController.h"
#include "modules/builtin_led/BuiltInLED.h"
#include "modules/wifi/WiFiAP.h"
#include "modules/wifi/WiFiSTA.h"
#include "modules/webserver/WebServer.h"
#include "modules/data_storage/DataStorage.h"
#include "modules/config/ConfigManager.h"
#include <functional>

MainApplication::MainApplication()
    : _ledController(nullptr), _displayManager(nullptr),
      _configManager(nullptr), _dataStorage(nullptr),
      _builtInLED(nullptr), _wifiAP(nullptr), _wifiSTA(nullptr), _wifiWebServer(nullptr) {
}

void MainApplication::begin() {
    // Initialize serial communication first
    Serial.begin(115200);
    delay(500); // Brief delay to allow serial to stabilize
    
    Serial.println("Initializing Table Clock Application...");
    
    initConfig();
    delay(50); // Small delay after config init
    
    initHardware();
    delay(20); // Small delay after hardware init

    //initDisplay();
    delay(20); // Small delay after display init
    
    initLED();
    delay(20); // Small delay after LED init

    initWiFiAP();
    initWiFiSTA();
    initWebServer();
    delay(20); // Combined delay was 200ms at end of old initWiFi()
    

    
    //connectLEDControllerToWiFi();
    
    Serial.println("Table Clock Application started successfully.");
}

void MainApplication::appLoop() {
    // Update WiFi STA (monitor connection status)
    if (_wifiSTA) {
        _wifiSTA->update();
    }
    
    // Update web server
    if (_wifiWebServer) {
        _wifiWebServer->update();
    }
    
    // Update built-in LED based on WiFi STA connection status
    if (_builtInLED && _wifiSTA) {
        bool connected = _wifiSTA->isConnected();
        static bool lastConnected = !connected; // Opposite to trigger initial set
        if (connected != lastConnected) {
            lastConnected = connected;
            if (connected) {
                _builtInLED->setPattern(3); // On
            } else {
                _builtInLED->setPattern(1); // Slow blink
            }
        }
        // Actually update the LED state
        _builtInLED->update();
    }
    
    // Update external LED controller
    if (_ledController) {
        _ledController->updatePattern();
    }
    
    // Periodic heartbeat to indicate the device is running
    static unsigned long lastHeartbeat = 0;
    if (millis() - lastHeartbeat > 5000) { // Every 5 seconds
        Serial.print("Device running... Heartbeat: ");
        Serial.println(millis());
        lastHeartbeat = millis();
    }
}

void MainApplication::initConfig() {
    _dataStorage = new DataStorage();
    _configManager = new ConfigManager();
    
    if (_configManager) {
        // Inject data storage into config manager
        _configManager->setDataStorage(_dataStorage);
        
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
    // Hardware initialization (if needed)
}

void MainApplication::initWiFiAP() {
    // Create and initialize WiFi AP
    _wifiAP = new WiFiAP();
    
    // Inject DataStorage
    if (_dataStorage) {
        _wifiAP->setDataStorage(_dataStorage);
    }
    
    // Initialize AP
    if (_wifiAP) {
        _wifiAP->begin(_deviceConfig.wifi.ap_ssid, _deviceConfig.wifi.ap_password, _deviceConfig.wifi.ap_ip);
    }
}

void MainApplication::initWiFiSTA() {
    // Create and initialize WiFi STA
    _wifiSTA = new WiFiSTA();
    
    // Inject DataStorage
    if (_dataStorage) {
        _wifiSTA->setDataStorage(_dataStorage);
    }
    
    // Initialize STA (if credentials available)
    bool hasStaCredentials = strlen(_deviceConfig.wifi.sta_ssid) > 0 && strlen(_deviceConfig.wifi.sta_password) >= 8;
    if (_wifiSTA) {
        if (hasStaCredentials) {
            _wifiSTA->begin(_deviceConfig.wifi.sta_ssid, _deviceConfig.wifi.sta_password);
        } else {
            _wifiSTA->begin("", ""); // Won't connect
        }
    }
}

void MainApplication::initWebServer() {
    // Create web server
    _wifiWebServer = new WebServer();
    
    // Inject DataStorage
    if (_dataStorage) {
        _wifiWebServer->setDataStorage(_dataStorage);
    }
    
    // Inject ConfigManager
    if (_configManager) {
        _wifiWebServer->setConfigManager(_configManager);
    }
    
    // Set up callbacks
    if (_wifiWebServer) {
        _wifiWebServer->setCallbacks(
            &MainApplication::onSaveWifiSta,
            &MainApplication::onSaveWifiAp,
            &MainApplication::onForgetWifi
        );
        _wifiWebServer->setScanCallback(&MainApplication::scanNetworksCallback);
    }
    
    // Start web server
    if (_wifiWebServer) {
        _wifiWebServer->begin();
    }
}

void MainApplication::initDisplay() {
    // Initialize display manager with TCA9548A channel 0
    _displayManager = new DisplayManager(0x3C, I2C_SDA_PIN, I2C_SCL_PIN);
    _displayManager->begin();
}

void MainApplication::initLED() {
    // Initialize external LED controller
    _ledController = new LEDController(LED_DATA_PIN);
    if (_dataStorage) {
        _ledController->setDataStorage(_dataStorage);
    }
    _ledController->begin();
    _ledController->setPattern(_deviceConfig.led);
    
    // Initialize built-in LED
    _builtInLED = new BuiltInLED();
    if (_dataStorage) {
        _builtInLED->setDataStorage(_dataStorage);
    }
    _builtInLED->begin();
    // Initial pattern will be set in appLoop based on WiFi status
}

void MainApplication::connectLEDControllerToWiFi() {
    // No longer needed - dependencies are injected directly
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
    if (_configManager && _dataStorage) {
        // Update the configuration
        strncpy(_deviceConfig.wifi.sta_ssid, ssid, sizeof(_deviceConfig.wifi.sta_ssid) - 1);
        _deviceConfig.wifi.sta_ssid[sizeof(_deviceConfig.wifi.sta_ssid) - 1] = '\0';
        
        strncpy(_deviceConfig.wifi.sta_password, password, sizeof(_deviceConfig.wifi.sta_password) - 1);
        _deviceConfig.wifi.sta_password[sizeof(_deviceConfig.wifi.sta_password) - 1] = '\0';
        
        // Save to EEPROM
        //_configManager->saveConfig(_deviceConfig);
        
        // Store credentials in DataStorage and request connection
        _dataStorage->setStaCredentials(ssid, password);
        _dataStorage->requestStaConnection();
        
        Serial.println("MainApplication: WiFi STA credentials stored and connection requested");
    }
}

void MainApplication::saveWifiApConfig(const char* ssid, const char* password, const char* ip) {
    if (_configManager) {
        // Update the configuration
        strncpy(_deviceConfig.wifi.ap_ssid, ssid, sizeof(_deviceConfig.wifi.ap_ssid) - 1);
        _deviceConfig.wifi.ap_ssid[sizeof(_deviceConfig.wifi.ap_ssid) - 1] = '\0';
        
        strncpy(_deviceConfig.wifi.ap_password, password, sizeof(_deviceConfig.wifi.ap_password) - 1);
        _deviceConfig.wifi.ap_password[sizeof(_deviceConfig.wifi.ap_password) - 1] = '\0';
        
        // Update IP if provided
        if (ip && strlen(ip) > 0) {
            strncpy(_deviceConfig.wifi.ap_ip, ip, sizeof(_deviceConfig.wifi.ap_ip) - 1);
            _deviceConfig.wifi.ap_ip[sizeof(_deviceConfig.wifi.ap_ip) - 1] = '\0';
        }
        
        // Save to EEPROM
        //_configManager->saveConfig(_deviceConfig);
        
        // Reinitialize AP with new settings
        if (_wifiAP) {
            _wifiAP->begin(_deviceConfig.wifi.ap_ssid, _deviceConfig.wifi.ap_password, _deviceConfig.wifi.ap_ip);
        }
    }
}

void MainApplication::forgetWifiConfig() {
    if (_configManager && _dataStorage) {
        // Clear the STA configuration
        strcpy(_deviceConfig.wifi.sta_ssid, "");
        strcpy(_deviceConfig.wifi.sta_password, "");
        
        // Save to EEPROM
        //_configManager->saveConfig(_deviceConfig);
        
        // Clear credentials from DataStorage
        _dataStorage->setStaCredentials("", "");
        
        // Disconnect STA
        if (_wifiSTA) {
            _wifiSTA->disconnect();
        }
    }
}

// Static callback implementations
void MainApplication::onSaveWifiSta(const char* ssid, const char* password) {
    if (appInstance) {
        appInstance->saveWifiStaConfig(ssid, password);
    }
}

void MainApplication::onSaveWifiAp(const char* ssid, const char* password, const char* ip) {
    if (appInstance) {
        appInstance->saveWifiApConfig(ssid, password, ip);
    }
}

void MainApplication::onForgetWifi() {
    if (appInstance) {
        appInstance->forgetWifiConfig();
    }
}

void MainApplication::saveLedConfig(const PatternConfig& config) {
    if (_configManager) {
        // Update the configuration
        _deviceConfig.led = config;
        
        // Save to EEPROM
        //_configManager->saveConfig(_deviceConfig);
        
        // Update the LED controller
        if (_ledController) {
            _ledController->setPattern(_deviceConfig.led);
        }
    }
}

String MainApplication::getWifiScanJson() {
    if (!_wifiSTA) {
        return "[]";
    }
    auto networks = _wifiSTA->scanNetworks();
    String response = "[";
    for (size_t i = 0; i < networks.size(); ++i) {
        if (i > 0) {
            response += ",";
        }
        const auto& net = networks[i];
        response += "{\"ssid\":\"" + net.ssid + "\",\"rssi\":" + String(net.rssi) + ",\"secure\":" + (net.secure ? "true" : "false") + "}";
    }
    response += "]";
    return response;
}

String MainApplication::scanNetworksCallback() {
    if (appInstance) {
        return appInstance->getWifiScanJson();
    }
    return "[]";
}
