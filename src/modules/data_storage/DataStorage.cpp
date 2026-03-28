#include "DataStorage.h"

DataStorage::DataStorage() : _locked(false) {
    // WiFi data is initialized by its constructors
    // LED data is initialized by its constructor
    // System data is initialized by its constructor
    // Geo data is initialized by its constructors
    // Weather data is initialized by its constructors
}

// Accessor methods
WiFiData& DataStorage::wifi() {
    return _wifiData;
}

LEDData& DataStorage::led() {
    return _ledData;
}

//SystemData& DataStorage::system() {
//    return _systemData;
//}

GeoData& DataStorage::geo() {
    return _geoData;
}

WeatherData& DataStorage::weather() {
    return _weatherData;
}

void DataStorage::lock() {
    _locked = true;
}

void DataStorage::unlock() {
    _locked = false;
}

void DataStorage::updateWifiStatus(bool connected, const String& status, const String& ip) {
    if (!_locked) {
        _wifiData.sta.connected = connected;
        _wifiData.sta.status = status;
        _wifiData.sta.ip_address = ip;
    }
}

void DataStorage::updateLedStatus(bool enabled, uint8_t brightness) {
    if (!_locked) {
        _ledData.enabled = enabled;
        _ledData.brightness = brightness;
    }
}

void DataStorage::updateBuiltinLEDPattern(int pattern) {
    if (!_locked) {
        _ledData.builtin_led_pattern = pattern;
    }
}

/*
void DataStorage::updateSystemStatus(float temp, uint32_t uptime, bool error, const String& lastError) {
    if (!_locked) {
        _systemData.temperature = temp;
        _systemData.uptime = uptime;
        _systemData.system_error = error;
        _systemData.last_error = lastError;
    }
}

*/
void DataStorage::setStaCredentials(const String& ssid, const String& password) {
    if (!_locked) {
        _wifiData.sta.sta_ssid = ssid;
        _wifiData.sta.sta_password = password;
    }
}

void DataStorage::requestStaConnection() {
    if (!_locked) {
        _wifiData.sta.sta_connect_requested = true;
    }
}

void DataStorage::clearStaConnectionRequest() {
    if (!_locked) {
        _wifiData.sta.sta_connect_requested = false;
    }
}

void DataStorage::updateAPInfo(bool active, const String& ssid, const String& password, const String& ip, const String& mask, const String& mac, uint8_t channel, uint8_t maxClients, uint8_t connectedClients) {
    if (!_locked) {
        _wifiData.ap.active = active;
        _wifiData.ap.ssid = ssid;
        _wifiData.ap.password = password;
        _wifiData.ap.ip = ip;
        _wifiData.ap.mask = mask;
        _wifiData.ap.mac = mac;
        _wifiData.ap.channel = channel;
        _wifiData.ap.max_clients = maxClients;
        _wifiData.ap.connected_clients = connectedClients;
    }
}

void DataStorage::updateGeolocation(const String& continent, const String& country, const String& city, float latitude, float longitude, const String& ip_address) {
    if (!_locked) {
        _geoData.continent = continent;
        _geoData.country = country;
        _geoData.city = city;
        _geoData.latitude = latitude;
        _geoData.longitude = longitude;
        _geoData.ip_address = ip_address;
        _geoData.last_update = millis();
    }
}

void DataStorage::updateWeatherForecast(const WeatherDay* forecast, int days, bool valid, const String& error) {
    if (!_locked) {
        if (valid && forecast && days == 7) {
            for (int i = 0; i < 7; i++) {
                _weatherData.forecast.forecast[i] = forecast[i];
            }
            _weatherData.forecast.last_update = millis();
            _weatherData.forecast.valid = true;
            _weatherData.forecast.error = "";
        } else {
            _weatherData.forecast.valid = false;
            _weatherData.forecast.error = error;
        }
    }
}

void DataStorage::updateCurrentWeather(float temperature, float apparent_temperature, float wind_speed, int wind_direction, int humidity, int cloud_cover, int weather_code, bool valid, const String& error) {
    if (!_locked) {
        if (valid) {
            _weatherData.current.temperature = temperature;
            _weatherData.current.apparent_temperature = apparent_temperature;
            _weatherData.current.wind_speed = wind_speed;
            _weatherData.current.wind_direction = wind_direction;
            _weatherData.current.humidity = humidity;
            _weatherData.current.cloud_cover = cloud_cover;
            _weatherData.current.weather_code = weather_code;
            _weatherData.current.last_update = millis();
            _weatherData.current.valid = true;
            _weatherData.current.error = "";
        } else {
            _weatherData.current.valid = false;
            _weatherData.current.error = error;
        }
    }
}
