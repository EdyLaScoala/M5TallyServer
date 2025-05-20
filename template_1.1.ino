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
IPAddress broadcastIP;       // Will be set in setup based on WiFi IP
unsigned long lastUdpSend = 0;
const int udpInterval = 2000; // Send status every 2 seconds

// Device identification
const int deviceId = {camNumber}; // Using camera number as device ID
int currentStatus = 0; // 0-idle, 1-preview, 2-live

// Tally state tracking
int PreviewTallyPrevious = 1;
int ProgramTallyPrevious = 1;

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
  AtomS3.Display.drawString("Starting...", AtomS3.Display.width() / 2, AtomS3.Display.height() / 2.5);
  AtomS3.Display.setTextSize(0.5);
  AtomS3.Display.setTextColor(TFT_RED);
  AtomS3.Display.drawString("Powered by", AtomS3.Display.width() / 2, AtomS3.Display.height() / 1.25);
  AtomS3.Display.setTextSize(0.7);
  AtomS3.Display.drawString("edybalasa", AtomS3.Display.width() / 2, AtomS3.Display.height() / 1.1);

  Serial.begin(9600);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {{
    delay(500);
    Serial.println("Connecting to WiFi..");
  }}
  Serial.println("Connected to the WiFi network");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

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

void loop() {{
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
      AtomS3.Display.drawString(String(deviceId), AtomS3.Display.width() / 2, AtomS3.Display.height() / 2);
    }} else if (PreviewTally && !ProgramTally) {{ // Preview - Green
      AtomS3.Display.clear();
      AtomS3.Display.setTextColor(TFT_BLACK);
      AtomS3.Lcd.fillScreen(TFT_GREEN);
      AtomS3.Display.drawString(String(deviceId), AtomS3.Display.width() / 2, AtomS3.Display.height() / 2);
    }} else if (!PreviewTally && !ProgramTally) {{ // Idle - Black
      AtomS3.Display.clear();
      AtomS3.Display.setTextColor(TFT_WHITE);
      AtomS3.Lcd.fillScreen(TFT_BLACK);
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
  
  // Small delay to prevent tight loop
  delay(10);
}}