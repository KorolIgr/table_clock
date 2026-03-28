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
    _sharedData.geo_ip_address = "";
    // Initialize weather forecast
    for (int i = 0; i < 7; i++) {
        _sharedData.weather_forecast[i] = WeatherDay();
    }
    _sharedData.weather_last_update = 0;
    _sharedData.weather_valid = false;
    _sharedData.weather_error = "";
    _sharedData.last_error = "";
    // Initialize AP info
    _sharedData.ap_active = false;
    _sharedData.ap_ssid = "";
    _sharedData.ap_password = "";
    _sharedData.ap_ip = "";
    _sharedData.ap_mask = "";
    _sharedData.ap_mac = "";
    _sharedData.ap_channel = 0;
    _sharedData.ap_max_clients = 0;
    _sharedData.ap_connected_clients = 0;
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

void DataStorage::updateGeolocation(const String& continent, const String& country, const String& city, float latitude, float longitude, const String& ip_address) {
    if (!_locked) {
        _sharedData.continent = continent;
        _sharedData.country = country;
        _sharedData.city = city;
        _sharedData.latitude = latitude;
        _sharedData.longitude = longitude;
        _sharedData.geo_ip_address = ip_address;
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

void DataStorage::updateAPInfo(bool active, const String& ssid, const String& password, const String& ip, const String& mask, const String& mac, uint8_t channel, uint8_t maxClients, uint8_t connectedClients) {
    if (!_locked) {
        _sharedData.ap_active = active;
        _sharedData.ap_ssid = ssid;
        _sharedData.ap_password = password;
        _sharedData.ap_ip = ip;
        _sharedData.ap_mask = mask;
        _sharedData.ap_mac = mac;
        _sharedData.ap_channel = channel;
        _sharedData.ap_max_clients = maxClients;
        _sharedData.ap_connected_clients = connectedClients;
    }
}

void DataStorage::updateCurrentWeather(float temperature, float apparent_temperature, float wind_speed, int wind_direction, int humidity, int cloud_cover, int weather_code, bool valid, const String& error) {
    if (!_locked) {
        if (valid) {
            _sharedData.current_temperature = temperature;
            _sharedData.current_apparent_temperature = apparent_temperature;
            _sharedData.current_wind_speed = wind_speed;
            _sharedData.current_wind_direction = wind_direction;
            _sharedData.current_humidity = humidity;
            _sharedData.current_cloud_cover = cloud_cover;
            _sharedData.current_weather_code = weather_code;
            _sharedData.current_weather_last_update = millis();
            _sharedData.current_weather_valid = true;
            _sharedData.current_weather_error = "";
        } else {
            _sharedData.current_weather_valid = false;
            _sharedData.current_weather_error = error;
        }
    }
}