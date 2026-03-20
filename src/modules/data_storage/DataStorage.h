#ifndef DATA_STORAGE_H
#define DATA_STORAGE_H

#include <Arduino.h>

// Forward declarations
class ConfigManager;

// Shared data structure for exchanging data between modules
struct SharedData {
    // WiFi status information
    bool wifi_connected = false;
    String wifi_status = "";
    String ip_address = "";
    
    // LED status information
    bool led_enabled = true;
    uint8_t led_brightness = 100;  // 0-100 percentage
    int builtin_led_pattern = 1;   // 0=off, 1=slow blink, 2=fast blink, 3=on
    
    // System status information
    float temperature = 0.0f;
    uint32_t uptime = 0;  // in seconds
    bool system_error = false;
    
    // Additional fields can be added as needed
    String last_error = "";
};

class DataStorage {
public:
    DataStorage();
    
    // Get reference to shared data
    SharedData& getData();
    
    // Thread-safe access methods
    void lock();
    void unlock();
    
    // Update specific data fields
    void updateWifiStatus(bool connected, const String& status, const String& ip);
    void updateLedStatus(bool enabled, uint8_t brightness);
    void updateBuiltinLEDPattern(int pattern);
    void updateSystemStatus(float temp, uint32_t uptime, bool error, const String& lastError = "");
    
private:
    SharedData _sharedData;
    bool _locked;
    
    // Friend classes that can access private members directly if needed
    friend class ConfigManager;
};

#endif // DATA_STORAGE_H