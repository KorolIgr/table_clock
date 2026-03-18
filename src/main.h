#ifndef MAIN_H
#define MAIN_H

#include "app_config.h"
#include "modules/led/LEDController.h"
#include "modules/display/DisplayManager.h"
#include "modules/wifi/WiFiManager.h"
#include "modules/config/ConfigManager.h"

// Forward declarations
class MainApplication {
public:
    MainApplication();
    void begin();
    void appLoop();  // Renamed from loop to avoid conflict with Arduino's loop
    // Public methods for WiFi configuration
    void saveWifiStaConfig(const char* ssid, const char* password);
    void saveWifiApConfig(const char* ssid, const char* password);
    void forgetWifiConfig();
    void saveLedConfig(const PatternConfig& config);

private:
    LEDController* _ledController;
    DisplayManager* _displayManager;
    WiFiManager* _wifiManager;
    ConfigManager* _configManager;
    DeviceConfig _deviceConfig;
    
    void initHardware();
    void initConfig();
    void initWiFi();
    void initDisplay();
    void initLED();
    void connectLEDControllerToWiFi();
};

// Global application instance declaration
extern MainApplication* appInstance;

#endif // MAIN_H
