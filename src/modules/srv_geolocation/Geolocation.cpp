#include "Geolocation.h"
#include <ESP8266WiFi.h>

Geolocation::Geolocation(DataStorage* dataStorage) 
    : _dataStorage(dataStorage), _lastUpdateAttempt(0) {
}

void Geolocation::begin() {
    // Initial setup - nothing to do here, will fetch on first update
    _lastUpdateAttempt = 0;
}

void Geolocation::update() {
    if (!_dataStorage) return;
    
    // Check if we need to attempt an update
    if (shouldAttemptUpdate()) {
        _lastUpdateAttempt = millis();
        fetchGeolocation();
    }
}

bool Geolocation::isUpdateNeeded() const {
    if (!_dataStorage) return false;
    
    GeoData& data = _dataStorage->geo();
    unsigned long lastUpdate = data.last_update;
    
    // If never updated, we need an update
    if (lastUpdate == 0) return true;
    
    // Check if UPDATE_INTERVAL has passed
    return (millis() - lastUpdate) >= UPDATE_INTERVAL;
}

void Geolocation::forceUpdate() {
    _lastUpdateAttempt = 0; // Reset attempt timer to force immediate fetch
}

String Geolocation::getContinent() const {
    if (!_dataStorage) return "";
    return _dataStorage->geo().continent;
}

String Geolocation::getCountry() const {
    if (!_dataStorage) return "";
    return _dataStorage->geo().country;
}

String Geolocation::getCity() const {
    if (!_dataStorage) return "";
    return _dataStorage->geo().city;
}

unsigned long Geolocation::getLastUpdate() const {
    if (!_dataStorage) return 0;
    return _dataStorage->geo().last_update;
}

bool Geolocation::hasValidData() const {
    if (!_dataStorage) return false;
    
    GeoData& data = _dataStorage->geo();
    return data.city.length() > 0 && data.country.length() > 0;
}

bool Geolocation::shouldAttemptUpdate() const {
    // Check if hourly update is needed
    if (isUpdateNeeded()) {
        return true;
    }
    
    // Also check minimum interval between attempts to avoid hammering the server
    return (millis() - _lastUpdateAttempt) >= MIN_UPDATE_INTERVAL;
}

bool Geolocation::fetchGeolocation() {
    if (!_dataStorage) return false;
    
    // Check if WiFi is connected first
    WiFiData& wifi = _dataStorage->wifi();
    if (!wifi.sta.connected) {
        Serial.println("Geolocation: WiFi not connected, skipping update");
        return false;
    }
    
    Serial.println("Geolocation: Fetching position from ipapi.org...");
    
    HTTPClient http;
    WiFiClient client;
    
    // Use ipapi.org free endpoint (https://ipapi.co/json/ or http://ip-api.com/json)
    // Using ipapi.co as it's simple and doesn't require API key for basic usage
    // Alternative: http://ip-api.com/json/ (also free, no key required)
    // Let's use ip-api.com as it's more generous with rate limits
    String url = "http://ip-api.com/json/";
    
    http.begin(client, url);
    http.addHeader("Content-Type", "application/json");
    
    int httpCode = http.GET();
    
    if (httpCode != HTTP_CODE_OK) {
        Serial.print("Geolocation: HTTP request failed, code: ");
        Serial.println(httpCode);
        http.end();
        return false;
    }
    
    String payload = http.getString();
    http.end();
    
    Serial.println("Geolocation: Response received:");
    Serial.println(payload);
    
    // Parse JSON
    // Expected response format from ip-api.com:
    // {
    //   "status": "success",
    //   "country": "Country Name",
    //   "countryCode": "CC",
    //   "region": "Region",
    //   "regionName": "Region Name",
    //   "city": "City Name",
    //   "zip": "12345",
    //   "lat": 12.3456,
    //   "lon": 78.9012,
    //   "timezone": "Timezone",
    //   "isp": "ISP Name",
    //   "org": "Organization",
    //   "as": "AS12345 ISP Name"
    // }
    
    StaticJsonDocument<1024> doc;
    DeserializationError error = deserializeJson(doc, payload);
    
    if (error) {
        Serial.print("Geolocation: JSON parse failed: ");
        Serial.println(error.c_str());
        return false;
    }
    
    const char* status = doc["status"];
    if (status && strcmp(status, "fail") == 0) {
        const char* failReason = doc["message"].as<const char*>();
        if (!failReason) failReason = "Unknown error";
        Serial.print("Geolocation: API request failed: ");
        Serial.println(failReason);
        return false;
    }
    
    const char* continent = doc["continent"]; // ip-api doesn't provide continent directly
    const char* country = doc["country"];
    const char* city = doc["city"];
    const char* ip = doc["query"];  // IP address from the query
    float lat = doc["lat"].as<float>();
    float lon = doc["lon"].as<float>();
    
    // If continent is not provided, try to derive from country code or leave empty
    String continentStr = continent ? String(continent) : "";
    String countryStr = country ? String(country) : "";
    String cityStr = city ? String(city) : "";
    String ipStr = ip ? String(ip) : "";
    
    // Update DataStorage with coordinates and IP
    updateDataStorage(continentStr, countryStr, cityStr, lat, lon, ipStr);
    
    Serial.print("Geolocation: Updated - Continent: ");
    Serial.print(continentStr);
    Serial.print(", Country: ");
    Serial.print(countryStr);
    Serial.print(", City: ");
    Serial.print(cityStr);
    Serial.print(", Lat: ");
    Serial.print(lat);
    Serial.print(", Lon: ");
    Serial.println(lon);
    
    return true;
}

void Geolocation::updateDataStorage(const String& continent, const String& country, const String& city, float latitude, float longitude, const String& ip_address) {
    if (_dataStorage) {
        _dataStorage->updateGeolocation(continent, country, city, latitude, longitude, ip_address);
    }
}
