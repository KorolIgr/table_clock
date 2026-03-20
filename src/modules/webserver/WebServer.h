#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include <ESP8266WebServer.h>
#include <LittleFS.h>
#include "../data_storage/DataStorage.h"

// Forward declarations
class ConfigManager;
class LEDController;

typedef void (*SaveWifiStaCallback)(const char* ssid, const char* password);
typedef void (*SaveWifiApCallback)(const char* ssid, const char* password, const char* ip);
typedef void (*ForgetWifiCallback)();
typedef void (*SaveConfigCallback)();
typedef String (*ScanNetworksCallback)();
typedef String (*GetLEDPatternsCallback)();
typedef String (*GetLEDCurrentCallback)();
typedef void (*ApplyLEDSettingsCallback)(const char* pattern, const char* color, uint16_t speed, bool direction);

class WebServer {
public:
    WebServer();
    
    void setDataStorage(DataStorage* dataStorage);
    void setConfigManager(ConfigManager* configManager);
    void setLEDController(LEDController* ledController);
    void setCallbacks(SaveWifiStaCallback saveStaCb, SaveWifiApCallback saveApCb, ForgetWifiCallback forgetCb);
    void setSaveConfigCallback(SaveConfigCallback saveConfigCb);
    void setScanCallback(ScanNetworksCallback scanCb);
    void setGetLEDPatternsCallback(GetLEDPatternsCallback getPatternsCb);
    void setGetLEDCurrentCallback(GetLEDCurrentCallback getCurrentCb);
    void setApplyLEDSettingsCallback(ApplyLEDSettingsCallback applySettingsCb);
    
    void begin();
    void update();
    
private:
    DataStorage* _dataStorage;
    ConfigManager* _configManager;
    LEDController* _ledController;
    
    ESP8266WebServer* _server;
    
    SaveWifiStaCallback _saveStaCallback;
    SaveWifiApCallback _saveApCallback;
    ForgetWifiCallback _forgetCallback;
    SaveConfigCallback _saveConfigCallback;
    ScanNetworksCallback _scanCallback;
    GetLEDPatternsCallback _getLEDPatternsCallback;
    GetLEDCurrentCallback _getLEDCurrentCallback;
    ApplyLEDSettingsCallback _applyLEDSettingsCallback;
    
    void handleNotFound();
    void handleWifiSTAScan();
    void handleWifiSTAApply();
    void handleWifiSTAForget();
    void handleWifiApApply();
    void handleWifiApForm(); // For POST /wifi_ap form with ip
    void handleSaveConfig();
    void handleStatus();
    void handleGetLEDPatterns();
    void handleGetLEDCurrent();
    void handleApplyLEDSettings();
};

#endif // WEB_SERVER_H
