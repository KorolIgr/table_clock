#include "WiFiSTA.h"
#include "../../app_config.h"

WiFiSTA::WiFiSTA()
    : _dataStorage(nullptr), _lastConnectedStatus(false) {
}

void WiFiSTA::setDataStorage(DataStorage* dataStorage) {
    _dataStorage = dataStorage;
}

void WiFiSTA::begin(const char* ssid, const char* password) {
    // Only connect if both ssid and password are provided and password is at least 8 chars
    if (ssid && password && strlen(ssid) > 0 && strlen(password) >= 8) {
        WiFi.begin(ssid, password);
        
        Serial.print("Connecting to STA: ");
        Serial.println(ssid);
        
        // Wait for connection with timeout
        int attempts = 0;
        const int maxAttempts = 20; // 20 * 500ms = 10 seconds timeout
        while (WiFi.status() != WL_CONNECTED && attempts < maxAttempts) {
            delay(500);
            attempts++;
        }
        
        if (WiFi.status() == WL_CONNECTED) {
            Serial.println("");
            Serial.print("STA Connected! IP address: ");
            Serial.println(WiFi.localIP());
            _lastConnectedStatus = true;
        } else {
            Serial.println("");
            Serial.println("STA Connection failed!");
            _lastConnectedStatus = false;
        }
    } else {
        // No credentials provided, don't attempt connection
        _lastConnectedStatus = false;
    }
    
    // Update DataStorage with initial status
    updateDataStorage();
}

void WiFiSTA::disconnect() {
    WiFi.disconnect();
    _lastConnectedStatus = false;
    updateDataStorage();
}

void WiFiSTA::update() {
    bool currentlyConnected = WiFi.status() == WL_CONNECTED;
    if (currentlyConnected != _lastConnectedStatus) {
        _lastConnectedStatus = currentlyConnected;
        updateDataStorage();
    }
}

bool WiFiSTA::isConnected() const {
    return _lastConnectedStatus;
}

String WiFiSTA::getIP() const {
    return WiFi.localIP().toString();
}

std::vector<NetworkInfo> WiFiSTA::scanNetworks() {
    std::vector<NetworkInfo> networks;
    
    int n = WiFi.scanNetworks();
    for (int i = 0; i < n; ++i) {
        NetworkInfo info;
        info.ssid = WiFi.SSID(i);
        info.rssi = WiFi.RSSI(i);
        info.secure = WiFi.encryptionType(i) != ENC_TYPE_NONE;
        networks.push_back(info);
    }
    
    return networks;
}

void WiFiSTA::updateDataStorage() {
    if (_dataStorage) {
        if (_lastConnectedStatus) {
            _dataStorage->updateWifiStatus(true, "Connected to STA", WiFi.localIP().toString());
        } else {
            _dataStorage->updateWifiStatus(false, "STA not connected", "");
        }
    }
}
