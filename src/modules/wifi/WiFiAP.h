#ifndef WIFI_AP_H
#define WIFI_AP_H

#include <ESP8266WiFi.h>
#include "../data_storage/DataStorage.h"

class WiFiAP {
public:
    WiFiAP();
    
    void setDataStorage(DataStorage* dataStorage);
    
    void begin(const char* ssid, const char* password, const char* ip_str = nullptr);
    IPAddress getIP() const;
    String getSSID() const { return _ssid; }
    
private:
    DataStorage* _dataStorage;
    String _ssid;
    String _password;
    IPAddress _ip;
};

#endif // WIFI_AP_H
