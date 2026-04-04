#ifndef MAIN_APPLICATION_H
#define MAIN_APPLICATION_H

#include <Arduino.h>
#include "modules/display/DisplayManager.h"
#include "modules/display/PageManager.h"
#include "mediator/ConfigurationMediator.h"
#include "mediator/WebCallbackMediator.h"
#include "mediator/InitializationMediator.h"

// Forward declarations
class Scheduler;
class Task;

class MainApplication {
public:
    MainApplication();
    
    void begin();
    void appLoop();
    
    // Public methods for configuration (delegated to ConfigurationMediator)
    void saveWifiStaConfig(const char* ssid, const char* password);
    void saveWifiApConfig(const char* ssid, const char* password, const char* ip);
    void forgetWifiConfig();
    void saveConfig();
    void saveLedConfig(const PatternConfig& config);
    
    // Web callback getters
    String getWifiScanJson();
    String getLEDPatternsJson();
    String getLEDCurrentJson();
    void applyLEDSettings(const char* pattern, const char* color, uint16_t speed, bool direction);
    
    // Web callback setters
    void setWebServerCallbacks();
    
    // Get access to mediators
    ConfigurationMediator* getConfigurationMediator() { return _configMediator; }
    
private:
    // Module pointers
    LEDController* _ledController;
    DisplayManager* _allDisplays[8];
    PageManager* _pageManager;
    ConfigManager* _configManager;
    DataStorage* _dataStorage;
    BuiltInLED* _builtInLED;
    WiFiAP* _wifiAP;
    WiFiSTA* _wifiSTA;
    WebServer* _wifiWebServer;
    Geolocation* _geolocation;
    Weather* _weather;
    AirQuality* _airQuality;
    LoRa* _lora;
    
    DeviceConfig _deviceConfig;
    
    // Mediators
    ConfigurationMediator* _configMediator;
    WebCallbackMediator* _webCallbackMediator;
    InitializationMediator* _initMediator;
    
    // Task scheduler and tasks
    Scheduler* _scheduler;
    Task* tLoRaSend;
    Task* tHeartbeat;
    Task* tPageRotation;
    Task* tAirQualityUpdate;
    Task* tGeolocationUpdate;
    Task* tWeatherUpdate;
    Task* tBuiltInLEDUpdate;
    
    // Static task callbacks
    static void staticLoRaSend();
    static void staticHeartbeat();
    static void staticPageRotation();
    static void staticAirQualityUpdate();
    static void staticGeolocationUpdate();
    static void staticWeatherUpdate();
    static void staticBuiltInLEDUpdate();
    
    void initApp();
};

extern MainApplication app;
extern MainApplication* appInstance;

#endif // MAIN_APPLICATION_H
