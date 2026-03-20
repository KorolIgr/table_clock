#ifndef MAIN_H
#define MAIN_H

#include "app_config.h"
#include "modules/led/LEDController.h"
#include "modules/display/DisplayManager.h"
#include "modules/builtin_led/BuiltInLED.h"
#include "modules/wifi/WiFiAP.h"
#include "modules/wifi/WiFiSTA.h"
#include "modules/webserver/WebServer.h"
#include "modules/config/ConfigManager.h"
#include "modules/data_storage/DataStorage.h"

// Forward declarations
class MainApplication {
public:
    MainApplication();
    
    void begin();
    void appLoop();  // Renamed from loop to avoid conflict with Arduino's loop
    
    // Public methods for WiFi configuration
    void saveWifiStaConfig(const char* ssid, const char* password);
    void saveWifiApConfig(const char* ssid, const char* password, const char* ip);
    void forgetWifiConfig();
    void saveLedConfig(const PatternConfig& config);
    
    // Method to provide WiFi scan results as JSON (for web server callback)
    String getWifiScanJson();
    
    // LED-related methods for web interface
    String getLEDPatternsJson();
    String getLEDCurrentJson();
    void applyLEDSettings(const char* pattern, const char* color, uint16_t speed, bool direction);
    
    // Static callback wrappers
    static void onSaveWifiSta(const char* ssid, const char* password);
    static void onSaveWifiAp(const char* ssid, const char* password, const char* ip);
    static void onForgetWifi();
    static void onSaveConfig();
    static String scanNetworksCallback();
    static String onGetLEDPatterns();
    static String onGetLEDCurrent();
    static void onApplyLEDSettings(const char* pattern, const char* color, uint16_t speed, bool direction);

private:
    LEDController* _ledController;
    DisplayManager* _displayManager;
    ConfigManager* _configManager;
    DataStorage* _dataStorage;
    BuiltInLED* _builtInLED;
    WiFiAP* _wifiAP;
    WiFiSTA* _wifiSTA;
    WebServer* _wifiWebServer;
    DeviceConfig _deviceConfig;
    
    void initHardware();
    void initConfig();
    void initWiFiAP();
    void initWiFiSTA();
    void initWebServer();
    void initDisplay();
    void initLED();
    void connectLEDControllerToWiFi();
    void saveConfig();
};

// Global application instance declaration
extern MainApplication* appInstance;

#endif // MAIN_H
