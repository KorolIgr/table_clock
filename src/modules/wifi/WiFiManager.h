#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

class WiFiManager {
public:
    WiFiManager();
    
    void begin(const char* ssid, const char* password, WiFiMode_t mode = WIFI_AP_STA);
    void update();
    bool isConnected();
    WiFiMode_t getMode();
    const char* getLocalIP();
    ESP8266WebServer* getServer();

private:
    WiFiMode_t _mode;
    String _ssid;
    String _password;
    String _localIP;
    ESP8266WebServer* _server;
    
    void setupAP();
    void setupSTA(const char* ssid, const char* password);
    void handleRoot();
    void handleConfig();
};

#endif // WIFI_MANAGER_H
