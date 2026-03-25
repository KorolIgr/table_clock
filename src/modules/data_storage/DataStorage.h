#ifndef DATA_STORAGE_H
#define DATA_STORAGE_H

#include <Arduino.h>

// Forward declarations
class ConfigManager;

// Weather forecast data for a single day
struct WeatherDay {
    String date;
    float temp_max = 0.0f;
    float temp_min = 0.0f;
    int weather_code = 0;  // WMO weather code
};

// Shared data structure for exchanging data between modules
struct SharedData {
    // WiFi status information
    bool wifi_connected = false;
    String wifi_status = "";
    String ip_address = "";
    
    // WiFi STA configuration (for dynamic connection requests)
    String sta_ssid = "";
    String sta_password = "";
    bool sta_connect_requested = false;
    
    // LED status information
    bool led_enabled = true;
    uint8_t led_brightness = 100;  // 0-100 percentage
    int builtin_led_pattern = 1;   // 0=off, 1=slow blink, 2=fast blink, 3=on
    
    // System status information
    float temperature = 0.0f;
    uint32_t uptime = 0;  // in seconds
    bool system_error = false;
    
    // Geolocation information
    String continent = "";
    String country = "";
    String city = "";
    float latitude = 0.0f;
    float longitude = 0.0f;
    unsigned long geo_last_update = 0;  // timestamp of last update
    
    // Weather information
    WeatherDay weather_forecast[7];  // 7-day forecast
    unsigned long weather_last_update = 0;  // timestamp of last update
    bool weather_valid = false;
    String weather_error = "";
    
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
    void updateGeolocation(const String& continent, const String& country, const String& city, float latitude = 0.0f, float longitude = 0.0f);
    void updateWeather(const WeatherDay* forecast, int days, bool valid = true, const String& error = "");
    
    // WiFi STA configuration methods
    void setStaCredentials(const String& ssid, const String& password);
    void requestStaConnection();
    void clearStaConnectionRequest();
    
private:
    SharedData _sharedData;
    bool _locked;
    
    // Friend classes that can access private members directly if needed
    friend class ConfigManager;
};

#endif // DATA_STORAGE_H