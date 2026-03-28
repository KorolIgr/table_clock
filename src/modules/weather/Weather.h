#ifndef WEATHER_H
#define WEATHER_H

#include <Arduino.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include "../data_storage/DataStorage.h"

class Weather {
public:
    Weather(DataStorage* dataStorage);
    
    void begin();
    void update();
    bool isUpdateNeeded() const;
    void forceUpdate();
    
    // Get weather data from storage
    bool isValid() const;
    unsigned long getLastUpdate() const;
    String getError() const;
    
    // Get forecast for a specific day (0-6)
    WeatherDay getForecast(int day) const;
    
private:
    DataStorage* _dataStorage;
    unsigned long _lastUpdateAttempt;
    
    static const unsigned long UPDATE_INTERVAL = 3600000; // 1 hour in ms
    static const unsigned long MIN_UPDATE_INTERVAL = 300000; // 5 min minimum between retries
    
    bool fetchWeather();
    void updateDataStorage(const WeatherDay* forecast, int days);
    bool fetchCurrentWeather();
    void updateCurrentDataStorage(float temperature, float apparent_temperature, float wind_speed, int wind_direction, int humidity, int cloud_cover, int weather_code);
    bool shouldAttemptUpdate() const;
    bool hasValidCoordinates() const;
};

#endif // WEATHER_H