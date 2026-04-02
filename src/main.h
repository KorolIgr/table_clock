#ifndef MAIN_H
#define MAIN_H

#include "app_config.h"
#include "modules/led/LEDController.h"
#include "modules/display/DisplayManager.h"
#include "modules/display/PageManager.h"
#include "modules/builtin_led/BuiltInLED.h"
#include "modules/wifi/WiFiAP.h"
#include "modules/wifi/WiFiSTA.h"
#include "modules/webserver/WebServer.h"
#include "modules/config/ConfigManager.h"
#include "modules/data_storage/DataStorage.h"
#include "modules/geolocation/Geolocation.h"
#include "modules/weather/Weather.h"
#include "modules/AHT21/AHT21.h"
#include "modules/ENS160/ENS160.h"

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
    
    // Private helper methods
    // Removed updateAllDisplays method - counter functionality removed
    void initAHT21();
    void initENS160();

private:
    LEDController* _ledController;
    DisplayManager* _displayManager;  // First display (channel 0) - keeping for backward compatibility
    DisplayManager* _allDisplays[8];  // Array to hold all 8 displays
    PageManager* _pageManager;        // Central page manager for all displays
    ConfigManager* _configManager;
    DataStorage* _dataStorage;
    BuiltInLED* _builtInLED;
    WiFiAP* _wifiAP;
    WiFiSTA* _wifiSTA;
    WebServer* _wifiWebServer;
    Geolocation* _geolocation;
    Weather* _weather;
    AHT21Wrapper* _aht21;
    ENS160Wrapper* _ens160;
    DeviceConfig _deviceConfig;
    
    // Removed counter-related variables - using page system instead
    
    void initHardware();
    void initConfig();
    void initWiFiAP();
    void initWiFiSTA();
    void initWebServer();
    void initDisplay();
    void initLED();
    void initGeolocation();
    void initWeather();
    void connectLEDControllerToWiFi();
    void saveConfig();
};

// Global application instance declaration
extern MainApplication* appInstance;

#endif // MAIN_H
