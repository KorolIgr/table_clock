#include "InitializationMediator.h"

InitializationMediator::InitializationMediator(ConfigManager* configManager, DataStorage* dataStorage,
                                               DisplayManager** allDisplays,
                                               LEDController* ledController, BuiltInLED* builtInLED,
                                               WiFiAP* wifiAP, WiFiSTA* wifiSTA, WebServer* wifiWebServer,
                                               Geolocation* geolocation, Weather* weather,
                                               AirQuality* airQuality, LoRa* lora)
    : _configManager(configManager)
    , _dataStorage(dataStorage)
    , _allDisplays(allDisplays)
    , _ledController(ledController)
    , _builtInLED(builtInLED)
    , _wifiAP(wifiAP)
    , _wifiSTA(wifiSTA)
    , _wifiWebServer(wifiWebServer)
    , _geolocation(geolocation)
    , _weather(weather)
    , _airQuality(airQuality)
    , _lora(lora) {
}

void InitializationMediator::initConfig(DeviceConfig& deviceConfig) {
    if (_configManager && _dataStorage) {
        _configManager->setDataStorage(_dataStorage);
        
        if (_configManager->loadConfig(deviceConfig)) {
            Serial.println("InitializationMediator: Configuration loaded successfully from EEPROM");
        } else {
            Serial.println("InitializationMediator: Using default configuration");
            deviceConfig = _configManager->getDefaultConfig();
        }
    }
}

void InitializationMediator::initHardware() {
    Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
    Wire.setClock(10000);
    delay(50);
}

void InitializationMediator::initWiFiAP(DeviceConfig& deviceConfig) {
    if (_wifiAP && _dataStorage) {
        _wifiAP->setDataStorage(_dataStorage);
        _wifiAP->begin(deviceConfig.wifi.ap_ssid, deviceConfig.wifi.ap_password, deviceConfig.wifi.ap_ip);
    }
}

void InitializationMediator::initWiFiSTA(DeviceConfig& deviceConfig) {
    if (_wifiSTA && _dataStorage) {
        _wifiSTA->setDataStorage(_dataStorage);
        
        bool hasStaCredentials = strlen(deviceConfig.wifi.sta_ssid) > 0 && strlen(deviceConfig.wifi.sta_password) >= 8;
        if (hasStaCredentials) {
            _wifiSTA->begin(deviceConfig.wifi.sta_ssid, deviceConfig.wifi.sta_password);
        } else {
            _wifiSTA->begin("", "");
        }
    }
}

void InitializationMediator::initWebServer() {
    if (_wifiWebServer && _dataStorage) {
        _wifiWebServer->setDataStorage(_dataStorage);
    }
    if (_wifiWebServer && _configManager) {
        _wifiWebServer->setConfigManager(_configManager);
    }
    if (_wifiWebServer && _ledController) {
        _wifiWebServer->setLEDController(_ledController);
    }
    if (_wifiWebServer) {
        _wifiWebServer->begin();
    }
}

void InitializationMediator::initDisplay() {
    for (int i = 0; i < 8; i++) {
        _allDisplays[i] = new DisplayManager(0x70, i, I2C_SDA_PIN, I2C_SCL_PIN);
        if (_dataStorage) {
            _allDisplays[i]->setDataStorage(_dataStorage);
        }
        delay(50);
        _allDisplays[i]->begin();
        delay(50);
    }
}

void InitializationMediator::initLED(DeviceConfig& deviceConfig) {
    if (_ledController && _dataStorage) {
        _ledController->setDataStorage(_dataStorage);
        _ledController->begin();
        _ledController->setPattern(deviceConfig.led);
    }
    
    if (_builtInLED && _dataStorage) {
        _builtInLED->setDataStorage(_dataStorage);
        _builtInLED->begin();
    }
}

void InitializationMediator::initGeolocation() {
    if (_geolocation && _dataStorage) {
        _geolocation->begin();
        Serial.println("InitializationMediator: Geolocation module initialized");
    }
}

void InitializationMediator::initWeather() {
    if (_weather && _dataStorage) {
        _weather->begin();
        Serial.println("InitializationMediator: Weather module initialized");
    }
}

void InitializationMediator::initAirQuality() {
    if (_airQuality && _dataStorage) {
        _airQuality->begin(I2C_SDA_PIN, I2C_SCL_PIN);
        Serial.println("InitializationMediator: AirQuality sensor initialized");
    }
}

void InitializationMediator::initLoRa() {
    if (_lora && _dataStorage) {
        _lora->setDataStorage(_dataStorage);
        _lora->begin();
        Serial.println("InitializationMediator: LoRa module initialized");
    }
}
