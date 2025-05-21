#include "M5AtomS3.h"
#include <WiFi.h>
#include <SkaarhojPgmspace.h>
#include <ATEMbase.h>
#include <ATEMstd.h>
#include <WiFiUdp.h>
#include <ArduinoJson.h>

IPAddress switcherIp({ip_octet1}, {ip_octet2}, {ip_octet3}, {ip_octet4});
ATEMstd AtemSwitcher;

const char* ssid = "{ssid}";
const char* password = "{passwd}";

// UDP Communication settings
WiFiUDP udp;
const int localPort = 5001;  // Port to listen on for incoming messages
const int broadcastPort = 12001; // Port to send "I'm here" messages
IPAddress broadcastIP;       // Will be set in setup based on WiFi IP
unsigned long lastUdpSend = 0;
const int udpInterval = 2000; // Send status every 2 seconds
unsigned long lastAnnounce = 0;
const int announceInterval = 5000; // Send "I'm here" every 5 seconds
unsigned long lastButtonCheck = 0;
const int buttonCheckInterval = 100; // Check button every 100ms

// Device identification
const int deviceId = {camNumber}; // Using camera number as device ID
int currentStatus = 0; // 0-idle, 1-preview, 2-live

// Tally state tracking
int PreviewTallyPrevious = 1;
int ProgramTallyPrevious = 1;

// WiFi connection timer variables
unsigned long connectionStartTime = 0;
unsigned long elapsedTime = 0;
bool connectionTimerRunning = false;

// Function to display WiFi status/error
void displayWiFiStatus(wl_status_t status) {{
  resetStartingScreen();
  AtomS3.Display.setTextSize(0.7);
  AtomS3.Display.setTextColor(TFT_WHITE);
  // Set appropriate background color based on status
  String statusMsg;
  switch (status) {{
    case WL_NO_SSID_AVAIL:
      statusMsg = "SSID not found";
      break;
    case WL_CONNECT_FAILED:
      statusMsg = "Wrong password";
      break;
    case WL_CONNECTION_LOST:
      statusMsg = "Connection lost";
      break;
    case WL_DISCONNECTED:
      statusMsg = "Disconnected";
      break;
    case WL_IDLE_STATUS:
      statusMsg = "Connecting...";
      break;
    case WL_CONNECTED:
      statusMsg = "Connected";
      break;
    default:
      statusMsg = "Status code: " + String(status);
      break;
  }}

  AtomS3.Display.drawString(statusMsg, AtomS3.Display.width() / 2, AtomS3.Display.height() / 10);
}}

// Function to send device status via UDP
void sendStatusUpdate() {{
  // Create JSON message with device ID and status
  StaticJsonDocument<128> doc;
  doc["device_id"] = deviceId;
  doc["status"] = currentStatus;
  
  char jsonBuffer[128];
  serializeJson(doc, jsonBuffer);
  
  // Send to broadcast address
  udp.beginPacket(broadcastIP, 5001);
  udp.write((uint8_t*)jsonBuffer, strlen(jsonBuffer));
  udp.endPacket();
  
  Serial.printf("Sent status update: %s\n", jsonBuffer);
  lastUdpSend = millis();
}}

// Function to send "I'm here" announcement
void sendAnnounce() {{
  byte deviceIdByte = (byte)deviceId;
  udp.beginPacket(broadcastIP, broadcastPort);
  udp.write(deviceIdByte);
  udp.endPacket();

  Serial.printf("Sent announcement to %s:%d: %d\n", broadcastIP.toString().c_str(), broadcastPort, deviceId);
  lastAnnounce = millis();
}}

// Function to check for incoming UDP messages
void checkForUdpMessages() {{
  int packetSize = udp.parsePacket();
  if (packetSize) {{
    char incomingPacket[255];
    int len = udp.read(incomingPacket, 255);
    if (len > 0) {{
      incomingPacket[len] = 0; // Null terminate
    }}
    
    // Try to parse as JSON
    StaticJsonDocument<128> doc;
    DeserializationError error = deserializeJson(doc, incomingPacket);
    
    if (!error) {{
      // Check if it's a ping message
      if (doc.containsKey("type") && strcmp(doc["type"], "ping") == 0) {{
        // Respond to ping with our status
        Serial.println("Received ping, sending status");
        sendStatusUpdate();
      }}
    }}
  }}
}}

// Functon to reset the starting screen
void resetStartingScreen() {{
  AtomS3.Display.setTextColor(GREEN);
  AtomS3.Display.setTextDatum(middle_center);
  AtomS3.Display.setFont(&fonts::Orbitron_Light_24);
  AtomS3.Display.setTextSize(1);
  AtomS3.Display.clear();
  AtomS3.Display.setTextColor(TFT_BLUE);
  AtomS3.Lcd.fillScreen(TFT_BLACK);
  AtomS3.Display.drawString("Starting...", AtomS3.Display.width() / 2, AtomS3.Display.height() / 3);
  AtomS3.Display.setTextSize(0.5);
  AtomS3.Display.setTextColor(TFT_RED);
  AtomS3.Display.drawString("Powered by", AtomS3.Display.width() / 2, AtomS3.Display.height() / 1.8);
  AtomS3.Display.drawString("&", AtomS3.Display.width() / 2, AtomS3.Display.height() / 1.3);
  AtomS3.Display.drawString("Eduard Balasea", AtomS3.Display.width() / 2, AtomS3.Display.height() / 1.5);
  AtomS3.Display.drawString("Rares B. Cazan", AtomS3.Display.width() / 2, AtomS3.Display.height() / 1.1);
}}


void setup() {{
  // Initialize M5Atom S3
  auto cfg = M5.config();
  AtomS3.begin(cfg);
  AtomS3.Display.setTextColor(GREEN);
  AtomS3.Display.setTextDatum(middle_center);
  AtomS3.Display.setFont(&fonts::Orbitron_Light_24);
  AtomS3.Display.setTextSize(1);
  AtomS3.Display.clear();
  AtomS3.Display.setTextColor(TFT_BLUE);
  AtomS3.Lcd.fillScreen(TFT_BLACK);
  AtomS3.Display.drawString("Starting...", AtomS3.Display.width() / 2, AtomS3.Display.height() / 3);
  AtomS3.Display.setTextSize(0.5);
  AtomS3.Display.setTextColor(TFT_RED);
  AtomS3.Display.drawString("Powered by", AtomS3.Display.width() / 2, AtomS3.Display.height() / 1.8);
  AtomS3.Display.drawString("&", AtomS3.Display.width() / 2, AtomS3.Display.height() / 1.3);
  AtomS3.Display.drawString("Eduard Balasea", AtomS3.Display.width() / 2, AtomS3.Display.height() / 1.5);
  AtomS3.Display.drawString("Rares B. Cazan", AtomS3.Display.width() / 2, AtomS3.Display.height() / 1.1);


  Serial.begin(9600);
  
  // Start connection timer
  connectionStartTime = millis();
  connectionTimerRunning = true;
  elapsedTime = 0;

  // Connect to WiFi
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {{
    
    // Display status
    displayWiFiStatus(WiFi.status());
    
    delay(500);
  }}
  
  // Check if connected successfully
  if (WiFi.status() == WL_CONNECTED) {{
    
    resetStartingScreen();

    AtomS3.Display.setTextSize(0.8);
    AtomS3.Display.drawString("Connected!", AtomS3.Display.width() / 2, AtomS3.Display.height() / 10);
    AtomS3.Display.setTextSize(0.5);
    
    // Calculate broadcast IP based on our IP address
    IPAddress myIP = WiFi.localIP();
    IPAddress subnet = WiFi.subnetMask();
    for (int i = 0; i < 4; i++) {{
      broadcastIP[i] = myIP[i] | (subnet[i] ^ 255);
    }}
    Serial.print("Broadcast IP: ");
    Serial.println(broadcastIP);
    
    // Initialize UDP
    udp.begin(localPort);
    Serial.printf("Listening on UDP port %d\n", localPort);
    
    // Initialize ATEM Switcher connection
    AtemSwitcher.begin(switcherIp);
    AtemSwitcher.serialOutput(0x80);
    AtemSwitcher.connect();
    
    // Send initial status broadcast
    sendStatusUpdate();
    
    // Show device ID after a delay
    delay(2000);
    AtomS3.Display.clear();
    AtomS3.Display.setTextColor(TFT_WHITE);
    AtomS3.Lcd.fillScreen(TFT_BLACK);
    AtomS3.Display.setTextSize(1);
    AtomS3.Display.drawString(String(deviceId), AtomS3.Display.width() / 2, AtomS3.Display.height() / 2);
  }} else {{
    // Connection failed after max retries
    elapsedTime = millis() - connectionStartTime;
    
    AtomS3.Display.clear();
    AtomS3.Display.setTextColor(TFT_WHITE);
    AtomS3.Lcd.fillScreen(TFT_RED);
    AtomS3.Display.setTextSize(0.8);
    AtomS3.Display.drawString("WiFi Failed", AtomS3.Display.width() / 2, AtomS3.Display.height() / 3);
    AtomS3.Display.setTextSize(0.5);
    AtomS3.Display.drawString("Check settings", AtomS3.Display.width() / 2, AtomS3.Display.height() / 2);
    
    // Try to proceed anyway
    IPAddress localIP = WiFi.localIP();
    IPAddress subnet = WiFi.subnetMask();
    broadcastIP = (localIP & subnet) | ~subnet;
    udp.begin(localPort);
    
    delay(3000);
  }}
  AtomS3.Display.setTextSize(2);
}}

void loop() {{
  AtomS3.update();
  
  // Monitor WiFi connection status
  if (WiFi.status() != WL_CONNECTED) {{
    // Only attempt reconnection if timer isn't already running
    if (!connectionTimerRunning) {{
      connectionStartTime = millis();
      connectionTimerRunning = true;
      WiFi.begin(ssid, password);
      
      AtomS3.Display.clear();
      AtomS3.Display.setTextColor(TFT_WHITE);
      AtomS3.Lcd.fillScreen(TFT_YELLOW);
      AtomS3.Display.setTextSize(0.8);
      AtomS3.Display.drawString("Reconnecting...", AtomS3.Display.width() / 2, AtomS3.Display.height() / 3);
    }}
    
    // Update timer
    elapsedTime = millis() - connectionStartTime;
    displayWiFiStatus(WiFi.status());
    
    // Still try to check button and send announcements even if WiFi is disconnected
  }} else {{
    // WiFi is connected
    if (connectionTimerRunning) {{
      connectionTimerRunning = false;
      
      // Recalculate broadcast IP if we just reconnected
      IPAddress myIP = WiFi.localIP();
      IPAddress subnet = WiFi.subnetMask();
      for (int i = 0; i < 4; i++) {{
        broadcastIP[i] = myIP[i] | (subnet[i] ^ 255);
      }}
      
      // Re-initialize UDP if needed
      udp.begin(localPort);
      
      // Send immediate status update
      sendStatusUpdate();
    }}
    
    // Normal operation when WiFi is connected:
    
    // Run ATEM Switcher loop
    AtemSwitcher.runLoop();
    
    // Check tally status
    int ProgramTally = AtemSwitcher.getProgramTally(deviceId);
    int PreviewTally = AtemSwitcher.getPreviewTally(deviceId);
    
    // Determine new status based on tally lights
    int newStatus = 0; // Default to idle
    if (ProgramTally) {{
      newStatus = 2; // Live
    }} else if (PreviewTally) {{
      newStatus = 1; // Preview
    }}
    
    // Update display and send UDP status if changed
    if ((ProgramTallyPrevious != ProgramTally) || (PreviewTallyPrevious != PreviewTally)) {{
      // Status has changed
      currentStatus = newStatus;
      
      // Update display
      if (ProgramTally) {{ // Live - Red
        AtomS3.Display.clear();
        AtomS3.Display.setTextColor(TFT_BLACK);
        AtomS3.Lcd.fillScreen(TFT_RED);
        AtomS3.Display.setTextSize(2);
        AtomS3.Display.drawString(String(deviceId), AtomS3.Display.width() / 2, AtomS3.Display.height() / 2);
      }} else if (PreviewTally && !ProgramTally) {{ // Preview - Green
        AtomS3.Display.clear();
        AtomS3.Display.setTextColor(TFT_BLACK);
        AtomS3.Lcd.fillScreen(TFT_GREEN);
        AtomS3.Display.setTextSize(2);
        AtomS3.Display.drawString(String(deviceId), AtomS3.Display.width() / 2, AtomS3.Display.height() / 2);
      }} else if (!PreviewTally && !ProgramTally) {{ // Idle - Black
        AtomS3.Display.clear();
        AtomS3.Display.setTextColor(TFT_WHITE);
        AtomS3.Lcd.fillScreen(TFT_BLACK);
        AtomS3.Display.setTextSize(2);
        AtomS3.Display.drawString(String(deviceId), AtomS3.Display.width() / 2, AtomS3.Display.height() / 2);
      }}
      
      // Send immediate status update via UDP
      sendStatusUpdate();
    }}
    
    // Store current tally state for next comparison
    ProgramTallyPrevious = ProgramTally;
    PreviewTallyPrevious = PreviewTally;
    
    // Check for incoming UDP messages
    checkForUdpMessages();
    
    // Send periodic status updates
    if (millis() - lastUdpSend > udpInterval) {{
      sendStatusUpdate();
    }}
  }}
  
  // Send periodic "I'm here" announcement every 5 seconds
  // (happens regardless of WiFi status)
  if (millis() - lastAnnounce > announceInterval) {{
    if (WiFi.status() == WL_CONNECTED) {{
      sendAnnounce();
    }}
  }}
  
  // Check for button press to send camera number via port 5001
  if (millis() - lastButtonCheck > buttonCheckInterval) {{
    lastButtonCheck = millis();
    
    if (AtomS3.BtnA.wasPressed() && WiFi.status() == WL_CONNECTED) {{
      // Send camera number via port 5001
      byte cameraNumber = (byte)deviceId;
      udp.beginPacket(broadcastIP, 5001);
      udp.write(cameraNumber);
      udp.endPacket();
      
      Serial.printf("Button pressed, sent camera number %d via port 5001\n", deviceId);
    }}
  }}
  
  // Small delay to prevent tight loop
  delay(10);
}}