#include "../../../app_config.h"
#include "../WiFiManager.h"
#include <LittleFS.h>


void WiFiManager::handleWifiSTAForget() {
    // The actual forgetting will be handled by the main application
    _server->send(200, "text/plain", "WiFi credentials cleared");
}

void WiFiManager::handleWifiSTAScan() {
     _server->send(200, "text/html", scanNetworks());
}


String WiFiManager::scanNetworks() {
    String networks = "[";
    
    int n = WiFi.scanNetworks();
    for (int i = 0; i < n; ++i) {
        if (i > 0) {
            networks += ",";
        }
        networks += "{\"ssid\":\"" + WiFi.SSID(i) + "\",\"rssi\":" + WiFi.RSSI(i) + ",\"secure\":" + (WiFi.encryptionType(i) == ENC_TYPE_NONE ? "false" : "true") + "}";
    }
    networks += "]";
    
    return networks;
}

void WiFiManager::setupSTA(const char* ssid, const char* password) {
    // Only connect if both ssid and password are provided
    if (ssid && password && strlen(ssid) > 0 && strlen(password) > 0) {
        WiFi.begin(ssid, password);

        // Set LED to fast blink while connecting
        _ledPattern = 2; // Fast blink

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

            // Set LED to solid on when connected
            _ledPattern = 3; // On
        } else {
            Serial.println("");
            Serial.println("STA Connection failed!");

            // Set LED to slow blink when connection fails
            _ledPattern = 1; // Slow blink
        }
    } else {
        // No credentials provided, set LED to slow blink
        _ledPattern = 1; // Slow blink
    }
}

void WiFiManager::updateBuiltinLED() {
    if (!_ledController) return;
    
    unsigned long currentTime = millis();
    
    switch (_ledPattern) {
        case 0: // Off
            digitalWrite(BUILTIN_LED_PIN, HIGH); // Turn off built-in LED (active low)
            break;
        case 1: // Slow blink (5s interval)
            if (currentTime - _lastLEDToggle >= 5000) {
                _ledState = !_ledState;
                digitalWrite(BUILTIN_LED_PIN, _ledState ? LOW : HIGH); // Active low
                _lastLEDToggle = currentTime;
            }
            break;
        case 2: // Fast blink (4Hz)
            if (currentTime - _lastLEDToggle >= 250) { // 250ms = 4Hz
                _ledState = !_ledState;
                digitalWrite(BUILTIN_LED_PIN, _ledState ? LOW : HIGH); // Active low
                _lastLEDToggle = currentTime;
            }
            break;
        case 3: // On
            digitalWrite(BUILTIN_LED_PIN, LOW); // Turn on built-in LED (active low)
            break;
    }
}

