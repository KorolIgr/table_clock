#ifndef LORA_H
#define LORA_H

//https://asriot.readthedocs.io/en/latest/ASR6601/Download-Tool/multi_programmer.html

#include <Arduino.h>
#include <SoftwareSerial.h>
#include "../data_storage/DataStorage.h"

class LoRa {
public:
    LoRa();
    ~LoRa();

    void setDataStorage(DataStorage* dataStorage);

    void begin();
    bool sendData(const uint8_t* data, size_t length);
    bool sendData(const String& data);
    bool sendTVOCData(float tvoc, unsigned long timestamp);
    void update();
    bool isInitialized() const;

private:
    DataStorage* _dataStorage;
    bool _initialized;
    uint32_t _lastCommandTime;
    String _responseBuffer;
    SoftwareSerial* _loraSerial;

    bool sendATCommand(const String& command, const char* expectedResponse = NULL, uint32_t timeout = 1000);
    void readAvailable();
};

#endif // LORA_H
