#ifndef INITIALIZATION_MEDIATOR_H
#define INITIALIZATION_MEDIATOR_H

#include "modules/config/ConfigManager.h"
#include "modules/data_storage/DataStorage.h"
#include "app_config.h"
#include "modules/led_line/LEDController.h"
#include "modules/led_builtin/BuiltInLED.h"
#include "modules/wifi_ap/WiFiAP.h"
#include "modules/wifi_sta/WiFiSTA.h"
#include "modules/web_server/WebServer.h"
#include "modules/display/DisplayManager.h"
#include "modules/srv_geolocation/Geolocation.h"
#include "modules/srv_weather/Weather.h"
#include "modules/sensor_air_quality/AirQuality.h"
#include "modules/modem_lora/LoRa.h"

class InitializationMediator {
public:
    InitializationMediator(ConfigManager* configManager, DataStorage* dataStorage,
                           DisplayManager** allDisplays,
                           LEDController* ledController, BuiltInLED* builtInLED,
                           WiFiAP* wifiAP, WiFiSTA* wifiSTA, WebServer* wifiWebServer,
                           Geolocation* geolocation, Weather* weather,
                           AirQuality* airQuality, LoRa* lora);
    
    void initConfig(DeviceConfig& deviceConfig);
    void initHardware();
    void initWiFiAP(DeviceConfig& deviceConfig);
    void initWiFiSTA(DeviceConfig& deviceConfig);
    void initWebServer();
    void initDisplay();
    void initLED(DeviceConfig& deviceConfig);
    void initGeolocation();
    void initWeather();
    void initAirQuality();
    void initLoRa();
    
private:
    ConfigManager* _configManager;
    DataStorage* _dataStorage;
    DisplayManager** _allDisplays;
    LEDController* _ledController;
    BuiltInLED* _builtInLED;
    WiFiAP* _wifiAP;
    WiFiSTA* _wifiSTA;
    WebServer* _wifiWebServer;
    Geolocation* _geolocation;
    Weather* _weather;
    AirQuality* _airQuality;
    LoRa* _lora;
};

#endif // INITIALIZATION_MEDIATOR_H
