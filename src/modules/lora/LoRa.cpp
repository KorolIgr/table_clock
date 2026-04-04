#include "LoRa.h"
#include "../../app_config.h"
#include <SoftwareSerial.h>

LoRa::LoRa()
    : _dataStorage(nullptr), _initialized(false), _lastCommandTime(0), _loraSerial(nullptr) {
}

LoRa::~LoRa() {
    if (_loraSerial) {
        delete _loraSerial;
        _loraSerial = nullptr;
    }
}

void LoRa::setDataStorage(DataStorage* dataStorage) {
    _dataStorage = dataStorage;
}

void LoRa::begin() {
    if (_initialized) {
        return;
    }

    Serial.println("Initializing LoRa module (ASR6601) on pins D5/D6...");

    // Create SoftwareSerial instance for ASR6601 communication
    _loraSerial = new SoftwareSerial(LORA_RX_PIN, LORA_TX_PIN);
    if (!_loraSerial) {
        Serial.println("LoRa: Failed to allocate SoftwareSerial");
        return;
    }

    _loraSerial->begin(9600); // ASR6601 default baud rate
    delay(100); // Wait for module to stabilize

    sendATCommand("+++", "Entry AT", 1000);

    // Test AT communication
    if (!sendATCommand("AT", "OK", 1000)) {
        Serial.println("LoRa: Module not responding to AT command");
        delete _loraSerial;
        _loraSerial = nullptr;
        return;
    }

    Serial.println("LoRa: Module connected");

    // Configure module for LoRa mode
    //sendATCommand("AT+TEST=LORA", NULL, 1000);
    //Serial.println("LoRa: LoRa mode configured");

    // Disable continuous transmit mode
    //sendATCommand("AT+TEST=TX,0", NULL, 1000);

    sendATCommand("AT+RESET", "OK", 1000);
     _loraSerial->begin(9600); // ASR6601 default baud rate

    _initialized = true;
    _lastCommandTime = millis();
}

bool LoRa::sendData(const uint8_t* data, size_t length) {
    if (!_initialized || !_loraSerial) {
        Serial.println("LoRa: Module not initialized");
        return false;
    }

    // Convert data to hex string for AT command
    char payload[513]; // Max 510 hex chars + "AT+TEST=TX," + null terminator
    snprintf(payload, sizeof(payload), "AT+TEST=TX,\"");
    
    size_t pos = strlen("AT+TEST=TX,\"");
    for (size_t i = 0; i < length && pos < 500; i++) {
        pos += sprintf(payload + pos, "%02X", data[i]);
    }
    strcat(payload, "\"");

    return sendATCommand(payload, "OK", 3000);
}

bool LoRa::sendData(const String& data) {
    if (!_initialized || !_loraSerial) {
        Serial.println("LoRa: Module not initialized");
        return false;
    }

    // For String data, send as hex representation
    String payload = "AT+TEST=TX,\"";
    for (size_t i = 0; i < data.length(); i++) {
        char hex[3];
        sprintf(hex, "%02X", (uint8_t)data[i]);
        payload += hex;
    }
    payload += "\"";

    return sendATCommand(payload, "OK", 3000);
}


bool LoRa::sendTVOCData(float tvoc, unsigned long timestamp) {
    if (!_initialized || !_loraSerial) {
        Serial.println("LoRa: Module not initialized");
        return false;
    }

    // Format: "TVOC,<timestamp>,<tvoc>"
    String payload = "TVOC," + String(timestamp) + "," + String(tvoc, 2);
    Serial.print("LoRa: Sending TVOC data: ");
    Serial.println(payload);

    return sendData(payload);
}

void LoRa::update() {
    if (!_initialized || !_loraSerial) {
        return;
    }

    readAvailable();
}

bool LoRa::isInitialized() const {
    return _initialized;
}

bool LoRa::sendATCommand(const String& command, const char* expectedResponse, uint32_t timeout) {
    if (!_loraSerial) {
        return false;
    }

    Serial.print("LoRa AT> ");
    Serial.println(command);

    // Clear any existing buffer
    _responseBuffer = "";

    // Send command
    _loraSerial->println(command);

    uint32_t startTime = millis();
    while (millis() - startTime < timeout) {
        readAvailable();

        if (expectedResponse && _responseBuffer.indexOf(expectedResponse) != -1) {
            return true;
        }
        delay(10);
    }

    // Timeout or response not matched
    if (expectedResponse) {
        Serial.print("LoRa: Expected '");
        Serial.print(expectedResponse);
        Serial.print("' but got: ");
        Serial.println(_responseBuffer);
    }

    return false;
}

void LoRa::readAvailable() {
    if (!_loraSerial) {
        return;
    }

    while (_loraSerial->available()) {
        char c = _loraSerial->read();
        _responseBuffer += c;

        // Keep buffer from growing indefinitely
        if (_responseBuffer.length() > 256) {
            _responseBuffer = _responseBuffer.substring(_responseBuffer.length() - 128);
        }
    }
}
