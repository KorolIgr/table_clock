#include "WiFiAP.h"
#include "../../app_config.h"
#include <cstdio>

WiFiAP::WiFiAP()
    : _dataStorage(nullptr), _ip(0, 0, 0, 0), _channel(0), _maxClients(0) {
}

void WiFiAP::setDataStorage(DataStorage* dataStorage) {
    _dataStorage = dataStorage;
}

void WiFiAP::begin(const char* ssid, const char* password, const char* ip_str) {
    _ssid = ssid ? ssid : "";
    _password = password ? password : "";
    
    IPAddress ip(192, 168, 4, 1);
    IPAddress gateway(192, 168, 4, 1);
    IPAddress subnet(255, 255, 255, 0);
    
    if (ip_str && strlen(ip_str) > 0) {
        // Parse IP string (e.g., "192.168.4.1")
        int parts[4];
        if (sscanf(ip_str, "%d.%d.%d.%d", &parts[0], &parts[1], &parts[2], &parts[3]) == 4) {
            ip = IPAddress(parts[0], parts[1], parts[2], parts[3]);
            gateway = ip;
        }
    }
    
    WiFi.softAPConfig(ip, gateway, subnet);
    WiFi.softAP(_ssid.c_str(), _password.c_str());
    
    _ip = WiFi.softAPIP();
    _mask = subnet.toString();  // Store the subnet mask we configured
    _mac = WiFi.softAPmacAddress();
    _channel = WiFi.channel();
    _maxClients = 4;  // Default maximum clients for ESP8266 AP mode
    
    Serial.println("Access Point initialized:");
    Serial.print("SSID: ");
    Serial.println(_ssid);
    Serial.print("Password: ");
    Serial.println(_password);
    Serial.print("IP Address: ");
    Serial.println(_ip.toString());
    Serial.print("MAC Address: ");
    Serial.println(_mac);
    Serial.print("Channel: ");
    Serial.println(_channel);
    
    // Push AP info to DataStorage
    updateDataStorage();
}

void WiFiAP::update() {
    if (_dataStorage) {
        updateDataStorage();
    }
}

void WiFiAP::updateDataStorage() {
    if (_dataStorage) {
        uint8_t connected = WiFi.softAPgetStationNum();
        _dataStorage->updateAPInfo(true, _ssid, _password, _ip.toString(), _mask, _mac, _channel, _maxClients, connected);
    }
}

