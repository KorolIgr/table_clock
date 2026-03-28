#ifndef WEATHER_DATA_H
#define WEATHER_DATA_H

#include <Arduino.h>

// Weather forecast data for a single day
struct WeatherDay {
    String date;
    float temp_max = 0.0f;
    float temp_min = 0.0f;
    int weather_code = 0;  // WMO weather code
};

// Weather forecast data (7-day)
struct WeatherForecastData {
    WeatherDay forecast[7];
    unsigned long last_update = 0;  // timestamp of last update
    bool valid = false;
    String error = "";
};

// Current weather data
struct CurrentWeatherData {
    float temperature = 0.0f;
    float apparent_temperature = 0.0f;
    float wind_speed = 0.0f;
    int wind_direction = 0;  // degrees
    int humidity = 0;        // percentage
    int cloud_cover = 0;     // percentage
    int weather_code = 0;    // WMO weather code
    unsigned long last_update = 0;
    bool valid = false;
    String error = "";
};

// Combined weather data
struct WeatherData {
    WeatherForecastData forecast;
    CurrentWeatherData current;
};

#endif // WEATHER_DATA_H
