#include "main.h"
#include "modules/led/LEDController.h"
#include "modules/builtin_led/BuiltInLED.h"
#include "modules/wifi/WiFiAP.h"
#include "modules/wifi/WiFiSTA.h"
#include "modules/webserver/WebServer.h"
#include "modules/data_storage/DataStorage.h"
#include "modules/config/ConfigManager.h"
#include <functional>

// LED pattern names for web interface
static const char* LED_PATTERN_NAMES[] = {
    "running_light",
    "ping_pong",
    "rainbow_wave",
    "chase",
    "blink"
};
static const int LED_PATTERN_COUNT = 5;

MainApplication::MainApplication()
    : _ledController(nullptr), _displayManager(nullptr),
      _configManager(nullptr), _dataStorage(nullptr),
      _builtInLED(nullptr), _wifiAP(nullptr), _wifiSTA(nullptr), _wifiWebServer(nullptr), _geolocation(nullptr), _weather(nullptr) {
}

void MainApplication::begin() {
    // Initialize serial communication first
    Serial.begin(115200);
    delay(500); // Brief delay to allow serial to stabilize
    
    Serial.println("Initializing Table Clock Application...");
    
    initConfig();
    delay(50); // Small delay after config init
    
    initHardware();
    delay(500); // Small delay after hardware init


    
    initLED();
    delay(20); // Small delay after LED init

    initWiFiAP();
    initWiFiSTA();
    initGeolocation();
    initWeather();
    initWebServer();
    delay(20); // Combined delay was 200ms at end of old initWiFi()
    
    initDisplay();
    delay(20); // Small delay after display init

    
    //connectLEDControllerToWiFi();
    
    Serial.println("Table Clock Application started successfully.");
}

void MainApplication::appLoop() {
    // Update WiFi STA (monitor connection status)
    if (_wifiSTA) {
        _wifiSTA->update();
    }
    
    // Update WiFi AP (refresh AP info in DataStorage)
    if (_wifiAP) {
        _wifiAP->update();
    }
    
    // Check for WiFi connection to trigger geolocation update
    static bool lastWifiConnected = false;
    bool wifiConnected = _wifiSTA ? _wifiSTA->isConnected() : false;
    if (wifiConnected && !lastWifiConnected) {
        // WiFi just connected, trigger geolocation update
        if (_geolocation) {
            _geolocation->forceUpdate();
        }
    }
    lastWifiConnected = wifiConnected;
    
    // Update geolocation (periodic fetch)
    if (_geolocation) {
        _geolocation->update();
    }
    
    // Check for new geolocation to trigger weather update
    static float lastLat = 0.0f;
    static float lastLon = 0.0f;
    if (_dataStorage) {
        auto& data = _dataStorage->getData();
        if (data.geo_last_update > 0 && (data.latitude != lastLat || data.longitude != lastLon)) {
            // New coordinates obtained, trigger weather update
            lastLat = data.latitude;
            lastLon = data.longitude;
            if (_weather) {
                _weather->forceUpdate();
            }
        }
    }
    
    // Update weather (periodic fetch)
    if (_weather) {
        _weather->update();
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
    
    // Update one display at a time for better performance (round-robin)
    static uint8_t currentDisplayIndex = 0;
    
    if (_pageManager && _allDisplays[currentDisplayIndex]) {
        _pageManager->updateSingleDisplay(_allDisplays[currentDisplayIndex], currentDisplayIndex);
    }
    
    // Move to next display in the next cycle
    currentDisplayIndex = (currentDisplayIndex + 1) % 8;
    
    // Removed counter functionality - now handled by page system
    
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
    
    // Inject LEDController
    if (_ledController) {
        _wifiWebServer->setLEDController(_ledController);
    }
    
    // Set up callbacks
    if (_wifiWebServer) {
        _wifiWebServer->setCallbacks(
            &MainApplication::onSaveWifiSta,
            &MainApplication::onSaveWifiAp,
            &MainApplication::onForgetWifi
        );
        _wifiWebServer->setSaveConfigCallback(&MainApplication::onSaveConfig);
        _wifiWebServer->setScanCallback(&MainApplication::scanNetworksCallback);
        _wifiWebServer->setGetLEDPatternsCallback(&MainApplication::onGetLEDPatterns);
        _wifiWebServer->setGetLEDCurrentCallback(&MainApplication::onGetLEDCurrent);
        _wifiWebServer->setApplyLEDSettingsCallback(&MainApplication::onApplyLEDSettings);
    }
    
    // Start web server
    if (_wifiWebServer) {
        _wifiWebServer->begin();
    }
}

void MainApplication::initDisplay() {
    // Initialize all 8 displays connected to TCA9548A channels 0-7
    for (int i = 0; i < 8; i++) {
        _allDisplays[i] = new DisplayManager(0x70, i, I2C_SDA_PIN, I2C_SCL_PIN);
        // Inject DataStorage dependency
        if (_dataStorage) {
            _allDisplays[i]->setDataStorage(_dataStorage);
        }
        delay(50);
        _allDisplays[i]->begin();
        delay(50);
    }
    
    // Also keep the first display in the original variable for backward compatibility
    _displayManager = _allDisplays[0];
    
    // Create a single PageManager to control all displays
    if (_dataStorage) {
        _pageManager = new PageManager(_dataStorage);
    }
    
    // Counter functionality removed - now using page system
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

void MainApplication::initGeolocation() {
    // Create and initialize Geolocation module
    _geolocation = new Geolocation(_dataStorage);
    
    if (_geolocation) {
        _geolocation->begin();
        Serial.println("Geolocation module initialized");
    }
}

void MainApplication::initWeather() {
    // Create and initialize Weather module
    _weather = new Weather(_dataStorage);
    
    if (_weather) {
        _weather->begin();
        Serial.println("Weather module initialized");
    }
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

void MainApplication::onSaveConfig() {
    if (appInstance) {
        appInstance->saveConfig();
    }
}

void MainApplication::saveConfig() {
    if (_configManager) {
        _configManager->saveConfig(_deviceConfig);
        Serial.println("Configuration saved to EEPROM");
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

String MainApplication::getLEDPatternsJson() {
    String response = "[";
    for (int i = 0; i < LED_PATTERN_COUNT; i++) {
        if (i > 0) {
            response += ",";
        }
        response += "\"" + String(LED_PATTERN_NAMES[i]) + "\"";
    }
    response += "]";
    return response;
}

String MainApplication::getLEDCurrentJson() {
    if (!_ledController) {
        return "{}";
    }
    PatternConfig config = _ledController->getCurrentConfig();
    
    // Convert pattern enum to string
    String patternStr;
    switch (config.pattern) {
        case LEDPattern::RUNNING_LIGHT: patternStr = "running_light"; break;
        case LEDPattern::PING_PONG: patternStr = "ping_pong"; break;
        case LEDPattern::RAINBOW_WAVE: patternStr = "rainbow_wave"; break;
        case LEDPattern::CHASE: patternStr = "chase"; break;
        case LEDPattern::BLINK: patternStr = "blink"; break;
        default: patternStr = "running_light"; break;
    }
    
    // Convert color to string (simple approach - use hex)
    String colorStr = String(config.color.R) + "," + String(config.color.G) + "," + String(config.color.B);
    
    // Convert direction to string
    String dirStr = config.direction ? "forward" : "reverse";
    
    String response = "{";
    response += "\"pattern\":\"" + patternStr + "\",";
    response += "\"color\":\"" + colorStr + "\",";
    response += "\"speed\":" + String(config.speed) + ",";
    response += "\"direction\":\"" + dirStr + "\"";
    response += "}";
    
    return response;
}

void MainApplication::applyLEDSettings(const char* pattern, const char* color, uint16_t speed, bool direction) {
    if (!_ledController || !_configManager) {
        return;
    }
    
    PatternConfig config;
    
    // Parse pattern
    String patternStr = String(pattern);
    if (patternStr == "running_light") {
        config.pattern = LEDPattern::RUNNING_LIGHT;
    } else if (patternStr == "ping_pong") {
        config.pattern = LEDPattern::PING_PONG;
    } else if (patternStr == "rainbow_wave") {
        config.pattern = LEDPattern::RAINBOW_WAVE;
    } else if (patternStr == "chase") {
        config.pattern = LEDPattern::CHASE;
    } else if (patternStr == "blink") {
        config.pattern = LEDPattern::BLINK;
    } else {
        config.pattern = LEDPattern::RUNNING_LIGHT; // default
    }
    
    // Parse color
    int r = 20, g = 20, b = 20; // default white
    String colorStr = String(color);
    int firstComma = colorStr.indexOf(',');
    int secondComma = colorStr.indexOf(',', firstComma + 1);
    if (firstComma > 0 && secondComma > firstComma) {
        r = colorStr.substring(0, firstComma).toInt();
        g = colorStr.substring(firstComma + 1, secondComma).toInt();
        b = colorStr.substring(secondComma + 1).toInt();
    }
    config.color = RgbColor(r, g, b);
    
    config.speed = speed;
    config.direction = direction;
    
    // Update LED controller
    _ledController->setPattern(config);
    
    // Update device configuration
    _deviceConfig.led = config;
    
    // Save to EEPROM (optional, can be done separately)
    // _configManager->saveConfig(_deviceConfig);
}

String MainApplication::onGetLEDPatterns() {
    if (appInstance) {
        return appInstance->getLEDPatternsJson();
    }
    return "[]";
}

String MainApplication::onGetLEDCurrent() {
    if (appInstance) {
        return appInstance->getLEDCurrentJson();
    }
    return "{}";
}

void MainApplication::onApplyLEDSettings(const char* pattern, const char* color, uint16_t speed, bool direction) {
    if (appInstance) {
        appInstance->applyLEDSettings(pattern, color, speed, direction);
    }
}
