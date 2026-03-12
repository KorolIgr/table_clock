// Автоматически сгенерированный файл с HTML-шаблонами
#pragma once
#include <Arduino.h>

static const char INDEX_HTML[] PROGMEM = R"rawhtml(
<!DOCTYPE html>
<html lang=\"ru\">
<head>
<meta charset=\"UTF-8\">
<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">
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
  .header {
    text-align: center;
    margin-bottom: 24px;
  }
  .header h1 {
    font-size: 24px;
    color: #4fc3f7;
    letter-spacing: 2px;
  }
  .header p {
    color: #888;
    font-size: 13px;
    margin-top: 4px;
  }
  .nav {
    display: flex;
    justify-content: center;
    gap: 10px;
    margin-bottom: 24px;
  }
  .nav-btn {
    padding: 8px 16px;
    background: #0f3460;
    border: 1px solid #1a4a7a;
    border-radius: 8px;
    color: #4fc3f7;
    cursor: pointer;
    font-size: 13px;
    transition: all 0.2s;
  }
  .nav-btn:hover {
    background: #1a5a9a;
  }
  .nav-btn.active {
    background: #4fc3f7;
    color: #1a1a2e;
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
<div class=\"container\">
  <div class=\"header\">
    <h1>&#9719; TABLE CLOCK</h1>
    <p>WiFi Configuration</p>
  </div>

  <div class=\"nav\">
    <button class=\"nav-btn active\" onclick=\"navigateTo('index')\">WiFi</button>
    <button class=\"nav-btn\" onclick=\"navigateTo('time')\">Time</button>
    <button class=\"nav-btn\" onclick=\"navigateTo('leds')\">LEDs</button>
    <button class=\"nav-btn\" onclick=\"navigateTo('settings')\">Settings</button>
  </div>

  <div class=\"status-bar\" id=\"statusBar\">
    <div class=\"status-dot disconnected\" id=\"statusDot\"></div>
    <div class=\"status-text\" id=\"statusText\">Loading...</div>
  </div>

  <div class=\"ap-info\">
    <strong>AP Mode Active:</strong> Connect to <strong>TableClock-Setup</strong> to configure WiFi
  </div>

  <div id=\"message\" class=\"message\"></div>

  <div class=\"networks-list\">
    <div class=\"section-title\">
      <span>Available Networks</span>
      <button class=\"btn btn-secondary\" style=\"width:auto;padding:4px 12px;font-size:12px;\" onclick=\"scanNetworks()\">&#8635; Scan</button>
    </div>
    <div id=\"networksList\">
      <div class=\"loading\"><span class=\"spinner\"></span>Scanning...</div>
    </div>
  </div>

  <form id=\"connectForm\" onsubmit=\"connectWiFi(event)\">
    <div class=\"form-group\">
      <label for=\"ssid\">Network Name (SSID)</label>
      <input type=\"text\" id=\"ssid\" name=\"ssid\" placeholder=\"Enter SSID\" required maxlength=\"32\">
    </div>
    <div class=\"form-group\">
      <label for=\"password\">Password</label>
      <input type=\"password\" id=\"password\" name=\"password\" placeholder=\"Enter password\" maxlength=\"64\">
    </div>
    <button type=\"submit\" class=\"btn btn-primary\" id=\"connectBtn\">Connect</button>
  </form>

  <hr class=\"divider\">

  <button class=\"btn btn-danger\" onclick=\"forgetWiFi()\" id=\"forgetBtn\" style=\"display:none;\">
    Forget Saved Network
  </button>
</div>

<script>
var selectedSSID = '';
var statusInterval = null;

function navigateTo(page) {
  window.location.href = '/' + page;
}

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
    text.innerHTML = '<span class=\"spinner\"></span>Connecting to <strong>' + (data.ssid || '...') + '</strong>';
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
  list.innerHTML = '<div class=\"loading\"><span class=\"spinner\"></span>Scanning...</div>';
  fetch('/scan')
    .then(function(r) { return r.json(); })
    .then(function(data) {
      if (!data.networks || data.networks.length === 0) {
        list.innerHTML = '<div class=\"loading\">No networks found</div>';
        return;
      }
      var html = '';
      data.networks.forEach(function(n) {
        var rssiIcon = n.rssi > -60 ? '&#9646;&#9646;&#9646;' : n.rssi > -75 ? '&#9646;&#9646;&#9647;' : '&#9646;&#9647;&#9647;';
        var lockIcon = n.encrypted ? ' &#128274;' : '';
        html += '<div class=\"network-item\" onclick=\"selectNetwork(\\'' + escapeHtml(n.ssid) + '\\')\" id=\"net-' + escapeId(n.ssid) + '\">';
        html += '<span class=\"network-name\">' + escapeHtml(n.ssid) + lockIcon + '</span>';
        html += '<span class=\"network-info\"><span class=\"network-rssi\">' + n.rssi + ' dBm</span><span class=\"signal-icon\">' + rssiIcon + '</span></span>';
        html += '</div>';
      });
      list.innerHTML = html;
    })
    .catch(function() {
      list.innerHTML = '<div class=\"loading\">Scan failed. Try again.</div>';
    });
}

function escapeHtml(str) {
  return str.replace(/&/g,'&amp;').replace(/</g,'&lt;').replace(/>/g,'&gt;').replace(/\"/g,'&quot;').replace(/'/g,'&#39;');
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
  btn.innerHTML = '<span class=\"spinner\"></span>Connecting...';
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

// Update active nav button
document.addEventListener('DOMContentLoaded', function() {
  document.querySelector('.nav-btn[onclick=\"navigateTo(\\'index\\')\"]').classList.add('active');
});
</script>
</body>
</html>)rawhtml";

static const char LEDS_HTML[] PROGMEM = R"rawhtml(
<!DOCTYPE html>
<html lang=\"ru\">
<head>
<meta charset=\"UTF-8\">
<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">
<title>Table Clock - LED Settings</title>
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
  .header {
    text-align: center;
    margin-bottom: 24px;
  }
  .header h1 {
    font-size: 24px;
    color: #4fc3f7;
    letter-spacing: 2px;
  }
  .header p {
    color: #888;
    font-size: 13px;
    margin-top: 4px;
  }
  .nav {
    display: flex;
    justify-content: center;
    gap: 10px;
    margin-bottom: 24px;
  }
  .nav-btn {
    padding: 8px 16px;
    background: #0f3460;
    border: 1px solid #1a4a7a;
    border-radius: 8px;
    color: #4fc3f7;
    cursor: pointer;
    font-size: 13px;
    transition: all 0.2s;
  }
  .nav-btn:hover {
    background: #1a5a9a;
  }
  .nav-btn.active {
    background: #4fc3f7;
    color: #1a1a2e;
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
  .form-group { margin-bottom: 16px; }
  .form-group label {
    display: block;
    font-size: 13px;
    color: #aaa;
    margin-bottom: 6px;
  }
  .form-group input, .form-group select {
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
  .form-group input:focus, .form-group select:focus { border-color: #4fc3f7; }
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
  .color-picker {
    display: flex;
    align-items: center;
    gap: 10px;
  }
  .color-preview {
    width: 30px;
    height: 30px;
    border-radius: 50%;
    border: 2px solid #4fc3f7;
  }
  .brightness-control {
    display: flex;
    align-items: center;
    gap: 10px;
  }
  .brightness-slider {
    flex-grow: 1;
  }
  .led-effect {
    background: #0f3460;
    border-radius: 8px;
    padding: 12px;
    margin-bottom: 10px;
  }
</style>
</head>
<body>
<div class=\"container\">
  <div class=\"header\">
    <h1>&#9719; TABLE CLOCK</h1>
    <p>LED Settings</p>
  </div>

  <div class=\"nav\">
    <button class=\"nav-btn\" onclick=\"navigateTo('index')\">WiFi</button>
    <button class=\"nav-btn\" onclick=\"navigateTo('time')\">Time</button>
    <button class=\"nav-btn active\" onclick=\"navigateTo('leds')\">LEDs</button>
    <button class=\"nav-btn\" onclick=\"navigateTo('settings')\">Settings</button>
  </div>

  <div class=\"status-bar\" id=\"statusBar\">
    <div class=\"status-dot disconnected\" id=\"statusDot\"></div>
    <div class=\"status-text\" id=\"statusText\">Loading...</div>
  </div>

  <div id=\"message\" class=\"message\"></div>

  <div class=\"section-title\">
    <span>Brightness Control</span>
  </div>
  <div class=\"form-group brightness-control\">
    <label for=\"brightness\">Level:</label>
    <input type=\"range\" id=\"brightness\" name=\"brightness\" min=\"0\" max=\"255\" value=\"100\" class=\"brightness-slider\">
    <span id=\"brightnessValue\">100</span>
  </div>

  <div class=\"section-title\">
    <span>Color Settings</span>
  </div>
  <div class=\"form-group\">
    <label>Select Color</label>
    <div class=\"color-picker\">
      <input type=\"color\" id=\"ledColor\" name=\"ledColor\" value=\"#ffffff\">
      <div class=\"color-preview\" id=\"colorPreview\" style=\"background-color: #ffffff;\"></div>
      <span id=\"colorHex\">#ffffff</span>
    </div>
  </div>

  <div class=\"section-title\">
    <span>LED Effects</span>
  </div>
  <div class=\"led-effect\">
    <div class=\"form-group\">
      <label for=\"effectType\">Effect Type</label>
      <select id=\"effectType\" name=\"effectType\">
        <option value=\"static\">Static Color</option>
        <option value=\"rainbow\">Rainbow</option>
        <option value=\"pulse\">Pulse</option>
        <option value=\"chase\">Chase</option>
        <option value=\"fire\">Fire</option>
        <option value=\"breathe\">Breathe</option>
      </select>
    </div>
    <div class=\"form-group\">
      <label for=\"effectSpeed\">Effect Speed</label>
      <input type=\"range\" id=\"effectSpeed\" name=\"effectSpeed\" min=\"1\" max=\"10\" value=\"5\">
    </div>
  </div>

  <div class=\"section-title\">
    <span>LED Behavior</span>
  </div>
  <div class=\"form-group\">
    <label for=\"ledMode\">Mode</label>
    <select id=\"ledMode\" name=\"ledMode\">
      <option value=\"time_sync\">Synchronized with time</option>
      <option value=\"manual\">Manual control</option>
      <option value=\"auto_dim\">Auto dimming</option>
    </select>
  </div>

  <button class=\"btn btn-primary\" onclick=\"saveLEDSettings()\">Save LED Settings</button>
  <button class=\"btn btn-secondary\" onclick=\"testLEDs()\">Test LEDs</button>
  <button class=\"btn btn-danger\" onclick=\"turnOffLEDs()\">Turn Off All</button>
</div>

<script>
function navigateTo(page) {
  window.location.href = '/' + page;
}

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
  } else if (data.state === 'CONNECTING' || data.state === 'RECONNECTING') {
    dot.classList.add('connecting');
    text.innerHTML = '<span class=\"spinner\"></span>Connecting to <strong>' + (data.ssid || '...') + '</strong>';
  } else {
    dot.classList.add('disconnected');
    text.innerHTML = 'Not connected &mdash; AP: <strong>' + data.ap_ip + '</strong>';
  }
}

function fetchStatus() {
  fetch('/status')
    .then(function(r) { return r.json(); })
    .then(function(data) { updateStatus(data); })
    .catch(function() {});
}

function saveLEDSettings() {
  var brightness = document.getElementById('brightness').value;
  var ledColor = document.getElementById('ledColor').value;
  var effectType = document.getElementById('effectType').value;
  var effectSpeed = document.getElementById('effectSpeed').value;
  var ledMode = document.getElementById('ledMode').value;

  var settings = {
    brightness: parseInt(brightness),
    color: ledColor,
    effect: effectType,
    speed: parseInt(effectSpeed),
    mode: ledMode
  };

  fetch('/save_led_settings', {
    method: 'POST',
    headers: {
      'Content-Type': 'application/json'
    },
    body: JSON.stringify(settings)
  })
  .then(function(r) { return r.json(); })
  .then(function(data) {
    if (data.success) {
      showMessage('LED settings saved successfully!', 'success');
    } else {
      showMessage('Error: ' + (data.message || 'Failed to save settings'), 'error');
    }
  })
  .catch(function() {
    showMessage('Request failed. Check connection.', 'error');
  });
}

function testLEDs() {
  fetch('/test_leds', { method: 'POST' })
    .then(function(r) { return r.json(); })
    .then(function(data) {
      if (data.success) {
        showMessage('LED test started!', 'info');
      } else {
        showMessage('Error: ' + (data.message || 'Failed to start LED test'), 'error');
      }
    })
    .catch(function() {
      showMessage('Request failed. Check connection.', 'error');
    });
}

function turnOffLEDs() {
  fetch('/turn_off_leds', { method: 'POST' })
    .then(function(r) { return r.json(); })
    .then(function(data) {
      if (data.success) {
        showMessage('All LEDs turned off!', 'info');
      } else {
        showMessage('Error: ' + (data.message || 'Failed to turn off LEDs'), 'error');
      }
    })
    .catch(function() {
      showMessage('Request failed. Check connection.', 'error');
    });
}

// Update color preview when color picker changes
document.getElementById('ledColor').addEventListener('input', function() {
  var color = this.value;
  document.getElementById('colorPreview').style.backgroundColor = color;
  document.getElementById('colorHex').textContent = color;
});

// Update brightness value display
document.getElementById('brightness').addEventListener('input', function() {
  document.getElementById('brightnessValue').textContent = this.value;
});

// Update active nav button
document.addEventListener('DOMContentLoaded', function() {
  document.querySelector('.nav-btn[onclick=\"navigateTo(\\'leds\\')\"]').classList.add('active');
  
  // Load initial status
  fetchStatus();
  // Refresh status every 5 seconds
  setInterval(fetchStatus, 5000);
});
</script>
</body>
</html>)rawhtml";

static const char SETTINGS_HTML[] PROGMEM = R"rawhtml(
<!DOCTYPE html>
<html lang=\"ru\">
<head>
<meta charset=\"UTF-8\">
<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">
<title>Table Clock - General Settings</title>
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
  .header {
    text-align: center;
    margin-bottom: 24px;
  }
  .header h1 {
    font-size: 24px;
    color: #4fc3f7;
    letter-spacing: 2px;
  }
  .header p {
    color: #888;
    font-size: 13px;
    margin-top: 4px;
  }
  .nav {
    display: flex;
    justify-content: center;
    gap: 10px;
    margin-bottom: 24px;
  }
  .nav-btn {
    padding: 8px 16px;
    background: #0f3460;
    border: 1px solid #1a4a7a;
    border-radius: 8px;
    color: #4fc3f7;
    cursor: pointer;
    font-size: 13px;
    transition: all 0.2s;
  }
  .nav-btn:hover {
    background: #1a5a9a;
  }
  .nav-btn.active {
    background: #4fc3f7;
    color: #1a1a2e;
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
  .form-group { margin-bottom: 16px; }
  .form-group label {
    display: block;
    font-size: 13px;
    color: #aaa;
    margin-bottom: 6px;
  }
  .form-group input, .form-group select, .form-group textarea {
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
  .form-group input:focus, .form-group select:focus, .form-group textarea:focus { border-color: #4fc3f7; }
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
  .device-info {
    background: #0f3460;
    border-radius: 8px;
    padding: 12px;
    margin-bottom: 16px;
    font-size: 12px;
  }
  .info-row {
    display: flex;
    justify-content: space-between;
    margin-bottom: 6px;
  }
  .info-label {
    color: #aaa;
  }
  .info-value {
    color: #eee;
    font-weight: 500;
  }
</style>
</head>
<body>
<div class=\"container\">
  <div class=\"header\">
    <h1>&#9719; TABLE CLOCK</h1>
    <p>General Settings</p>
  </div>

  <div class=\"nav\">
    <button class=\"nav-btn\" onclick=\"navigateTo('index')\">WiFi</button>
    <button class=\"nav-btn\" onclick=\"navigateTo('time')\">Time</button>
    <button class=\"nav-btn\" onclick=\"navigateTo('leds')\">LEDs</button>
    <button class=\"nav-btn active\" onclick=\"navigateTo('settings')\">Settings</button>
  </div>

  <div class=\"status-bar\" id=\"statusBar\">
    <div class=\"status-dot disconnected\" id=\"statusDot\"></div>
    <div class=\"status-text\" id=\"statusText\">Loading...</div>
  </div>

  <div id=\"message\" class=\"message\"></div>

  <div class=\"section-title\">
    <span>Device Information</span>
  </div>
  <div class=\"device-info\">
    <div class=\"info-row\">
      <span class=\"info-label\">Firmware Version:</span>
      <span class=\"info-value\" id=\"firmwareVersion\">-</span>
    </div>
    <div class=\"info-row\">
      <span class=\"info-label\">Uptime:</span>
      <span class=\"info-value\" id=\"uptime\">-</span>
    </div>
    <div class=\"info-row\">
      <span class=\"info-label\">Free Heap:</span>
      <span class=\"info-value\" id=\"freeHeap\">-</span>
    </div>
    <div class=\"info-row\">
      <span class=\"info-label\">CPU Frequency:</span>
      <span class=\"info-value\" id=\"cpuFreq\">-</span>
    </div>
  </div>

  <div class=\"section-title\">
    <span>Display Settings</span>
  </div>
  <div class=\"form-group\">
    <label for=\"displayBrightness\">Display Brightness</label>
    <input type=\"range\" id=\"displayBrightness\" name=\"displayBrightness\" min=\"0\" max=\"255\" value=\"255\">
  </div>

  <div class=\"form-group\">
    <label for=\"displayTimeout\">Display Timeout (minutes, 0 = never)</label>
    <input type=\"number\" id=\"displayTimeout\" name=\"displayTimeout\" min=\"0\" max=\"120\" value=\"0\">
  </div>

  <div class=\"section-title\">
    <span>Power Management</span>
  </div>
  <div class=\"form-group\">
    <label for=\"nightModeStart\">Night Mode Start Time</label>
    <input type=\"time\" id=\"nightModeStart\" name=\"nightModeStart\" value=\"22:00\">
  </div>

  <div class=\"form-group\">
    <label for=\"nightModeEnd\">Night Mode End Time</label>
    <input type=\"time\" id=\"nightModeEnd\" name=\"nightModeEnd\" value=\"07:00\">
  </div>

  <div class=\"form-group\">
    <label for=\"nightBrightness\">Night Mode Brightness</label>
    <input type=\"range\" id=\"nightBrightness\" name=\"nightBrightness\" min=\"0\" max=\"255\" value=\"50\">
  </div>

  <div class=\"section-title\">
    <span>System Settings</span>
  </div>
  <div class=\"form-group\">
    <label for=\"deviceName\">Device Name</label>
    <input type=\"text\" id=\"deviceName\" name=\"deviceName\" placeholder=\"Table Clock\" maxlength=\"32\">
  </div>

  <button class=\"btn btn-primary\" onclick=\"saveGeneralSettings()\">Save Settings</button>
  <button class=\"btn btn-secondary\" onclick=\"rebootDevice()\">Reboot Device</button>
  <button class=\"btn btn-danger\" onclick=\"resetSettings()\">Factory Reset</button>
</div>

<script>
function navigateTo(page) {
  window.location.href = '/' + page;
}

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
    text.innerHTML = '<span class=\"spinner\"></span>Connecting to <strong>' + (data.ssid || '...') + '</strong>';
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

function updateDeviceInfo() {
  fetch('/device_info')
    .then(function(r) { return r.json(); })
    .then(function(data) {
      document.getElementById('firmwareVersion').textContent = data.version || '-';
      document.getElementById('uptime').textContent = data.uptime || '-';
      document.getElementById('freeHeap').textContent = data.free_heap ? data.free_heap + ' bytes' : '-';
      document.getElementById('cpuFreq').textContent = data.cpu_freq ? data.cpu_freq + ' MHz' : '-';
    })
    .catch(function() {
      document.getElementById('firmwareVersion').textContent = 'Error';
      document.getElementById('uptime').textContent = 'Error';
      document.getElementById('freeHeap').textContent = 'Error';
      document.getElementById('cpuFreq').textContent = 'Error';
    });
}

function saveGeneralSettings() {
  var displayBrightness = document.getElementById('displayBrightness').value;
  var displayTimeout = document.getElementById('displayTimeout').value;
  var nightModeStart = document.getElementById('nightModeStart').value;
  var nightModeEnd = document.getElementById('nightModeEnd').value;
  var nightBrightness = document.getElementById('nightBrightness').value;
  var deviceName = document.getElementById('deviceName').value;

  var settings = {
    display_brightness: parseInt(displayBrightness),
    display_timeout: parseInt(displayTimeout),
    night_mode_start: nightModeStart,
    night_mode_end: nightModeEnd,
    night_brightness: parseInt(nightBrightness),
    device_name: deviceName
  };

  fetch('/save_general_settings', {
    method: 'POST',
    headers: {
      'Content-Type': 'application/json'
    },
    body: JSON.stringify(settings)
  })
  .then(function(r) { return r.json(); })
  .then(function(data) {
    if (data.success) {
      showMessage('General settings saved successfully!', 'success');
    } else {
      showMessage('Error: ' + (data.message || 'Failed to save settings'), 'error');
    }
  })
  .catch(function() {
    showMessage('Request failed. Check connection.', 'error');
  });
}

function rebootDevice() {
  if (!confirm('Reboot the device? This will temporarily disconnect the web interface.')) return;
  
  fetch('/reboot', { method: 'POST' })
    .then(function(r) { return r.json(); })
    .then(function(data) {
      if (data.success) {
        showMessage('Device is rebooting...', 'info');
      } else {
        showMessage('Error: ' + (data.message || 'Failed to reboot device'), 'error');
      }
    })
    .catch(function() {
      showMessage('Request failed. Check connection.', 'error');
    });
}

function resetSettings() {
  if (!confirm('Perform factory reset? This will erase all settings and require reconfiguration.')) return;
  
  fetch('/factory_reset', { method: 'POST' })
    .then(function(r) { return r.json(); })
    .then(function(data) {
      if (data.success) {
        showMessage('Settings reset. Device will restart...', 'info');
      } else {
        showMessage('Error: ' + (data.message || 'Failed to reset settings'), 'error');
      }
    })
    .catch(function() {
      showMessage('Request failed. Check connection.', 'error');
    });
}

// Update active nav button
document.addEventListener('DOMContentLoaded', function() {
  document.querySelector('.nav-btn[onclick=\"navigateTo(\\'settings\\')\"]').classList.add('active');
  
  // Load initial status
  fetchStatus();
  // Refresh status every 5 seconds
  setInterval(fetchStatus, 5000);
  
  // Update device info every 10 seconds
  updateDeviceInfo();
  setInterval(updateDeviceInfo, 10000);
});
</script>
</body>
</html>)rawhtml";

static const char TEMPLATE_HTML[] PROGMEM = R"rawhtml(
<!DOCTYPE html>
<html lang=\"ru\">
<head>
<meta charset=\"UTF-8\">
<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">
<title>Table Clock - {PAGE_TITLE}</title>
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
  .header {
    text-align: center;
    margin-bottom: 24px;
  }
  .header h1 {
    font-size: 24px;
    color: #4fc3f7;
    letter-spacing: 2px;
  }
  .header p {
    color: #888;
    font-size: 13px;
    margin-top: 4px;
  }
  .nav {
    display: flex;
    justify-content: center;
    gap: 10px;
    margin-bottom: 24px;
  }
  .nav-btn {
    padding: 8px 16px;
    background: #0f3460;
    border: 1px solid #1a4a7a;
    border-radius: 8px;
    color: #4fc3f7;
    cursor: pointer;
    font-size: 13px;
    transition: all 0.2s;
  }
  .nav-btn:hover {
    background: #1a5a9a;
  }
  .nav-btn.active {
    background: #4fc3f7;
    color: #1a1a2e;
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
  .form-group { margin-bottom: 16px; }
  .form-group label {
    display: block;
    font-size: 13px;
    color: #aaa;
    margin-bottom: 6px;
  }
  .form-group input, .form-group select {
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
  .form-group input:focus, .form-group select:focus { border-color: #4fc3f7; }
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
<div class=\"container\">
  <div class=\"header\">
    <h1>&#9719; TABLE CLOCK</h1>
    <p>{PAGE_DESCRIPTION}</p>
  </div>

  <div class=\"nav\">
    <button class=\"nav-btn\" onclick=\"navigateTo('index')\">WiFi</button>
    <button class=\"nav-btn\" onclick=\"navigateTo('time')\">Time</button>
    <button class=\"nav-btn\" onclick=\"navigateTo('leds')\">LEDs</button>
    <button class=\"nav-btn\" onclick=\"navigateTo('settings')\">Settings</button>
  </div>

  <div class=\"status-bar\" id=\"statusBar\">
    <div class=\"status-dot disconnected\" id=\"statusDot\"></div>
    <div class=\"status-text\" id=\"statusText\">Loading...</div>
  </div>

  <div id=\"content\">
    <!-- Page content will be inserted here -->
    {PAGE_CONTENT}
  </div>
</div>

<script>
// Navigation function
function navigateTo(page) {
  window.location.href = '/' + page;
}

// Common functions for all pages
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
    text.innerHTML = '<span class=\"spinner\"></span>Connecting to <strong>' + (data.ssid || '...') + '</strong>';
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

// Update active nav button
document.addEventListener('DOMContentLoaded', function() {
  var currentPage = window.location.pathname.substring(1) || 'index';
  if(currentPage === '') currentPage = 'index';
  var activeBtn = document.querySelector('.nav-btn[onclick=\"navigateTo(\\'' + currentPage + '\\')\"');
  if(activeBtn) {
    activeBtn.classList.add('active');
  } else {
    // Default to index
    document.querySelector('.nav-btn[onclick=\"navigateTo(\\'index\\')\"]').classList.add('active');
  }
  
  // Load initial status
  fetchStatus();
  // Refresh status every 5 seconds
  setInterval(fetchStatus, 5000);
});
</script>
</body>
</html>)rawhtml";

static const char TIME_HTML[] PROGMEM = R"rawhtml(
<!DOCTYPE html>
<html lang=\"ru\">
<head>
<meta charset=\"UTF-8\">
<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">
<title>Table Clock - Time Settings</title>
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
  .header {
    text-align: center;
    margin-bottom: 24px;
  }
  .header h1 {
    font-size: 24px;
    color: #4fc3f7;
    letter-spacing: 2px;
  }
  .header p {
    color: #888;
    font-size: 13px;
    margin-top: 4px;
  }
  .nav {
    display: flex;
    justify-content: center;
    gap: 10px;
    margin-bottom: 24px;
  }
  .nav-btn {
    padding: 8px 16px;
    background: #0f3460;
    border: 1px solid #1a4a7a;
    border-radius: 8px;
    color: #4fc3f7;
    cursor: pointer;
    font-size: 13px;
    transition: all 0.2s;
  }
  .nav-btn:hover {
    background: #1a5a9a;
  }
  .nav-btn.active {
    background: #4fc3f7;
    color: #1a1a2e;
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
  .form-group { margin-bottom: 16px; }
  .form-group label {
    display: block;
    font-size: 13px;
    color: #aaa;
    margin-bottom: 6px;
  }
  .form-group input, .form-group select {
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
  .form-group input:focus, .form-group select:focus { border-color: #4fc3f7; }
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
  .time-display {
    background: #0f3460;
    border-radius: 8px;
    padding: 16px;
    text-align: center;
    font-family: monospace;
    font-size: 24px;
    margin-bottom: 20px;
    letter-spacing: 2px;
  }
</style>
</head>
<body>
<div class=\"container\">
  <div class=\"header\">
    <h1>&#9719; TABLE CLOCK</h1>
    <p>Time Settings</p>
  </div>

  <div class=\"nav\">
    <button class=\"nav-btn\" onclick=\"navigateTo('index')\">WiFi</button>
    <button class=\"nav-btn active\" onclick=\"navigateTo('time')\">Time</button>
    <button class=\"nav-btn\" onclick=\"navigateTo('leds')\">LEDs</button>
    <button class=\"nav-btn\" onclick=\"navigateTo('settings')\">Settings</button>
  </div>

  <div class=\"status-bar\" id=\"statusBar\">
    <div class=\"status-dot disconnected\" id=\"statusDot\"></div>
    <div class=\"status-text\" id=\"statusText\">Loading...</div>
  </div>

  <div id=\"message\" class=\"message\"></div>

  <div class=\"time-display\" id=\"currentTime\">
    --:--:--
  </div>

  <div class=\"section-title\">
    <span>Time Format</span>
  </div>
  <div class=\"form-group\">
    <label for=\"timeFormat\">Display Format</label>
    <select id=\"timeFormat\" name=\"timeFormat\">
      <option value=\"12\">12-hour format (AM/PM)</option>
      <option value=\"24\" selected>24-hour format</option>
    </select>
  </div>

  <div class=\"form-group\">
    <label for=\"dateFormat\">Date Format</label>
    <select id=\"dateFormat\" name=\"dateFormat\">
      <option value=\"dd-mm-yyyy\">DD-MM-YYYY</option>
      <option value=\"mm-dd-yyyy\">MM-DD-YYYY</option>
      <option value=\"yyyy-mm-dd\">YYYY-MM-DD</option>
    </select>
  </div>

  <div class=\"section-title\">
    <span>NTP Settings</span>
  </div>
  <div class=\"form-group\">
    <label for=\"ntpServer\">NTP Server</label>
    <input type=\"text\" id=\"ntpServer\" name=\"ntpServer\" value=\"pool.ntp.org\">
  </div>

  <div class=\"form-group\">
    <label for=\"timeZone\">Time Zone Offset (hours)</label>
    <input type=\"number\" id=\"timeZone\" name=\"timeZone\" value=\"0\" min=\"-12\" max=\"14\">
  </div>

  <button class=\"btn btn-primary\" onclick=\"saveTimeSettings()\">Save Time Settings</button>
  <button class=\"btn btn-secondary\" onclick=\"syncTime()\">Sync Now</button>
</div>

<script>
function navigateTo(page) {
  window.location.href = '/' + page;
}

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
    text.innerHTML = '<span class=\"spinner\"></span>Connecting to <strong>' + (data.ssid || '...') + '</strong>';
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

function saveTimeSettings() {
  var timeFormat = document.getElementById('timeFormat').value;
  var dateFormat = document.getElementById('dateFormat').value;
  var ntpServer = document.getElementById('ntpServer').value;
  var timeZone = document.getElementById('timeZone').value;

  var settings = {
    timeFormat: timeFormat,
    dateFormat: dateFormat,
    ntpServer: ntpServer,
    timeZone: timeZone
  };

  fetch('/save_time_settings', {
    method: 'POST',
    headers: {
      'Content-Type': 'application/json'
    },
    body: JSON.stringify(settings)
  })
  .then(function(r) { return r.json(); })
  .then(function(data) {
    if (data.success) {
      showMessage('Time settings saved successfully!', 'success');
    } else {
      showMessage('Error: ' + (data.message || 'Failed to save settings'), 'error');
    }
  })
  .catch(function() {
    showMessage('Request failed. Check connection.', 'error');
  });
}

function syncTime() {
  fetch('/sync_time', { method: 'POST' })
    .then(function(r) { return r.json(); })
    .then(function(data) {
      if (data.success) {
        showMessage('Time synchronized successfully!', 'success');
        updateTimeDisplay();
      } else {
        showMessage('Error: ' + (data.message || 'Failed to sync time'), 'error');
      }
    })
    .catch(function() {
      showMessage('Request failed. Check connection.', 'error');
    });
}

function updateTimeDisplay() {
  fetch('/current_time')
    .then(function(r) { return r.text(); })
    .then(function(time) {
      document.getElementById('currentTime').textContent = time;
    })
    .catch(function() {
      document.getElementById('currentTime').textContent = '--:--:--';
    });
}

// Update active nav button
document.addEventListener('DOMContentLoaded', function() {
  document.querySelector('.nav-btn[onclick=\"navigateTo(\\'time\\')\"]').classList.add('active');
  
  // Load initial status
  fetchStatus();
  // Refresh status every 5 seconds
  setInterval(fetchStatus, 5000);
  
  // Update time display every second
  updateTimeDisplay();
  setInterval(updateTimeDisplay, 1000);
});
</script>
</body>
</html>)rawhtml";

