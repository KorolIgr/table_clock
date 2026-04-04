#define TASK_SCHEDULER_IMPLEMENTATION
#include <TaskScheduler.h>
#include "MainApplication.h"

MainApplication::MainApplication()
    : _ledController(nullptr)
    , _pageManager(nullptr)
    , _configManager(nullptr)
    , _dataStorage(nullptr)
    , _builtInLED(nullptr)
    , _wifiAP(nullptr)
    , _wifiSTA(nullptr)
    , _wifiWebServer(nullptr)
    , _geolocation(nullptr)
    , _weather(nullptr)
    , _airQuality(nullptr)
    , _lora(nullptr)
    , _configMediator(nullptr)
    , _webCallbackMediator(nullptr)
    , _initMediator(nullptr)
    , _scheduler(nullptr)
    , tLoRaSend(nullptr)
    , tHeartbeat(nullptr)
    , tPageRotation(nullptr)
    , tAirQualityUpdate(nullptr)
    , tGeolocationUpdate(nullptr)
    , tWeatherUpdate(nullptr)
    , tBuiltInLEDUpdate(nullptr) {
    
    for (int i = 0; i < 8; i++) {
        _allDisplays[i] = nullptr;
    }
}

void MainApplication::begin() {
    Serial.begin(115200);
    delay(500);
    
    Serial.println("Initializing Table Clock Application...");
    
    initApp();
    
    Serial.println("Table Clock Application started successfully.");
}

void MainApplication::initApp() {
    // Create modules
    _dataStorage = new DataStorage();
    _configManager = new ConfigManager();
    _ledController = new LEDController(LED_DATA_PIN);
    _builtInLED = new BuiltInLED();
    _wifiAP = new WiFiAP();
    _wifiSTA = new WiFiSTA();
    _wifiWebServer = new WebServer();
    _geolocation = new Geolocation(_dataStorage);
    _weather = new Weather(_dataStorage);
    _airQuality = new AirQuality(_dataStorage);
    _lora = new LoRa();
    
    // Create mediators
    _configMediator = new ConfigurationMediator(_configManager, _dataStorage, _ledController, _wifiAP, _wifiSTA, &_deviceConfig);
    _webCallbackMediator = new WebCallbackMediator(_wifiSTA, _ledController);
    _initMediator = new InitializationMediator(_configManager, _dataStorage, _allDisplays,
                                                _ledController, _builtInLED, _wifiAP, _wifiSTA, _wifiWebServer,
                                                _geolocation, _weather, _airQuality, _lora);
    
    // Initialize everything through mediators
    _initMediator->initConfig(_deviceConfig);
    delay(50);
    
    _initMediator->initHardware();
    delay(50);
    
    _initMediator->initWiFiAP(_deviceConfig);
    _initMediator->initWiFiSTA(_deviceConfig);
    
    _initMediator->initGeolocation();
    _initMediator->initWeather();
    _initMediator->initAirQuality();
    _initMediator->initLoRa();
    
    _initMediator->initLED(_deviceConfig);
    _initMediator->initDisplay();
    // Create PageManager after displays are initialized
    if (_dataStorage) {
        _pageManager = new PageManager(_dataStorage);
    }
    
    _initMediator->initWebServer();
    
    // Set up web server callbacks
    setWebServerCallbacks();
    
    // Create scheduler
    _scheduler = new Scheduler();
    
    // Create tasks
    tLoRaSend = new Task(10000, -1, &staticLoRaSend, _scheduler, true, nullptr, nullptr);
    tHeartbeat = new Task(5000, -1, &staticHeartbeat, _scheduler, true, nullptr, nullptr);
    tPageRotation = new Task(1000, -1, &staticPageRotation, _scheduler, true, nullptr, nullptr);
    tAirQualityUpdate = new Task(1000, -1, &staticAirQualityUpdate, _scheduler, true, nullptr, nullptr);
    tGeolocationUpdate = new Task(300000, -1, &staticGeolocationUpdate, _scheduler, true, nullptr, nullptr);
    tWeatherUpdate = new Task(300000, -1, &staticWeatherUpdate, _scheduler, true, nullptr, nullptr);
    tBuiltInLEDUpdate = new Task(250, -1, &staticBuiltInLEDUpdate, _scheduler, true, nullptr, nullptr);
}

void MainApplication::appLoop() {
    // Update WiFi STA
    if (_wifiSTA) {
        _wifiSTA->update();
    }
    
    // Update WiFi AP
    if (_wifiAP) {
        _wifiAP->update();
    }
    
    // Check for WiFi connection to trigger geolocation update
    static bool lastWifiConnected = false;
    bool wifiConnected = _wifiSTA ? _wifiSTA->isConnected() : false;
    if (wifiConnected && !lastWifiConnected) {
        if (_geolocation) {
            _geolocation->forceUpdate();
        }
    }
    lastWifiConnected = wifiConnected;
    
    // Check for new geolocation to trigger weather update
    static float lastLat = 0.0f;
    static float lastLon = 0.0f;
    if (_dataStorage) {
        GeoData& geo = _dataStorage->geo();
        if (geo.last_update > 0 && (geo.latitude != lastLat || geo.longitude != lastLon)) {
            lastLat = geo.latitude;
            lastLon = geo.longitude;
            if (_weather) {
                _weather->forceUpdate();
            }
        }
    }
    
    // Update web server
    if (_wifiWebServer) {
        _wifiWebServer->update();
    }
    
    // Update built-in LED based on WiFi status
    if (_builtInLED && _wifiSTA) {
        bool connected = _wifiSTA->isConnected();
        static bool lastConnected = !connected;
        if (connected != lastConnected) {
            lastConnected = connected;
            if (connected) {
                _builtInLED->setPattern(3);
            } else {
                _builtInLED->setPattern(1);
            }
        }
    }
    
    // Update external LED controller
    if (_ledController) {
        _ledController->updatePattern();
    }
    
    // Update displays round-robin
    static uint8_t currentDisplayIndex = 0;
    if (_pageManager && _allDisplays[currentDisplayIndex]) {
        _pageManager->updateSingleDisplay(_allDisplays[currentDisplayIndex], currentDisplayIndex);
    }
    currentDisplayIndex = (currentDisplayIndex + 1) % 8;
    
    // Execute scheduled tasks
    _scheduler->execute();
}

void MainApplication::setWebServerCallbacks() {
    if (_wifiWebServer && _configMediator && _webCallbackMediator) {
        _wifiWebServer->setCallbacks(
            _configMediator->getSaveWifiStaCallback(),
            _configMediator->getSaveWifiApCallback(),
            _configMediator->getForgetWifiCallback()
        );
        _wifiWebServer->setSaveConfigCallback(_configMediator->getSaveConfigCallback());
        _wifiWebServer->setScanCallback(_webCallbackMediator->getScanCallback());
        _wifiWebServer->setGetLEDPatternsCallback(_webCallbackMediator->getLEDPatternsCallback());
        _wifiWebServer->setGetLEDCurrentCallback(_webCallbackMediator->getLEDCurrentCallback());
        _wifiWebServer->setApplyLEDSettingsCallback(_webCallbackMediator->getApplyLEDSettingsCallback());
    }
}

// Configuration methods - delegate to ConfigurationMediator
void MainApplication::saveWifiStaConfig(const char* ssid, const char* password) {
    _configMediator->saveWifiStaConfig(ssid, password);
}

void MainApplication::saveWifiApConfig(const char* ssid, const char* password, const char* ip) {
    _configMediator->saveWifiApConfig(ssid, password, ip);
}

void MainApplication::forgetWifiConfig() {
    _configMediator->forgetWifiConfig();
}

void MainApplication::saveConfig() {
    _configMediator->saveConfig();
}

void MainApplication::saveLedConfig(const PatternConfig& config) {
    _configMediator->saveLedConfig(config);
}

// Web callback methods - delegate to WebCallbackMediator
String MainApplication::getWifiScanJson() {
    return _webCallbackMediator->getWifiScanJson();
}

String MainApplication::getLEDPatternsJson() {
    return _webCallbackMediator->getLEDPatternsJson();
}

String MainApplication::getLEDCurrentJson() {
    return _webCallbackMediator->getLEDCurrentJson();
}

void MainApplication::applyLEDSettings(const char* pattern, const char* color, uint16_t speed, bool direction) {
    _webCallbackMediator->applyLEDSettings(pattern, color, speed, direction);
}

// Static task callback implementations
void MainApplication::staticLoRaSend() {
    if (appInstance && appInstance->_lora && appInstance->_lora->isInitialized() && appInstance->_dataStorage) {
        AirQualityData& aq = appInstance->_dataStorage->airQuality();
        if (aq.valid) {
            appInstance->_lora->sendTVOCData(aq.tvoc, millis());
        }
    }
}

void MainApplication::staticHeartbeat() {
    Serial.print("Device running... Heartbeat: ");
    Serial.println(millis());
}

void MainApplication::staticPageRotation() {
    if (appInstance && appInstance->_pageManager) {
        appInstance->_pageManager->updateRotation();
    }
}

void MainApplication::staticAirQualityUpdate() {
    if (appInstance && appInstance->_airQuality) {
        appInstance->_airQuality->update();
    }
}

void MainApplication::staticGeolocationUpdate() {
    if (appInstance && appInstance->_geolocation) {
        appInstance->_geolocation->update();
    }
}

void MainApplication::staticWeatherUpdate() {
    if (appInstance && appInstance->_weather) {
        appInstance->_weather->update();
    }
}

void MainApplication::staticBuiltInLEDUpdate() {
    if (appInstance && appInstance->_builtInLED) {
        appInstance->_builtInLED->update();
    }
}
