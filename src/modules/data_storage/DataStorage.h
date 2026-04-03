#ifndef DATA_STORAGE_H
#define DATA_STORAGE_H

#include <Arduino.h>
#include "WiFiData.h"
#include "LEDData.h"
//#include "SystemData.h"
#include "GeoData.h"
#include "WeatherData.h"
#include "AirQualityData.h"

// Forward declarations
//class ConfigManager;

class DataStorage {
public:
    DataStorage();
    
    // Get references to individual data structures
    WiFiData& wifi();
    LEDData& led();
    //SystemData& system();
    GeoData& geo();
    WeatherData& weather();
    AirQualityData& airQuality();
    
    // Thread-safe access methods
    void lock();
    void unlock();
    
    // Update methods - delegate to specific data structures
    void updateWifiStatus(bool connected, const String& status, const String& ip);
    void updateWifiStaInfo(const String& ssid, const String& subnetMask, const String& gatewayIp, int rssi, uint8_t channel);
    void updateLedStatus(bool enabled, uint8_t brightness);
    void updateBuiltinLEDPattern(int pattern);
    //void updateSystemStatus(float temp, uint32_t uptime, bool error, const String& lastError = "");
    void setStaCredentials(const String& ssid, const String& password);
    void requestStaConnection();
    void clearStaConnectionRequest();
    void updateAPInfo(bool active, const String& ssid, const String& password, const String& ip, const String& mask, const String& mac, uint8_t channel, uint8_t maxClients, uint8_t connectedClients);
    void updateGeolocation(const String& continent, const String& country, const String& city, float latitude = 0.0f, float longitude = 0.0f, const String& ip_address = "");
    void updateWeatherForecast(const WeatherDay* forecast, int days, bool valid = true, const String& error = "");
    void updateCurrentWeather(float temperature, float apparent_temperature, float wind_speed, int wind_direction, int humidity, int cloud_cover, int weather_code, bool valid = true, const String& error = "");
    void updateAirQuality(float tvoc, float eco2, int aqi, uint8_t flags, bool valid = true, const String& error = "");
    void updateAirQualityCompensation(float temperature, float humidity);
    
private:
    WiFiData _wifiData;
    LEDData _ledData;
    //SystemData _systemData;
    GeoData _geoData;
    WeatherData _weatherData;
    AirQualityData _airQualityData;
    
    bool _locked;
    
    // Friend classes that can access private members directly if needed
    friend class ConfigManager;
};

#endif // DATA_STORAGE_H