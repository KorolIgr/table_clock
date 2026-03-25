#ifndef GEOLOCATION_H
#define GEOLOCATION_H

#include <Arduino.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include "../data_storage/DataStorage.h"

class Geolocation {
public:
    Geolocation(DataStorage* dataStorage);
    
    void begin();
    void update();
    bool isUpdateNeeded() const;
    void forceUpdate();
    
    // Get geolocation data from storage
    String getContinent() const;
    String getCountry() const;
    String getCity() const;
    unsigned long getLastUpdate() const;
    
    // Check if we have valid geolocation data
    bool hasValidData() const;
    
private:
    DataStorage* _dataStorage;
    unsigned long _lastUpdateAttempt;
    static const unsigned long UPDATE_INTERVAL = 3600000; // 1 hour in ms
    static const unsigned long MIN_UPDATE_INTERVAL = 300000; // 5 min minimum between retries
    
    bool fetchGeolocation();
    void updateDataStorage(const String& continent, const String& country, const String& city, float latitude, float longitude);
    bool shouldAttemptUpdate() const;
};

#endif // GEOLOCATION_H
