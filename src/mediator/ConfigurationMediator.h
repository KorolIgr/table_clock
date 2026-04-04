#ifndef CONFIGURATION_MEDIATOR_H
#define CONFIGURATION_MEDIATOR_H

#include <Arduino.h>
#include "modules/config/ConfigManager.h"
#include "modules/data_storage/DataStorage.h"
#include "modules/led_line/LEDController.h"
#include "modules/wifi_ap/WiFiAP.h"
#include "modules/wifi_sta/WiFiSTA.h"
#include "app_config.h"

// Callback type definitions
typedef void (*SaveWifiStaCallback)(const char* ssid, const char* password);
typedef void (*SaveWifiApCallback)(const char* ssid, const char* password, const char* ip);
typedef void (*ForgetWifiCallback)();
typedef void (*SaveConfigCallback)();

class ConfigurationMediator {
public:
    ConfigurationMediator(ConfigManager* configManager, DataStorage* dataStorage,
                         LEDController* ledController, WiFiAP* wifiAP, WiFiSTA* wifiSTA,
                         DeviceConfig* deviceConfig);
    
    // Instance methods
    void saveWifiStaConfig(const char* ssid, const char* password);
    void saveWifiApConfig(const char* ssid, const char* password, const char* ip);
    void forgetWifiConfig();
    void saveConfig();
    void saveLedConfig(const PatternConfig& config);
    
    // Callback getters
    SaveWifiStaCallback getSaveWifiStaCallback() const { return &staticSaveWifiSta; }
    SaveWifiApCallback getSaveWifiApCallback() const { return &staticSaveWifiAp; }
    ForgetWifiCallback getForgetWifiCallback() const { return &staticForgetWifi; }
    SaveConfigCallback getSaveConfigCallback() const { return &staticSaveConfig; }
    
    ConfigManager* getConfigManager() const { return _configManager; }
    DataStorage* getDataStorage() const { return _dataStorage; }
    LEDController* getLEDController() const { return _ledController; }
    WiFiAP* getWiFiAP() const { return _wifiAP; }
    WiFiSTA* getWiFiSTA() const { return _wifiSTA; }
    DeviceConfig* getDeviceConfig() const { return _deviceConfig; }

private:
    ConfigManager* _configManager;
    DataStorage* _dataStorage;
    LEDController* _ledController;
    WiFiAP* _wifiAP;
    WiFiSTA* _wifiSTA;
    DeviceConfig* _deviceConfig;
    
    // Static callback wrappers
    static void staticSaveWifiSta(const char* ssid, const char* password);
    static void staticSaveWifiAp(const char* ssid, const char* password, const char* ip);
    static void staticForgetWifi();
    static void staticSaveConfig();
    
    static ConfigurationMediator* instance;
};

#endif // CONFIGURATION_MEDIATOR_H
