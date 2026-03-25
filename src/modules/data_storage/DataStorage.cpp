#include "DataStorage.h"

DataStorage::DataStorage() : _locked(false) {
    // Initialize with default values
    _sharedData.wifi_connected = false;
    _sharedData.wifi_status = "Not connected";
    _sharedData.ip_address = "0.0.0.0";
    _sharedData.led_enabled = true;
    _sharedData.led_brightness = 100;
    _sharedData.builtin_led_pattern = 1;
    _sharedData.temperature = 0.0f;
    _sharedData.uptime = 0;
    _sharedData.system_error = false;
    _sharedData.continent = "";
    _sharedData.country = "";
    _sharedData.city = "";
    _sharedData.latitude = 0.0f;
    _sharedData.longitude = 0.0f;
    _sharedData.geo_last_update = 0;
    // Initialize weather forecast
    for (int i = 0; i < 7; i++) {
        _sharedData.weather_forecast[i] = WeatherDay();
    }
    _sharedData.weather_last_update = 0;
    _sharedData.weather_valid = false;
    _sharedData.weather_error = "";
    _sharedData.last_error = "";
}

SharedData& DataStorage::getData() {
    return _sharedData;
}

void DataStorage::lock() {
    _locked = true;
}

void DataStorage::unlock() {
    _locked = false;
}

void DataStorage::updateWifiStatus(bool connected, const String& status, const String& ip) {
    if (!_locked) {
        _sharedData.wifi_connected = connected;
        _sharedData.wifi_status = status;
        _sharedData.ip_address = ip;
    }
}

void DataStorage::updateLedStatus(bool enabled, uint8_t brightness) {
    if (!_locked) {
        _sharedData.led_enabled = enabled;
        _sharedData.led_brightness = brightness;
    }
}

void DataStorage::updateBuiltinLEDPattern(int pattern) {
    if (!_locked) {
        _sharedData.builtin_led_pattern = pattern;
    }
}

void DataStorage::updateSystemStatus(float temp, uint32_t uptime, bool error, const String& lastError) {
    if (!_locked) {
        _sharedData.temperature = temp;
        _sharedData.uptime = uptime;
        _sharedData.system_error = error;
        _sharedData.last_error = lastError;
    }
}

void DataStorage::setStaCredentials(const String& ssid, const String& password) {
    if (!_locked) {
        _sharedData.sta_ssid = ssid;
        _sharedData.sta_password = password;
    }
}

void DataStorage::requestStaConnection() {
    if (!_locked) {
        _sharedData.sta_connect_requested = true;
    }
}

void DataStorage::clearStaConnectionRequest() {
    if (!_locked) {
        _sharedData.sta_connect_requested = false;
    }
}

void DataStorage::updateGeolocation(const String& continent, const String& country, const String& city, float latitude, float longitude) {
    if (!_locked) {
        _sharedData.continent = continent;
        _sharedData.country = country;
        _sharedData.city = city;
        _sharedData.latitude = latitude;
        _sharedData.longitude = longitude;
        _sharedData.geo_last_update = millis();
    }
}

void DataStorage::updateWeather(const WeatherDay* forecast, int days, bool valid, const String& error) {
    if (!_locked) {
        if (valid && forecast && days == 7) {
            for (int i = 0; i < 7; i++) {
                _sharedData.weather_forecast[i] = forecast[i];
            }
            _sharedData.weather_last_update = millis();
            _sharedData.weather_valid = true;
            _sharedData.weather_error = "";
        } else {
            _sharedData.weather_valid = false;
            _sharedData.weather_error = error;
        }
    }
}