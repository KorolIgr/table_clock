#ifndef WIFI_STA_H
#define WIFI_STA_H

#include <ESP8266WiFi.h>
#include <vector>
#include "../data_storage/DataStorage.h"

// Structure for network information
struct NetworkInfo {
    String ssid;
    int rssi;
    bool secure;
};

class WiFiSTA {
public:
    WiFiSTA();
    
    void setDataStorage(DataStorage* dataStorage);
    
    void begin(const char* ssid, const char* password);
    void disconnect();
    void update(); // Check connection status and update DataStorage
    bool isConnected() const;
    String getIP() const;
    std::vector<NetworkInfo> scanNetworks();
    
private:
    DataStorage* _dataStorage;
    bool _lastConnectedStatus;
    
    void updateDataStorage();
};

#endif // WIFI_STA_H
