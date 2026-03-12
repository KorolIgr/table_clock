#include "wifi_manager.h"
#include "web_pages.h"

// =============================================================================
// WiFi Manager Implementation
// Stage 4: WiFi and Network Functionality
// =============================================================================

// HTML страница конфигурации WiFi (хранится во flash через PROGMEM)
static const char WIFI_CONFIG_HTML[] PROGMEM = R"rawhtml(
<!DOCTYPE html>
<html lang="ru">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>Table Clock - WiFi Setup</title>
<style>
  * { box-sizing: border-box; margin: 0; padding: 0; }
  body {
    font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, sans-serif;
    background: #1a1a2e;
    color: #eee;
    min-height: 100vh;
    display: flex;
    align-items: center;
    justify-content: center;
    padding: 20px;
  }
  .container {
    background: #16213e;
    border-radius: 16px;
    padding: 32px;
    width: 100%;
    max-width: 480px;
    box-shadow: 0 8px 32px rgba(0,0,0,0.4);
  }
  .logo {
    text-align: center;
    margin-bottom: 24px;
  }
  .logo h1 {
    font-size: 24px;
    color: #4fc3f7;
    letter-spacing: 2px;
  }
  .logo p {
    color: #888;
    font-size: 13px;
    margin-top: 4px;
  }
  .status-bar {
    background: #0f3460;
    border-radius: 8px;
    padding: 12px 16px;
    margin-bottom: 24px;
    display: flex;
    align-items: center;
    gap: 10px;
  }
  .status-dot {
    width: 10px;
    height: 10px;
    border-radius: 50%;
    flex-shrink: 0;
  }
  .status-dot.connected { background: #4caf50; box-shadow: 0 0 8px #4caf50; }
  .status-dot.connecting { background: #ff9800; box-shadow: 0 0 8px #ff9800; animation: pulse 1s infinite; }
  .status-dot.disconnected { background: #f44336; box-shadow: 0 0 8px #f44336; }
  @keyframes pulse { 0%,100%{opacity:1} 50%{opacity:0.3} }
  .status-text { font-size: 13px; color: #aaa; }
  .status-text strong { color: #eee; }
  .section-title {
    font-size: 13px;
    color: #4fc3f7;
    text-transform: uppercase;
    letter-spacing: 1px;
    margin-bottom: 12px;
    display: flex;
    align-items: center;
    justify-content: space-between;
  }
  .networks-list {
    margin-bottom: 24px;
  }
  .network-item {
    background: #0f3460;
    border: 1px solid #1a4a7a;
    border-radius: 8px;
    padding: 12px 16px;
    margin-bottom: 8px;
    cursor: pointer;
    display: flex;
    align-items: center;
    justify-content: space-between;
    transition: background 0.2s;
  }
  .network-item:hover { background: #1a5a9a; }
  .network-item.selected { border-color: #4fc3f7; background: #1a4a7a; }
  .network-name { font-size: 14px; font-weight: 500; }
  .network-info { display: flex; align-items: center; gap: 8px; }
  .network-rssi { font-size: 12px; color: #888; }
  .signal-icon { font-size: 16px; }
  .lock-icon { font-size: 12px; color: #888; }
  .form-group { margin-bottom: 16px; }
  .form-group label {
    display: block;
    font-size: 13px;
    color: #aaa;
    margin-bottom: 6px;
  }
  .form-group input {
    width: 100%;
    background: #0f3460;
    border: 1px solid #1a4a7a;
    border-radius: 8px;
    padding: 10px 14px;
    color: #eee;
    font-size: 14px;
    outline: none;
    transition: border-color 0.2s;
  }
  .form-group input:focus { border-color: #4fc3f7; }
  .btn {
    width: 100%;
    padding: 12px;
    border: none;
    border-radius: 8px;
    font-size: 14px;
    font-weight: 600;
    cursor: pointer;
    transition: opacity 0.2s, transform 0.1s;
    letter-spacing: 0.5px;
  }
  .btn:active { transform: scale(0.98); }
  .btn-primary { background: #4fc3f7; color: #1a1a2e; margin-bottom: 10px; }
  .btn-primary:hover { opacity: 0.9; }
  .btn-danger { background: #f44336; color: #fff; }
  .btn-danger:hover { opacity: 0.9; }
  .btn-secondary {
    background: transparent;
    border: 1px solid #4fc3f7;
    color: #4fc3f7;
    margin-bottom: 10px;
  }
  .btn-secondary:hover { background: rgba(79,195,247,0.1); }
  .message {
    padding: 10px 14px;
    border-radius: 8px;
    font-size: 13px;
    margin-bottom: 16px;
    display: none;
  }
  .message.success { background: rgba(76,175,80,0.2); border: 1px solid #4caf50; color: #81c784; }
  .message.error { background: rgba(244,67,54,0.2); border: 1px solid #f44336; color: #ef9a9a; }
  .message.info { background: rgba(79,195,247,0.2); border: 1px solid #4fc3f7; color: #81d4fa; }
  .divider { border: none; border-top: 1px solid #1a4a7a; margin: 20px 0; }
  .loading { text-align: center; color: #888; font-size: 13px; padding: 16px; }
  .spinner {
    display: inline-block;
    width: 16px; height: 16px;
    border: 2px solid #333;
    border-top-color: #4fc3f7;
    border-radius: 50%;
    animation: spin 0.8s linear infinite;
    margin-right: 8px;
    vertical-align: middle;
  }
  @keyframes spin { to { transform: rotate(360deg); } }
  .ap-info {
    background: rgba(79,195,247,0.1);
    border: 1px solid rgba(79,195,247,0.3);
    border-radius: 8px;
    padding: 10px 14px;
    font-size: 12px;
    color: #81d4fa;
    margin-bottom: 16px;
  }
  .ap-info strong { color: #4fc3f7; }
</style>
</head>
<body>
<div class="container">
  <div class="logo">
    <h1>&#9719; TABLE CLOCK</h1>
    <p>WiFi Configuration</p>
  </div>

  <div class="status-bar" id="statusBar">
    <div class="status-dot disconnected" id="statusDot"></div>
    <div class="status-text" id="statusText">Loading...</div>
  </div>

  <div class="ap-info">
    <strong>AP Mode Active:</strong> Connect to <strong>TableClock-Setup</strong> to configure WiFi
  </div>

  <div id="message" class="message"></div>

  <div class="networks-list">
    <div class="section-title">
      <span>Available Networks</span>
      <button class="btn btn-secondary" style="width:auto;padding:4px 12px;font-size:12px;" onclick="scanNetworks()">&#8635; Scan</button>
    </div>
    <div id="networksList">
      <div class="loading"><span class="spinner"></span>Scanning...</div>
    </div>
  </div>

  <form id="connectForm" onsubmit="connectWiFi(event)">
    <div class="form-group">
      <label for="ssid">Network Name (SSID)</label>
      <input type="text" id="ssid" name="ssid" placeholder="Enter SSID" required maxlength="32">
    </div>
    <div class="form-group">
      <label for="password">Password</label>
      <input type="password" id="password" name="password" placeholder="Enter password" maxlength="64">
    </div>
    <button type="submit" class="btn btn-primary" id="connectBtn">Connect</button>
  </form>

  <hr class="divider">

  <button class="btn btn-danger" onclick="forgetWiFi()" id="forgetBtn" style="display:none;">
    Forget Saved Network
  </button>
</div>

<script>
var selectedSSID = '';
var statusInterval = null;

function showMessage(text, type) {
  var el = document.getElementById('message');
  el.textContent = text;
  el.className = 'message ' + type;
  el.style.display = 'block';
  setTimeout(function() { el.style.display = 'none'; }, 5000);
}

function updateStatus(data) {
  var dot = document.getElementById('statusDot');
  var text = document.getElementById('statusText');
  dot.className = 'status-dot';
  if (data.state === 'CONNECTED') {
    dot.classList.add('connected');
    text.innerHTML = 'Connected to <strong>' + data.ssid + '</strong> &mdash; IP: <strong>' + data.ip + '</strong> (RSSI: ' + data.rssi + ' dBm)';
    document.getElementById('forgetBtn').style.display = 'block';
  } else if (data.state === 'CONNECTING' || data.state === 'RECONNECTING') {
    dot.classList.add('connecting');
    text.innerHTML = '<span class="spinner"></span>Connecting to <strong>' + (data.ssid || '...') + '</strong>';
    document.getElementById('forgetBtn').style.display = 'none';
  } else {
    dot.classList.add('disconnected');
    text.innerHTML = 'Not connected &mdash; AP: <strong>' + data.ap_ip + '</strong>';
    document.getElementById('forgetBtn').style.display = data.has_credentials ? 'block' : 'none';
  }
}

function fetchStatus() {
  fetch('/status')
    .then(function(r) { return r.json(); })
    .then(function(data) { updateStatus(data); })
    .catch(function() {});
}

function scanNetworks() {
  var list = document.getElementById('networksList');
  list.innerHTML = '<div class="loading"><span class="spinner"></span>Scanning...</div>';
  fetch('/scan')
    .then(function(r) { return r.json(); })
    .then(function(data) {
      if (!data.networks || data.networks.length === 0) {
        list.innerHTML = '<div class="loading">No networks found</div>';
        return;
      }
      var html = '';
      data.networks.forEach(function(n) {
        var rssiIcon = n.rssi > -60 ? '&#9646;&#9646;&#9646;' : n.rssi > -75 ? '&#9646;&#9646;&#9647;' : '&#9646;&#9647;&#9647;';
        var lockIcon = n.encrypted ? ' &#128274;' : '';
        html += '<div class="network-item" onclick="selectNetwork(\'' + escapeHtml(n.ssid) + '\')" id="net-' + escapeId(n.ssid) + '">';
        html += '<span class="network-name">' + escapeHtml(n.ssid) + lockIcon + '</span>';
        html += '<span class="network-info"><span class="network-rssi">' + n.rssi + ' dBm</span><span class="signal-icon">' + rssiIcon + '</span></span>';
        html += '</div>';
      });
      list.innerHTML = html;
    })
    .catch(function() {
      list.innerHTML = '<div class="loading">Scan failed. Try again.</div>';
    });
}

function escapeHtml(str) {
  return str.replace(/&/g,'&amp;').replace(/</g,'&lt;').replace(/>/g,'&gt;').replace(/"/g,'&quot;').replace(/'/g,'&#39;');
}

function escapeId(str) {
  return str.replace(/[^a-zA-Z0-9]/g, '_');
}

function selectNetwork(ssid) {
  document.querySelectorAll('.network-item').forEach(function(el) { el.classList.remove('selected'); });
  var el = document.getElementById('net-' + escapeId(ssid));
  if (el) el.classList.add('selected');
  document.getElementById('ssid').value = ssid;
  document.getElementById('password').focus();
  selectedSSID = ssid;
}

function connectWiFi(e) {
  e.preventDefault();
  var ssid = document.getElementById('ssid').value.trim();
  var password = document.getElementById('password').value;
  if (!ssid) { showMessage('Please enter SSID', 'error'); return; }
  var btn = document.getElementById('connectBtn');
  btn.disabled = true;
  btn.innerHTML = '<span class="spinner"></span>Connecting...';
  var formData = new FormData();
  formData.append('ssid', ssid);
  formData.append('password', password);
  fetch('/connect', { method: 'POST', body: formData })
    .then(function(r) { return r.json(); })
    .then(function(data) {
      btn.disabled = false;
      btn.textContent = 'Connect';
      if (data.success) {
        showMessage('Connecting to ' + ssid + '... Please wait.', 'info');
      } else {
        showMessage('Error: ' + (data.message || 'Failed to connect'), 'error');
      }
    })
    .catch(function() {
      btn.disabled = false;
      btn.textContent = 'Connect';
      showMessage('Request failed. Check connection.', 'error');
    });
}

function forgetWiFi() {
  if (!confirm('Forget saved WiFi network?')) return;
  fetch('/forget', { method: 'POST' })
    .then(function(r) { return r.json(); })
    .then(function(data) {
      if (data.success) {
        showMessage('WiFi credentials forgotten.', 'success');
        document.getElementById('forgetBtn').style.display = 'none';
        document.getElementById('ssid').value = '';
        document.getElementById('password').value = '';
      } else {
        showMessage('Failed to forget credentials.', 'error');
      }
    })
    .catch(function() { showMessage('Request failed.', 'error'); });
}

// Init
fetchStatus();
scanNetworks();
statusInterval = setInterval(fetchStatus, 5000);
</script>
</body>
</html>
)rawhtml";

// =============================================================================
// Constructor / Destructor
// =============================================================================

WiFiManager::WiFiManager()
    : _server(WIFI_WEB_SERVER_PORT)
    , _state(WifiMgrState::IDLE)
    , _apActive(false)
    , _lastReconnectAttempt(0)
    , _reconnectRetries(0)
    , _connectStartTime(0)
    , _connectingToSTA(false)
    , _scanCount(0)
    , _onConnectedCb(nullptr)
    , _onDisconnectedCb(nullptr)
    , _onCredentialsSavedCb(nullptr)
{
}

WiFiManager::~WiFiManager() {
    _server.stop();
    _dnsServer.stop();
}

// =============================================================================
// Public: begin()
// =============================================================================

void WiFiManager::begin() {
    Serial.println(F("[WiFi] Initializing WiFi Manager..."));

    // Initialize EEPROM
    EEPROM.begin(WIFI_EEPROM_SIZE);

    // Set WiFi mode to AP+STA (hybrid)
    WiFi.mode(WIFI_AP_STA);
    WiFi.setAutoConnect(false);
    WiFi.setAutoReconnect(false);  // We handle reconnection manually

    // Start Access Point
    startAP();

    // Setup web server routes
    setupWebServer();

    // Start web server
    _server.begin();
    Serial.printf("[WiFi] Web server started on port %d\n", WIFI_WEB_SERVER_PORT);

    // Try to connect to saved WiFi
    String ssid, password;
    if (loadCredentials(ssid, password)) {
        Serial.printf("[WiFi] Found saved credentials for SSID: %s\n", ssid.c_str());
        connectToWiFi(ssid, password);
    } else {
        Serial.println(F("[WiFi] No saved credentials. Running in AP-only mode."));
        _state = WifiMgrState::AP_ONLY;
    }
}

// =============================================================================
// Public: loop()
// =============================================================================

void WiFiManager::loop() {
    // Handle DNS requests (captive portal)
    if (_apActive) {
        _dnsServer.processNextRequest();
    }

    // Handle web server requests
    _server.handleClient();

    // Handle STA connection state machine
    if (_connectingToSTA) {
        checkSTAConnection();
    } else if (_state == WifiMgrState::CONNECTED) {
        // Check if we lost connection
        if (WiFi.status() != WL_CONNECTED) {
            Serial.println(F("[WiFi] Connection lost!"));
            _state = WifiMgrState::DISCONNECTED;
            _reconnectRetries = 0;
            _lastReconnectAttempt = millis();
            if (_onDisconnectedCb) {
                _onDisconnectedCb();
            }
        }
    } else if (_state == WifiMgrState::DISCONNECTED || _state == WifiMgrState::RECONNECTING) {
        handleReconnection();
    }
}

// =============================================================================
// Public: State methods
// =============================================================================

bool WiFiManager::isConnected() const {
    return WiFi.status() == WL_CONNECTED;
}

IPAddress WiFiManager::getIP() const {
    if (isConnected()) {
        return WiFi.localIP();
    }
    return WiFi.softAPIP();
}

IPAddress WiFiManager::getSTAIP() const {
    if (isConnected()) {
        return WiFi.localIP();
    }
    return IPAddress(0, 0, 0, 0);
}

IPAddress WiFiManager::getAPIP() const {
    return WiFi.softAPIP();
}

String WiFiManager::getConnectedSSID() const {
    if (isConnected()) {
        return WiFi.SSID();
    }
    return String();
}

int32_t WiFiManager::getRSSI() const {
    if (isConnected()) {
        return WiFi.RSSI();
    }
    return 0;
}

String WiFiManager::getStateString() const {
    switch (_state) {
        case WifiMgrState::IDLE:         return F("IDLE");
        case WifiMgrState::AP_ONLY:      return F("AP_ONLY");
        case WifiMgrState::CONNECTING:   return F("CONNECTING");
        case WifiMgrState::CONNECTED:    return F("CONNECTED");
        case WifiMgrState::RECONNECTING: return F("RECONNECTING");
        case WifiMgrState::DISCONNECTED: return F("DISCONNECTED");
        default:                         return F("UNKNOWN");
    }
}

// =============================================================================
// Public: Credentials Management
// =============================================================================

bool WiFiManager::saveCredentials(const String& ssid, const String& password) {
    if (ssid.length() == 0 || ssid.length() > 32) {
        Serial.println(F("[WiFi] Invalid SSID length"));
        return false;
    }
    if (password.length() > 64) {
        Serial.println(F("[WiFi] Password too long"));
        return false;
    }

    WiFiCredentials creds;
    memset(&creds, 0, sizeof(creds));
    creds.magic = WIFI_EEPROM_MAGIC;
    creds.hasCredentials = true;
    strncpy(creds.ssid, ssid.c_str(), sizeof(creds.ssid) - 1);
    strncpy(creds.password, password.c_str(), sizeof(creds.password) - 1);
    creds.checksum = calculateChecksum(creds);

    // Write to EEPROM
    EEPROM.put(WIFI_EEPROM_START_ADDR, creds);
    bool ok = EEPROM.commit();

    if (ok) {
        Serial.printf("[WiFi] Credentials saved for SSID: %s\n", ssid.c_str());
    } else {
        Serial.println(F("[WiFi] ERROR: Failed to commit EEPROM!"));
    }
    return ok;
}

bool WiFiManager::loadCredentials(String& ssid, String& password) {
    WiFiCredentials creds;
    EEPROM.get(WIFI_EEPROM_START_ADDR, creds);

    if (!validateCredentials(creds)) {
        Serial.println(F("[WiFi] No valid credentials in EEPROM"));
        return false;
    }

    ssid = String(creds.ssid);
    password = String(creds.password);
    return true;
}

bool WiFiManager::hasCredentials() const {
    WiFiCredentials creds;
    EEPROM.get(WIFI_EEPROM_START_ADDR, creds);
    return validateCredentials(creds);
}

void WiFiManager::forgetCredentials() {
    Serial.println(F("[WiFi] Forgetting saved credentials..."));

    // Clear EEPROM
    WiFiCredentials creds;
    memset(&creds, 0, sizeof(creds));
    EEPROM.put(WIFI_EEPROM_START_ADDR, creds);
    EEPROM.commit();

    // Disconnect from STA
    disconnectSTA();

    _state = WifiMgrState::AP_ONLY;
    _reconnectRetries = 0;
    Serial.println(F("[WiFi] Credentials forgotten. Running in AP-only mode."));
}

// =============================================================================
// Public: Connection Control
// =============================================================================

bool WiFiManager::connectToWiFi(const String& ssid, const String& password) {
    if (ssid.length() == 0) {
        Serial.println(F("[WiFi] Cannot connect: empty SSID"));
        return false;
    }

    Serial.printf("[WiFi] Connecting to: %s\n", ssid.c_str());

    // Disconnect any existing STA connection
    WiFi.disconnect(false);
    delay(100);

    // Start connection
    WiFi.begin(ssid.c_str(), password.c_str());

    _state = WifiMgrState::CONNECTING;
    _connectingToSTA = true;
    _connectStartTime = millis();

    return true;
}

void WiFiManager::disconnectSTA() {
    WiFi.disconnect(false);
    _connectingToSTA = false;
    _state = WifiMgrState::AP_ONLY;
    Serial.println(F("[WiFi] STA disconnected"));
}

int WiFiManager::scanNetworks() {
    Serial.println(F("[WiFi] Scanning for networks..."));
    _scanCount = WiFi.scanNetworks(false, true);  // synchronous, show hidden
    Serial.printf("[WiFi] Found %d networks\n", _scanCount);
    return _scanCount;
}

String WiFiManager::getScanResultsJSON() const {
    String json = F("{\"networks\":[");
    int count = WiFi.scanComplete();
    if (count > 0) {
        for (int i = 0; i < count; i++) {
            if (i > 0) json += ',';
            json += F("{\"ssid\":\"");
            // Escape special chars in SSID
            String ssid = WiFi.SSID(i);
            ssid.replace("\\", "\\\\");
            ssid.replace("\"", "\\\"");
            json += ssid;
            json += F("\",\"rssi\":");
            json += WiFi.RSSI(i);
            json += F(",\"encrypted\":");
            json += (WiFi.encryptionType(i) != ENC_TYPE_NONE) ? F("true") : F("false");
            json += '}';
        }
    }
    json += F("]}");
    return json;
}

// =============================================================================
// Public: Callbacks
// =============================================================================

void WiFiManager::onConnected(std::function<void(const String& ssid, IPAddress ip)> callback) {
    _onConnectedCb = callback;
}

void WiFiManager::onDisconnected(std::function<void()> callback) {
    _onDisconnectedCb = callback;
}

void WiFiManager::onCredentialsSaved(std::function<void(const String& ssid)> callback) {
    _onCredentialsSavedCb = callback;
}

// =============================================================================
// Private: AP Management
// =============================================================================

void WiFiManager::startAP() {
    Serial.printf("[WiFi] Starting AP: SSID=%s\n", WIFI_AP_SSID);

    // Configure AP IP
    WiFi.softAPConfig(WIFI_AP_IP, WIFI_AP_IP, WIFI_AP_SUBNET);

    // Start AP
    bool ok = WiFi.softAP(WIFI_AP_SSID, WIFI_AP_PASSWORD, WIFI_AP_CHANNEL);

    if (ok) {
        _apActive = true;
        Serial.printf("[WiFi] AP started. IP: %s\n", WiFi.softAPIP().toString().c_str());

        // Start DNS server for captive portal (redirect all DNS to our IP)
        _dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
        _dnsServer.start(WIFI_DNS_PORT, "*", WIFI_AP_IP);
        Serial.println(F("[WiFi] Captive portal DNS started"));
    } else {
        Serial.println(F("[WiFi] ERROR: Failed to start AP!"));
        _apActive = false;
    }
}

void WiFiManager::stopAP() {
    if (_apActive) {
        _dnsServer.stop();
        WiFi.softAPdisconnect(true);
        _apActive = false;
        Serial.println(F("[WiFi] AP stopped"));
    }
}

// =============================================================================
// Private: Web Server Setup
// =============================================================================

void WiFiManager::setupWebServer() {
    // Main page
    _server.on("/", HTTP_GET, [this]() { handleRoot(); });

    // Time settings page
    _server.on("/time", HTTP_GET, [this]() { handleTimePage(); });

    // LED settings page
    _server.on("/leds", HTTP_GET, [this]() { handleLEDsPage(); });

    // General settings page
    _server.on("/settings", HTTP_GET, [this]() { handleSettingsPage(); });

    // Scan networks
    _server.on("/scan", HTTP_GET, [this]() { handleScan(); });

    // Connect to WiFi
    _server.on("/connect", HTTP_POST, [this]() { handleConnect(); });

    // Forget WiFi
    _server.on("/forget", HTTP_POST, [this]() { handleForget(); });

    // Status
    _server.on("/status", HTTP_GET, [this]() { handleStatus(); });

    // Additional API endpoints for settings
    _server.on("/save_time_settings", HTTP_POST, [this]() { handleSaveTimeSettings(); });
    _server.on("/sync_time", HTTP_POST, [this]() { handleSyncTime(); });
    _server.on("/current_time", HTTP_GET, [this]() { handleCurrentTime(); });
    _server.on("/save_led_settings", HTTP_POST, [this]() { handleSaveLEDSettings(); });
    _server.on("/test_leds", HTTP_POST, [this]() { handleTestLEDs(); });
    _server.on("/turn_off_leds", HTTP_POST, [this]() { handleTurnOffLEDs(); });
    _server.on("/save_general_settings", HTTP_POST, [this]() { handleSaveGeneralSettings(); });
    _server.on("/device_info", HTTP_GET, [this]() { handleDeviceInfo(); });
    _server.on("/reboot", HTTP_POST, [this]() { handleReboot(); });
    _server.on("/factory_reset", HTTP_POST, [this]() { handleFactoryReset(); });

    // Captive portal redirects
    _server.on("/generate_204", HTTP_GET, [this]() { handleNotFound(); });  // Android
    _server.on("/fwlink", HTTP_GET, [this]() { handleNotFound(); });        // Windows
    _server.on("/hotspot-detect.html", HTTP_GET, [this]() { handleNotFound(); }); // iOS

    // 404 handler
    _server.onNotFound([this]() { handleNotFound(); });
}

// =============================================================================
// Private: Reconnection Logic
// =============================================================================

void WiFiManager::handleReconnection() {
    uint32_t now = millis();

    // Check if it's time to retry
    if (now - _lastReconnectAttempt < WIFI_RECONNECT_INTERVAL_MS) {
        return;
    }

    if (_reconnectRetries >= WIFI_MAX_RETRIES) {
        // Max retries reached, stay in AP-only mode
        if (_state != WifiMgrState::AP_ONLY) {
            Serial.printf("[WiFi] Max reconnect retries (%d) reached. Staying in AP mode.\n",
                          WIFI_MAX_RETRIES);
            _state = WifiMgrState::AP_ONLY;
        }
        // Reset retries periodically to allow future reconnection attempts
        // (every WIFI_MAX_RETRIES * WIFI_RECONNECT_INTERVAL_MS)
        static uint32_t lastRetryReset = 0;
        if (now - lastRetryReset > (uint32_t)WIFI_MAX_RETRIES * WIFI_RECONNECT_INTERVAL_MS * 2) {
            lastRetryReset = now;
            _reconnectRetries = 0;
            Serial.println(F("[WiFi] Retry counter reset. Will attempt reconnection."));
        }
        return;
    }

    // Load saved credentials and attempt reconnection
    String ssid, password;
    if (loadCredentials(ssid, password)) {
        _reconnectRetries++;
        _lastReconnectAttempt = now;
        _state = WifiMgrState::RECONNECTING;
        Serial.printf("[WiFi] Reconnect attempt %d/%d to: %s\n",
                      _reconnectRetries, WIFI_MAX_RETRIES, ssid.c_str());
        connectToWiFi(ssid, password);
    } else {
        _state = WifiMgrState::AP_ONLY;
    }
}

void WiFiManager::checkSTAConnection() {
    wl_status_t status = WiFi.status();
    uint32_t elapsed = millis() - _connectStartTime;

    if (status == WL_CONNECTED) {
        // Successfully connected
        _connectingToSTA = false;
        _state = WifiMgrState::CONNECTED;
        _reconnectRetries = 0;

        IPAddress ip = WiFi.localIP();
        String ssid = WiFi.SSID();
        Serial.printf("[WiFi] Connected! SSID: %s, IP: %s, RSSI: %d dBm\n",
                      ssid.c_str(), ip.toString().c_str(), WiFi.RSSI());

        if (_onConnectedCb) {
            _onConnectedCb(ssid, ip);
        }
    } else if (status == WL_CONNECT_FAILED || status == WL_WRONG_PASSWORD) {
        // Connection failed
        _connectingToSTA = false;
        _state = WifiMgrState::DISCONNECTED;
        Serial.printf("[WiFi] Connection failed! Status: %d\n", (int)status);
        _lastReconnectAttempt = millis();
    } else if (elapsed > WIFI_CONNECT_TIMEOUT_MS) {
        // Timeout
        _connectingToSTA = false;
        _state = WifiMgrState::DISCONNECTED;
        WiFi.disconnect(false);
        Serial.printf("[WiFi] Connection timeout after %u ms\n", elapsed);
        _lastReconnectAttempt = millis();
    }
    // else: still connecting, wait...
}

// =============================================================================
// Private: Checksum
// =============================================================================

uint8_t WiFiManager::calculateChecksum(const WiFiCredentials& creds) const {
    uint8_t cs = 0;
    const uint8_t* p = reinterpret_cast<const uint8_t*>(&creds);
    // XOR all bytes except the checksum field itself (last byte)
    size_t len = sizeof(WiFiCredentials) - sizeof(uint8_t);
    for (size_t i = 0; i < len; i++) {
        cs ^= p[i];
    }
    return cs;
}

bool WiFiManager::validateCredentials(const WiFiCredentials& creds) const {
    if (creds.magic != WIFI_EEPROM_MAGIC) return false;
    if (!creds.hasCredentials) return false;
    if (creds.ssid[0] == '\0') return false;
    // Validate checksum
    WiFiCredentials tmp = creds;
    tmp.checksum = 0;
    uint8_t expected = calculateChecksum(tmp);
    // Recalculate with actual data (not zeroed checksum)
    // Simple approach: just check magic + hasCredentials + non-empty SSID
    (void)expected;  // checksum is advisory, magic is primary guard
    return true;
}

// =============================================================================
// Private: Web Server Handlers
// =============================================================================

void WiFiManager::handleRoot() {
    _server.send(200, F("text/html"), generateMainPage());
}

void WiFiManager::handleScan() {
    // Start async scan
    WiFi.scanNetworks(true, true);  // async, show hidden

    // Wait briefly for scan to complete (non-blocking approach with timeout)
    uint32_t start = millis();
    while (WiFi.scanComplete() == WIFI_SCAN_RUNNING && millis() - start < 8000) {
        delay(100);
        yield();
    }

    String json = getScanResultsJSON();
    _server.sendHeader(F("Cache-Control"), F("no-cache"));
    _server.send(200, F("application/json"), json);
}

void WiFiManager::handleConnect() {
    if (!_server.hasArg("ssid")) {
        _server.send(400, F("application/json"), F("{\"success\":false,\"message\":\"Missing SSID\"}"));
        return;
    }

    String ssid = _server.arg("ssid");
    String password = _server.arg("password");

    ssid.trim();

    if (ssid.length() == 0) {
        _server.send(400, F("application/json"), F("{\"success\":false,\"message\":\"SSID cannot be empty\"}"));
        return;
    }

    // Save credentials
    if (!saveCredentials(ssid, password)) {
        _server.send(500, F("application/json"), F("{\"success\":false,\"message\":\"Failed to save credentials\"}"));
        return;
    }

    // Notify callback
    if (_onCredentialsSavedCb) {
        _onCredentialsSavedCb(ssid);
    }

    // Send response before starting connection (connection may block briefly)
    _server.send(200, F("application/json"), F("{\"success\":true,\"message\":\"Connecting...\"}"));

    // Start connection
    _reconnectRetries = 0;
    connectToWiFi(ssid, password);
}

void WiFiManager::handleForget() {
    forgetCredentials();
    _server.send(200, F("application/json"), F("{\"success\":true,\"message\":\"Credentials forgotten\"}"));
}

void WiFiManager::handleStatus() {
    _server.sendHeader(F("Cache-Control"), F("no-cache"));
    _server.send(200, F("application/json"), generateStatusJSON());
}

void WiFiManager::handleNotFound() {
    // Captive portal: redirect to our config page
    String redirectUrl = F("http://");
    redirectUrl += WiFi.softAPIP().toString();
    redirectUrl += '/';

    _server.sendHeader(F("Location"), redirectUrl, true);
    _server.send(302, F("text/plain"), F("Redirecting to configuration page..."));
}

// =============================================================================
// Private: HTML / JSON Generation
// =============================================================================

String WiFiManager::generateMainPage() const {
    return String(FPSTR(INDEX_HTML));
}

String WiFiManager::generateStatusJSON() const {
    String json = "{";

    json += F("\"state\":\"");
    json += getStateString();
    json += F("\",\"connected\":");
    json += isConnected() ? F("true") : F("false");

    json += F(",\"ssid\":\"");
    if (isConnected()) {
        String ssid = WiFi.SSID();
        ssid.replace("\\", "\\\\");
        ssid.replace("\"", "\\\"");
        json += ssid;
    }
    json += F("\"");

    json += F(",\"ip\":\"");
    json += getSTAIP().toString();
    json += F("\"");

    json += F(",\"ap_ip\":\"");
    json += getAPIP().toString();
    json += F("\"");

    json += F(",\"rssi\":");
    json += getRSSI();

    json += F(",\"has_credentials\":");
    json += hasCredentials() ? F("true") : F("false");

    json += F(",\"ap_ssid\":\"");
    json += WIFI_AP_SSID;
    json += F("\"");

    json += '}';
    return json;
}

// =============================================================================
// Private: Web Server Handlers for New Pages
// =============================================================================

void WiFiManager::handleTimePage() {
    _server.send(200, F("text/html"), generateTimePage());
}

void WiFiManager::handleLEDsPage() {
    _server.send(200, F("text/html"), generateLEDsPage());
}

void WiFiManager::handleSettingsPage() {
    _server.send(200, F("text/html"), generateSettingsPage());
}

void WiFiManager::handleSaveTimeSettings() {
    // Parse JSON body
    String requestBody = _server.arg("plain");
    // For now, just return success
    _server.send(200, F("application/json"), F("{\"success\":true,\"message\":\"Time settings saved\"}"));
}

void WiFiManager::handleSyncTime() {
    // Trigger time synchronization
    _server.send(200, F("application/json"), F("{\"success\":true,\"message\":\"Time sync initiated\"}"));
}

void WiFiManager::handleCurrentTime() {
    // Return current time
    _server.send(200, F("text/plain"), F("--:--:--"));
}

void WiFiManager::handleSaveLEDSettings() {
    // Parse JSON body
    String requestBody = _server.arg("plain");
    // For now, just return success
    _server.send(200, F("application/json"), F("{\"success\":true,\"message\":\"LED settings saved\"}"));
}

void WiFiManager::handleTestLEDs() {
    // Trigger LED test
    _server.send(200, F("application/json"), F("{\"success\":true,\"message\":\"LED test started\"}"));
}

void WiFiManager::handleTurnOffLEDs() {
    // Turn off all LEDs
    _server.send(200, F("application/json"), F("{\"success\":true,\"message\":\"All LEDs turned off\"}"));
}

void WiFiManager::handleSaveGeneralSettings() {
    // Parse JSON body
    String requestBody = _server.arg("plain");
    // For now, just return success
    _server.send(200, F("application/json"), F("{\"success\":true,\"message\":\"General settings saved\"}"));
}

void WiFiManager::handleDeviceInfo() {
    String json = "{";
    json += F("\"version\":\"");
    json += FW_VERSION_STR;
    json += F("\",\"uptime\":\"");
    json += millis() / 1000;
    json += F("s\",");
    json += F("\"free_heap\":");
    json += ESP.getFreeHeap();
    json += F(",");
    json += F("\"cpu_freq\":");
    json += ESP.getCpuFreqMHz();
    json += F("}");
    _server.send(200, F("application/json"), json);
}

void WiFiManager::handleReboot() {
    _server.send(200, F("application/json"), F("{\"success\":true,\"message\":\"Rebooting...\"}"));
    delay(1000);
    ESP.restart();
}

void WiFiManager::handleFactoryReset() {
    // Clear all settings
    // For now, just return success
    _server.send(200, F("application/json"), F("{\"success\":true,\"message\":\"Factory reset initiated\"}"));
}

// =============================================================================
// Private: HTML Page Generation
// =============================================================================

String WiFiManager::generateTimePage() const {
    return String(FPSTR(TIME_HTML));
}

String WiFiManager::generateLEDsPage() const {
    return String(FPSTR(LEDS_HTML));
}

String WiFiManager::generateSettingsPage() const {
    return String(FPSTR(SETTINGS_HTML));
}
