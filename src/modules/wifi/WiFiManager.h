#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "../led/LEDController.h"

class WiFiManager {
public:
    WiFiManager();
    
    void begin(const char* ssid, const char* password, WiFiMode_t mode = WIFI_AP_STA);
    void update();
    bool isConnected();
    WiFiMode_t getMode();
    const char* getLocalIP();
    ESP8266WebServer* getServer();
    void setLEDController(LEDController* ledController);
    bool hasStoredCredentials();
    bool hasStoredApCredentials();
    bool loadStoredCredentials(String& ssid, String& password);
    bool loadStoredApCredentials(String& ssid, String& password);
    void saveCredentials(const char* ssid, const char* password);
    void saveApCredentials(const char* ssid, const char* password);
    void forgetCredentials();
    String scanNetworks();
    void updateBuiltinLED();

private:
    WiFiMode_t _mode;
    String _ssid;
    String _password;
    String _localIP;
    ESP8266WebServer* _server;
    LEDController* _ledController;
    bool _credentialsLoaded;
    bool _hasCredentials;
    char _storedSsid[33];  // Max SSID length is 32 + null terminator (for STA)
    char _storedPassword[65];  // Max password length is 64 + null terminator (for STA)
    char _storedApSsid[33];  // Max SSID length is 32 + null terminator (for AP)
    char _storedApPassword[65];  // Max password length is 64 + null terminator (for AP)
    
    // Built-in LED state tracking
    unsigned long _lastLEDToggle;
    bool _ledState;
    int _ledPattern; // 0 = off, 1 = slow blink (5s), 2 = fast blink (4Hz), 3 = on
    
    void setupAP();
    void setupSTA(const char* ssid, const char* password);
    void handleRoot();
    void handleConfig();
    void handleLED();
    void handleLEDPattern();
    void handleWifiAP();
    void handleWifiSTA();
    void handleForgetWifi();
    void handleScanWifi();
};

#endif // WIFI_MANAGER_H
