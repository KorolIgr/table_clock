#ifndef MAIN_H
#define MAIN_H

#include "app_config.h"
#include "modules/led/LEDController.h"
#include "modules/display/DisplayManager.h"
#include "modules/wifi/WiFiManager.h"

// Forward declarations
class MainApplication {
public:
    MainApplication();
    void begin();
    void appLoop();  // Renamed from loop to avoid conflict with Arduino's loop

private:
    LEDController* _ledController;
    DisplayManager* _displayManager;
    WiFiManager* _wifiManager;
    
    void initHardware();
    void initWiFi();
    void initDisplay();
    void initLED();
};

#endif // MAIN_H
