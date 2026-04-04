#ifndef WEB_CALLBACK_MEDIATOR_H
#define WEB_CALLBACK_MEDIATOR_H

#include <Arduino.h>
#include "modules/web_server/WebServer.h"
#include "modules/wifi_sta/WiFiSTA.h"
#include "modules/led_line/LEDController.h"
#include "app_config.h"

class WebCallbackMediator {
public:
    WebCallbackMediator(WiFiSTA* wifiSTA, LEDController* ledController);
    
    String getWifiScanJson();
    String getLEDPatternsJson();
    String getLEDCurrentJson();
    void applyLEDSettings(const char* pattern, const char* color, uint16_t speed, bool direction);
    
    // Callback getters for WebServer
    String (*getScanCallback())(void);
    String (*getLEDPatternsCallback())(void);
    String (*getLEDCurrentCallback())(void);
    void (*getApplyLEDSettingsCallback())(const char*, const char*, uint16_t, bool);
    
    // Set static instance pointer for callbacks
    static WebCallbackMediator* instance;

private:
    WiFiSTA* _wifiSTA;
    LEDController* _ledController;
    
    static String staticGetWifiScanJson();
    static String staticGetLEDPatternsJson();
    static String staticGetLEDCurrentJson();
    static void staticApplyLEDSettings(const char* pattern, const char* color, uint16_t speed, bool direction);
};

#endif // WEB_CALLBACK_MEDIATOR_H
