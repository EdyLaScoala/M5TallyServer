#include "M5AtomS3.h"
#include <WiFi.h>

const char* ssid = "{ssid}";
const char* password = "{passwd}";

void setup() {{
  // Initialize M5Atom S3
  auto cfg = M5.config();
  AtomS3.begin(cfg);
  AtomS3.Display.setTextColor(GREEN);
  AtomS3.Display.setTextDatum(middle_center);
  AtomS3.Display.setFont(&fonts::Orbitron_Light_24);
  AtomS3.Display.setTextSize(1);
  AtomS3.Display.clear();
  AtomS3.Lcd.fillScreen(TFT_BLACK);
  AtomS3.Display.drawString("WiFi Test", AtomS3.Display.width() / 2, AtomS3.Display.height() / 2.5);
  AtomS3.Display.setTextSize(0.5);
  AtomS3.Display.setTextColor(TFT_RED);
  AtomS3.Display.drawString("Connecting...", AtomS3.Display.width() / 2, AtomS3.Display.height() / 1.25);

  Serial.begin(9600);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  int attempt = 0;
  while (WiFi.status() != WL_CONNECTED) {{
    delay(500);
    AtomS3.Display.clear();
    AtomS3.Lcd.fillScreen(TFT_BLACK);
    AtomS3.Display.setTextColor(TFT_WHITE);
    AtomS3.Display.drawString("Connecting", AtomS3.Display.width() / 2, AtomS3.Display.height() / 2.5);
    AtomS3.Display.drawString(String(attempt++), AtomS3.Display.width() / 2, AtomS3.Display.height() / 1.25);
    Serial.print(".");
  }}

  Serial.println("\\nConnected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Show connected status
  AtomS3.Display.clear();
  AtomS3.Lcd.fillScreen(TFT_BLACK);
  AtomS3.Display.setTextColor(TFT_GREEN);
  AtomS3.Display.setTextSize(0.7);
  AtomS3.Display.drawString("Connected", AtomS3.Display.width() / 2, AtomS3.Display.height() / 3);
  AtomS3.Display.drawString(WiFi.localIP().toString(), AtomS3.Display.width() / 2, AtomS3.Display.height() * 2 / 3);
}}

void loop() {{
  // Do nothing in loop
  delay(1000);
}}
