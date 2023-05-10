#include <WiFi.h>
#include <Sun.h>
#include "Time.h"
#include "KasaSmartPlug.h"

// Replace with your network credentials
const char *ssid = "myssid";
const char *password = "mywifipassword";

// NTP server to request epoch time
const char *ntpServer = "pool.ntp.org";

// Variable to save current epoch time
unsigned long epochTime;

// Function that gets current epoch time
unsigned long getTime()
{
  time_t now;
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo))
  {
    // Serial.println("Failed to obtain time");
    return (0);
  }
  time(&now);
  return now;
}

KASAUtil kasaUtil;

// Initialize WiFi
void initWiFi()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
}

void setup()
{
  int found;
  Serial.begin(115200);
  initWiFi();
  configTime(0, 0, ntpServer);
  found = kasaUtil.ScanDevices();
}

void loop()
{
  Sun sun(45.501, -73.567); // Montreal,CA

  // Initialize Smart plugs

  KASASmartPlug *Plug1 = kasaUtil.GetSmartPlug("Light");
  KASASmartPlug *Plug2 = kasaUtil.GetSmartPlug("Mist");
  KASASmartPlug *Plug3 = kasaUtil.GetSmartPlug("Fan1");
  KASASmartPlug *Plug4 = kasaUtil.GetSmartPlug("Fan2");

  Serial.print("Epoch Time: ");
  Serial.println(epochTime);

  epochTime = getTime();
  unsigned long tm = epochTime;

  unsigned long rise = sun.getRise(tm);
  Serial.print("Sun rises at: ");
  Serial.println(rise);

  unsigned long set = sun.getSet(tm);
  Serial.print("Sun sets at: ");
  Serial.println(set);

  if (epochTime >= sun.getRise(tm) && epochTime < sun.getSet(tm))
  {
    Serial.println("It's day");
    Plug1->SetRelayState(1);
  }
  else
  {
    Serial.println("It's night");
    Plug1->SetRelayState(0);
  }

  Serial.println("Fans ON");
  Plug3->SetRelayState(1);
  Plug4->SetRelayState(1);
  delay(5000);
  Serial.println("Fan1 OFF");
  Plug3->SetRelayState(0);
  delay(5000);
  Serial.println("Fan2 OFF");
  Plug4->SetRelayState(0);
  delay(300000);
}
