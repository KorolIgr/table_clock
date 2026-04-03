#ifndef AIR_QUALITY_H
#define AIR_QUALITY_H

#include <Arduino.h>
#include <Wire.h>
#include "SparkFun_ENS160.h"
#include "SparkFun_Qwiic_Humidity_AHT20.h"
#include "../data_storage/DataStorage.h"

class AirQuality {
public:
    AirQuality(DataStorage* dataStorage);
    
    void begin(uint8_t sdaPin, uint8_t sclPin);
    void update();
    bool isInitialized() const { return _initialized; }
    
    // Sensor status check
    bool isConnected();
    
    // Get individual sensor status
    bool isAHT21Connected() const { return _aht21Connected; }
    bool isENS160Connected() const { return _ens160Connected; }
    
    // Get last error message from sensor status flags
    String getLastError() const { return _lastError; }
    
private:
    DataStorage* _dataStorage;
    bool _initialized;
    bool _aht21Connected;
    bool _ens160Connected;
    String _lastError;
    SparkFun_ENS160 _ens160;
    AHT20 _aht21;
    
    static const unsigned long UPDATE_INTERVAL = 1000; // 10 seconds
    unsigned long _lastUpdateAttempt;
    
    void readAndUpdate();
};

#endif // AIR_QUALITY_H
