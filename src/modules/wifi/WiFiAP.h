#ifndef WIFI_AP_H
#define WIFI_AP_H

#include <ESP8266WiFi.h>
#include "../data_storage/DataStorage.h"

class WiFiAP {
public:
    WiFiAP();
    
    void setDataStorage(DataStorage* dataStorage);
    
    void begin(const char* ssid, const char* password, const char* ip_str = nullptr);
    void update();  // Call periodically to update AP status in DataStorage
    IPAddress getIP() const { return _ip; }
    String getSSID() const { return _ssid; }
    
private:
    void updateDataStorage();  // Helper to push AP info to DataStorage
    
    DataStorage* _dataStorage;
    String _ssid;
    String _password;
    IPAddress _ip;
    String _mask;         // Subnet mask
    String _mac;          // MAC address
    uint8_t _channel;     // WiFi channel
    uint8_t _maxClients;  // Maximum allowed clients
};

#endif // WIFI_AP_H
