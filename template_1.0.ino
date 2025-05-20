#include "M5AtomS3.h"
#include <WiFi.h>
#include <SkaarhojPgmspace.h>
#include <ATEMbase.h>
#include <ATEMstd.h>

IPAddress switcherIp({ip_octet1}, {ip_octet2}, {ip_octet3}, {ip_octet4});
ATEMstd AtemSwitcher;

const char* ssid = "{ssid}";
const char* password =  "{passwd}";

const int cameraNumber = {camNumber};
int ledPin = 10;

int PreviewTallyPrevious = 1;
int ProgramTallyPrevious = 1;

void setup() {{

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
  AtomS3.Display.drawString("edybalasa", AtomS3.Display.width() / 2, AtomS3.Display.height() / 1.1);;


  Serial.begin(9600);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {{
    delay(500);
    Serial.println("Connecting to WiFi..");
  }}
  Serial.println("Connected to the WiFi network");
  AtomS3.Display.setTextSize(1.5);
  AtomS3.Display.clear();
  AtomS3.Display.setTextColor(TFT_WHITE);
  AtomS3.Lcd.fillScreen(TFT_BLACK);
  AtomS3.Display.drawString(String(cameraNumber), AtomS3.Display.width() / 2, AtomS3.Display.height() / 2);

  delay(500);

  AtemSwitcher.begin(switcherIp);
  AtemSwitcher.serialOutput(0x80);
  AtemSwitcher.connect();
}}

int ProgramTally;
int PreviewTally;

void loop() {{

  AtemSwitcher.runLoop();

  ProgramTally = AtemSwitcher.getProgramTally(cameraNumber);
  PreviewTally = AtemSwitcher.getPreviewTally(cameraNumber);

  if ((ProgramTallyPrevious != ProgramTally) || (PreviewTallyPrevious != PreviewTally)) {{ // changed?

    if (ProgramTally) {{ // only program, or program AND preview
      AtomS3.Display.clear();
      AtomS3.Display.setTextColor(TFT_BLACK);
      AtomS3.Lcd.fillScreen(TFT_RED);
      AtomS3.Display.drawString(String(cameraNumber), AtomS3.Display.width() / 2, AtomS3.Display.height() / 2);
    }} else if (PreviewTally && !ProgramTally) {{ // only preview
      AtomS3.Display.clear();
      AtomS3.Display.setTextColor(TFT_BLACK);
      AtomS3.Lcd.fillScreen(TFT_GREEN);
      AtomS3.Display.drawString(String(cameraNumber), AtomS3.Display.width() / 2, AtomS3.Display.height() / 2);
    }} else if (!PreviewTally && !ProgramTally) {{ // neither
      AtomS3.Display.clear();
      AtomS3.Display.setTextColor(TFT_WHITE);
      AtomS3.Lcd.fillScreen(TFT_BLACK);
      AtomS3.Display.drawString(String(cameraNumber), AtomS3.Display.width() / 2, AtomS3.Display.height() / 2);
    }}
  }}
  ProgramTallyPrevious = ProgramTally;
  PreviewTallyPrevious = PreviewTally;
}}