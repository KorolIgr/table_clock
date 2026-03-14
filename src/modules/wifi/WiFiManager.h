#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

enum WiFiMode {
    MODE_STA,
    MODE_AP,
    MODE_AP_STA
};

class WiFiManager {
public:
    WiFiManager();
    
    void begin(const char* ssid, const char* password, WiFiMode mode = MODE_AP_STA);
    void update();
    bool isConnected();
    WiFiMode getMode();
    const char* getLocalIP();
    ESP8266WebServer* getServer();

private:
    WiFiMode _mode;
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
