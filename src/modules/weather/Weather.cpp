#include "Weather.h"
#include <ESP8266WiFi.h>

Weather::Weather(DataStorage* dataStorage)
    : _dataStorage(dataStorage), _lastUpdateAttempt(0) {
}

void Weather::begin() {
    _lastUpdateAttempt = 0;
}

void Weather::update() {
    if (!_dataStorage) return;
    
    if (shouldAttemptUpdate()) {
        _lastUpdateAttempt = millis();
        fetchWeather();
    }
}

bool Weather::isUpdateNeeded() const {
    if (!_dataStorage) return false;
    
    auto& data = _dataStorage->getData();
    unsigned long lastUpdate = data.weather_last_update;
    
    if (lastUpdate == 0) return true;
    
    return (millis() - lastUpdate) >= UPDATE_INTERVAL;
}

void Weather::forceUpdate() {
    _lastUpdateAttempt = 0;
}

bool Weather::isValid() const {
    if (!_dataStorage) return false;
    return _dataStorage->getData().weather_valid;
}

unsigned long Weather::getLastUpdate() const {
    if (!_dataStorage) return 0;
    return _dataStorage->getData().weather_last_update;
}

String Weather::getError() const {
    if (!_dataStorage) return "";
    return _dataStorage->getData().weather_error;
}

WeatherDay Weather::getForecast(int day) const {
    WeatherDay empty;
    if (!_dataStorage || day < 0 || day >= 7) return empty;
    return _dataStorage->getData().weather_forecast[day];
}

bool Weather::shouldAttemptUpdate() const {
    // Only attempt if hourly update is needed
    if (!isUpdateNeeded()) {
        return false;
    }
    // If never attempted yet, allow immediate attempt
    if (_lastUpdateAttempt == 0) {
        return true;
    }
    // Otherwise ensure minimum interval between attempts to avoid rapid retries
    return (millis() - _lastUpdateAttempt) >= MIN_UPDATE_INTERVAL;
}

bool Weather::hasValidCoordinates() const {
    if (!_dataStorage) return false;
    auto& data = _dataStorage->getData();
    return data.latitude != 0.0f || data.longitude != 0.0f;
}

bool Weather::fetchWeather() {
    if (!_dataStorage) return false;
    
    auto& data = _dataStorage->getData();
    
    if (!data.wifi_connected) {
        Serial.println("Weather: WiFi not connected, skipping update");
        return false;
    }
    
    if (!hasValidCoordinates()) {
        Serial.println("Weather: No valid coordinates available, skipping update");
        return false;
    }
    
    float lat = data.latitude;
    float lon = data.longitude;
    
    Serial.print("Weather: Fetching forecast for lat=");
    Serial.print(lat);
    Serial.print(", lon=");
    Serial.println(lon);
    
    HTTPClient http;
    WiFiClient client;
    client.setTimeout(2000); // 2-second timeout to avoid WDT
    
    String url = "http://api.open-meteo.com/v1/forecast";
    url += "?latitude=" + String(lat, 6);
    url += "&longitude=" + String(lon, 6);
    url += "&daily=temperature_2m_max,temperature_2m_min,weather_code";
    url += "&timezone=auto";
    
    // Feed watchdog before blocking network operation
    yield();
    
    http.begin(client, url);
    // No need for Content-Type header on GET
    
    int httpCode = http.GET();
    
    if (httpCode != HTTP_CODE_OK) {
        Serial.print("Weather: HTTP request failed, code: ");
        Serial.println(httpCode);
        http.end();
        String error = "HTTP error " + String(httpCode);
        _dataStorage->updateWeather(nullptr, 0, false, error);
        return false;
    }
    
    String payload = http.getString();
    http.end();
    
    Serial.println("Weather: Response received:");
    Serial.println(payload);
    
    StaticJsonDocument<2048> doc;
    DeserializationError error = deserializeJson(doc, payload);
    
    if (error) {
        Serial.print("Weather: JSON parse failed: ");
        Serial.println(error.c_str());
        _dataStorage->updateWeather(nullptr, 0, false, "JSON parse error");
        return false;
    }
    
    JsonObject daily = doc["daily"].as<JsonObject>();
    if (!daily) {
        Serial.println("Weather: Invalid response - missing daily object");
        _dataStorage->updateWeather(nullptr, 0, false, "Invalid response format");
        return false;
    }
    
    JsonArray timeArray = daily["time"].as<JsonArray>();
    JsonArray tempMaxArray = daily["temperature_2m_max"].as<JsonArray>();
    JsonArray tempMinArray = daily["temperature_2m_min"].as<JsonArray>();
    JsonArray weatherCodeArray = daily["weather_code"].as<JsonArray>();
    
    if (!timeArray || !tempMaxArray || !tempMinArray || !weatherCodeArray ||
        timeArray.size() < 7 || tempMaxArray.size() < 7 || tempMinArray.size() < 7 || weatherCodeArray.size() < 7) {
        Serial.println("Weather: Invalid response - incomplete arrays");
        _dataStorage->updateWeather(nullptr, 0, false, "Invalid response format");
        return false;
    }
    
    WeatherDay forecast[7];
    
    for (int i = 0; i < 7; i++) {
        forecast[i].date = timeArray[i].as<String>();
        forecast[i].temp_max = tempMaxArray[i].as<float>();
        forecast[i].temp_min = tempMinArray[i].as<float>();
        forecast[i].weather_code = weatherCodeArray[i].as<int>();
    }
    
    updateDataStorage(forecast, 7);
    
    Serial.println("Weather: Forecast updated successfully");
    for (int i = 0; i < 7; i++) {
        Serial.print("  Day ");
        Serial.print(i);
        Serial.print(": ");
        Serial.print(forecast[i].date);
        Serial.print(" max=");
        Serial.print(forecast[i].temp_max);
        Serial.print(" min=");
        Serial.print(forecast[i].temp_min);
        Serial.print(" code=");
        Serial.println(forecast[i].weather_code);
    }
    
    return true;
}

void Weather::updateDataStorage(const WeatherDay* forecast, int days) {
    if (_dataStorage && forecast && days == 7) {
        _dataStorage->updateWeather(forecast, days, true, "");
    }
}