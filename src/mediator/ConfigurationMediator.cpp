#include "ConfigurationMediator.h"

// Static instance pointer
ConfigurationMediator* ConfigurationMediator::instance = nullptr;

ConfigurationMediator::ConfigurationMediator(ConfigManager* configManager, DataStorage* dataStorage,
                                             LEDController* ledController, WiFiAP* wifiAP, WiFiSTA* wifiSTA,
                                             DeviceConfig* deviceConfig)
    : _configManager(configManager)
    , _dataStorage(dataStorage)
    , _ledController(ledController)
    , _wifiAP(wifiAP)
    , _wifiSTA(wifiSTA)
    , _deviceConfig(deviceConfig) {
    instance = this;
}

void ConfigurationMediator::saveWifiStaConfig(const char* ssid, const char* password) {
    if (_configManager && _dataStorage && _deviceConfig) {
        // Update device configuration
        strncpy(_deviceConfig->wifi.sta_ssid, ssid, sizeof(_deviceConfig->wifi.sta_ssid) - 1);
        _deviceConfig->wifi.sta_ssid[sizeof(_deviceConfig->wifi.sta_ssid) - 1] = '\0';
        
        strncpy(_deviceConfig->wifi.sta_password, password, sizeof(_deviceConfig->wifi.sta_password) - 1);
        _deviceConfig->wifi.sta_password[sizeof(_deviceConfig->wifi.sta_password) - 1] = '\0';
        
        // Store credentials in DataStorage and request connection
        _dataStorage->setStaCredentials(ssid, password);
        _dataStorage->requestStaConnection();
        
        Serial.println("ConfigurationMediator: WiFi STA credentials stored and connection requested");
    }
}

void ConfigurationMediator::saveWifiApConfig(const char* ssid, const char* password, const char* ip) {
    if (_configManager && _wifiAP && _deviceConfig) {
        // Update device configuration
        strncpy(_deviceConfig->wifi.ap_ssid, ssid, sizeof(_deviceConfig->wifi.ap_ssid) - 1);
        _deviceConfig->wifi.ap_ssid[sizeof(_deviceConfig->wifi.ap_ssid) - 1] = '\0';
        
        strncpy(_deviceConfig->wifi.ap_password, password, sizeof(_deviceConfig->wifi.ap_password) - 1);
        _deviceConfig->wifi.ap_password[sizeof(_deviceConfig->wifi.ap_password) - 1] = '\0';
        
        if (ip && strlen(ip) > 0) {
            strncpy(_deviceConfig->wifi.ap_ip, ip, sizeof(_deviceConfig->wifi.ap_ip) - 1);
            _deviceConfig->wifi.ap_ip[sizeof(_deviceConfig->wifi.ap_ip) - 1] = '\0';
        }
        
        // Reinitialize AP with new settings
        _wifiAP->begin(ssid, password, ip);
    }
}

void ConfigurationMediator::forgetWifiConfig() {
    if (_dataStorage && _wifiSTA && _deviceConfig) {
        // Clear the STA configuration
        strcpy(_deviceConfig->wifi.sta_ssid, "");
        strcpy(_deviceConfig->wifi.sta_password, "");
        
        // Clear credentials from DataStorage
        _dataStorage->setStaCredentials("", "");
        
        // Disconnect STA
        _wifiSTA->disconnect();
    }
}

void ConfigurationMediator::saveConfig() {
    if (_configManager && _deviceConfig) {
        _configManager->saveConfig(*_deviceConfig);
        Serial.println("ConfigurationMediator: Configuration saved to EEPROM");
    }
}

void ConfigurationMediator::saveLedConfig(const PatternConfig& config) {
    if (_ledController && _deviceConfig) {
        _deviceConfig->led = config;
        _ledController->setPattern(config);
    }
}

// Static callback implementations
void ConfigurationMediator::staticSaveWifiSta(const char* ssid, const char* password) {
    if (instance) {
        instance->saveWifiStaConfig(ssid, password);
    }
}

void ConfigurationMediator::staticSaveWifiAp(const char* ssid, const char* password, const char* ip) {
    if (instance) {
        instance->saveWifiApConfig(ssid, password, ip);
    }
}

void ConfigurationMediator::staticForgetWifi() {
    if (instance) {
        instance->forgetWifiConfig();
    }
}

void ConfigurationMediator::staticSaveConfig() {
    if (instance) {
        instance->saveConfig();
    }
}
