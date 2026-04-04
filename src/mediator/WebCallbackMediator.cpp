#include "WebCallbackMediator.h"

// Static instance pointer
WebCallbackMediator* WebCallbackMediator::instance = nullptr;

WebCallbackMediator::WebCallbackMediator(WiFiSTA* wifiSTA, LEDController* ledController)
    : _wifiSTA(wifiSTA)
    , _ledController(ledController) {
    instance = this;
}

String WebCallbackMediator::getWifiScanJson() {
    if (!_wifiSTA) {
        return "[]";
    }
    auto networks = _wifiSTA->scanNetworks();
    String response = "[";
    for (size_t i = 0; i < networks.size(); ++i) {
        if (i > 0) {
            response += ",";
        }
        const auto& net = networks[i];
        response += "{\"ssid\":\"" + net.ssid + "\",\"rssi\":" + String(net.rssi) + ",\"secure\":" + (net.secure ? "true" : "false") + "}";
    }
    response += "]";
    return response;
}

String WebCallbackMediator::getLEDPatternsJson() {
    static const char* LED_PATTERN_NAMES[] = {
        "running_light",
        "ping_pong",
        "rainbow_wave",
        "chase",
        "blink"
    };
    static const int LED_PATTERN_COUNT = 5;
    
    String response = "[";
    for (int i = 0; i < LED_PATTERN_COUNT; i++) {
        if (i > 0) {
            response += ",";
        }
        response += "\"" + String(LED_PATTERN_NAMES[i]) + "\"";
    }
    response += "]";
    return response;
}

String WebCallbackMediator::getLEDCurrentJson() {
    if (!_ledController) {
        return "{}";
    }
    PatternConfig config = _ledController->getCurrentConfig();
    
    // Convert pattern enum to string
    String patternStr;
    switch (config.pattern) {
        case LEDPattern::RUNNING_LIGHT: patternStr = "running_light"; break;
        case LEDPattern::PING_PONG: patternStr = "ping_pong"; break;
        case LEDPattern::RAINBOW_WAVE: patternStr = "rainbow_wave"; break;
        case LEDPattern::CHASE: patternStr = "chase"; break;
        case LEDPattern::BLINK: patternStr = "blink"; break;
        default: patternStr = "running_light"; break;
    }
    
    // Convert color to string
    String colorStr = String(config.color.R) + "," + String(config.color.G) + "," + String(config.color.B);
    
    // Convert direction to string
    String dirStr = config.direction ? "forward" : "reverse";
    
    String response = "{";
    response += "\"pattern\":\"" + patternStr + "\",";
    response += "\"color\":\"" + colorStr + "\",";
    response += "\"speed\":" + String(config.speed) + ",";
    response += "\"direction\":\"" + dirStr + "\"";
    response += "}";
    
    return response;
}

void WebCallbackMediator::applyLEDSettings(const char* pattern, const char* color, uint16_t speed, bool direction) {
    if (!_ledController) {
        return;
    }
    
    PatternConfig config;
    
    // Parse pattern
    String patternStr = String(pattern);
    if (patternStr == "running_light") {
        config.pattern = LEDPattern::RUNNING_LIGHT;
    } else if (patternStr == "ping_pong") {
        config.pattern = LEDPattern::PING_PONG;
    } else if (patternStr == "rainbow_wave") {
        config.pattern = LEDPattern::RAINBOW_WAVE;
    } else if (patternStr == "chase") {
        config.pattern = LEDPattern::CHASE;
    } else if (patternStr == "blink") {
        config.pattern = LEDPattern::BLINK;
    } else {
        config.pattern = LEDPattern::RUNNING_LIGHT;
    }
    
    // Parse color
    int r = 20, g = 20, b = 20;
    String colorStr = String(color);
    int firstComma = colorStr.indexOf(',');
    int secondComma = colorStr.indexOf(',', firstComma + 1);
    if (firstComma > 0 && secondComma > firstComma) {
        r = colorStr.substring(0, firstComma).toInt();
        g = colorStr.substring(firstComma + 1, secondComma).toInt();
        b = colorStr.substring(secondComma + 1).toInt();
    }
    config.color = RgbColor(r, g, b);
    
    config.speed = speed;
    config.direction = direction;
    
    _ledController->setPattern(config);
}

// Static callback wrappers
String WebCallbackMediator::staticGetWifiScanJson() {
    if (instance) {
        return instance->getWifiScanJson();
    }
    return "[]";
}

String WebCallbackMediator::staticGetLEDPatternsJson() {
    if (instance) {
        return instance->getLEDPatternsJson();
    }
    return "[]";
}

String WebCallbackMediator::staticGetLEDCurrentJson() {
    if (instance) {
        return instance->getLEDCurrentJson();
    }
    return "{}";
}

void WebCallbackMediator::staticApplyLEDSettings(const char* pattern, const char* color, uint16_t speed, bool direction) {
    if (instance) {
        instance->applyLEDSettings(pattern, color, speed, direction);
    }
}

String (*WebCallbackMediator::getScanCallback())(void) {
    return &staticGetWifiScanJson;
}

String (*WebCallbackMediator::getLEDPatternsCallback())(void) {
    return &staticGetLEDPatternsJson;
}

String (*WebCallbackMediator::getLEDCurrentCallback())(void) {
    return &staticGetLEDCurrentJson;
}

void (*WebCallbackMediator::getApplyLEDSettingsCallback())(const char*, const char*, uint16_t, bool) {
    return &staticApplyLEDSettings;
}
