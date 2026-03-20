#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include <ESP8266WebServer.h>
#include <LittleFS.h>
#include "../data_storage/DataStorage.h"

// Forward declarations
class ConfigManager;

typedef void (*SaveWifiStaCallback)(const char* ssid, const char* password);
typedef void (*SaveWifiApCallback)(const char* ssid, const char* password, const char* ip);
typedef void (*ForgetWifiCallback)();
typedef void (*SaveConfigCallback)();
typedef String (*ScanNetworksCallback)();

class WebServer {
public:
    WebServer();
    
    void setDataStorage(DataStorage* dataStorage);
    void setConfigManager(ConfigManager* configManager);
    void setCallbacks(SaveWifiStaCallback saveStaCb, SaveWifiApCallback saveApCb, ForgetWifiCallback forgetCb);
    void setSaveConfigCallback(SaveConfigCallback saveConfigCb);
    void setScanCallback(ScanNetworksCallback scanCb);
    
    void begin();
    void update();
    
private:
    DataStorage* _dataStorage;
    ConfigManager* _configManager;
    
    ESP8266WebServer* _server;
    
    SaveWifiStaCallback _saveStaCallback;
    SaveWifiApCallback _saveApCallback;
    ForgetWifiCallback _forgetCallback;
    SaveConfigCallback _saveConfigCallback;
    ScanNetworksCallback _scanCallback;
    
    void handleNotFound();
    void handleWifiSTAScan();
    void handleWifiSTAApply();
    void handleWifiSTAForget();
    void handleWifiApApply();
    void handleWifiApForm(); // For POST /wifi_ap form with ip
    void handleSaveConfig();
    void handleStatus();
};

#endif // WEB_SERVER_H
