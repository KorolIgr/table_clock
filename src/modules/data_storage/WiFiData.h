#ifndef WIFI_DATA_H
#define WIFI_DATA_H

#include <Arduino.h>

// WiFi STA (client) data
struct WiFiSTAData {
    bool connected = false;
    String status = "";
    String ip_address = "";
    String ssid = "";
    String subnet_mask = "";
    String gateway_ip = "";
    int rssi = 0;
    uint8_t channel = 0;
    
    // Credentials for dynamic connection requests
    String sta_ssid = "";
    String sta_password = "";
    bool sta_connect_requested = false;
};

// WiFi AP (access point) data
struct WiFiAPData {
    bool active = false;
    String ssid = "";
    String password = "";
    String ip = "";
    String mask = "";
    String mac = "";
    uint8_t channel = 0;
    uint8_t max_clients = 0;
    uint8_t connected_clients = 0;
};

// Combined WiFi data
struct WiFiData {
    WiFiSTAData sta;
    WiFiAPData ap;
};

#endif // WIFI_DATA_H
