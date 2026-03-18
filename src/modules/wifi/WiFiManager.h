#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "../led/LEDController.h"
#include "../config/ConfigManager.h"  // Include the new config header

// Define callback function types
typedef void (*SaveWifiStaCallback)(const char* ssid, const char* password);
typedef void (*SaveWifiApCallback)(const char* ssid, const char* password);
typedef void (*ForgetWifiCallback)();

class WiFiManager {
public:
    WiFiManager();
    
    // Legacy begin - uses same credentials for both AP and STA
    void begin(const char* ssid, const char* password, WiFiMode_t mode = WIFI_AP_STA);
    
    // New begin - accepts separate AP and STA credentials for hybrid mode
    void begin(const char* ap_ssid, const char* ap_password, 
               const char* sta_ssid, const char* sta_password,
               WiFiMode_t mode = WIFI_AP_STA);
    void update();
    bool isConnected();
    WiFiMode_t getMode();
    const char* getLocalIP();
    ESP8266WebServer* getServer();
    void setLEDController(LEDController* ledController);
    void setCallbacks(SaveWifiStaCallback saveStaCb, SaveWifiApCallback saveApCb, ForgetWifiCallback forgetCb) {
        _saveStaCallback = saveStaCb;
        _saveApCallback = saveApCb;
        _forgetCallback = forgetCb;
    }
    // Removed the old credential functions since we're using the new config system
    void saveCredentials(const char* ssid, const char* password);
    void saveApCredentials(const char* ssid, const char* password);
    void forgetCredentials();
    String scanNetworks();
    void updateBuiltinLED();

private:
    WiFiMode_t _mode;
    String _localIP;
    ESP8266WebServer* _server;
    LEDController* _ledController;
    // Callback functions for configuration management
    SaveWifiStaCallback _saveStaCallback = nullptr;
    SaveWifiApCallback _saveApCallback = nullptr;
    ForgetWifiCallback _forgetCallback = nullptr;
    bool _credentialsLoaded;
    bool _hasCredentials;
    // Removed the old credential storage variables since we're using the new config system
    
    // Built-in LED state tracking
    unsigned long _lastLEDToggle;
    bool _ledState;
    int _ledPattern; // 0 = off, 1 = slow blink (5s), 2 = fast blink (4Hz), 3 = on
    
    // Page path constants
    static constexpr char PAGE_INDEX[] = "/index.html.gz";
    static constexpr char PAGE_LED[] = "/led.html.gz";
    static constexpr char PAGE_WIFI_AP[] = "/wifi_ap.html.gz";
    static constexpr char PAGE_WIFI_STA[] = "/wifi_sta.html.gz";
    
    // Helper method for page not found response
    void sendPageNotFound();
    
    void setupAP(const char* ssid, const char* password);
    void setupSTA(const char* ssid, const char* password);
    void handleRoot();
    void handleLED();
    void handleWifiAP();
    void handleWifiSTA();
    void handleForgetWifi();
    void handleScanWifi();
};

#endif // WIFI_MANAGER_H
