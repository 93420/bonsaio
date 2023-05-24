#include <WiFi.h>
#include <Sun.h>
#include <Time.h>
#include <KasaSmartPlug.h>
#include "DHT.h"

// Replace with your network credentials
const char *ssid = "myssid";
const char *password = "mywifipassword";

// NTP server to request epoch time
const char *ntpServer = "pool.ntp.org";
const char *LIGHT_NAME = "Plug Rampe";
const char *MIST_NAME = "Plug Mist";
const char *FAN1_NAME = "Plug Fan 1";
const char *FAN2_NAME = "Plug Fan 2";

// DHT22 settings
#define DHT1PIN 22
#define DHT2PIN 23
#define DHTTYPE DHT22

DHT dht1(DHT1PIN, DHTTYPE);
DHT dht2(DHT2PIN, DHTTYPE);

// Function that gets current epoch time
unsigned long getTime()
{
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo))
  {
    Serial.println("Failed to obtain time");
    return 0;
  }
  return time(nullptr);
}

// Initialize WiFi
void initWiFi()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print('.');
    delay(1000);
  }
  Serial.println("\nWiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void setup()
{
  Serial.begin(115200);
  initWiFi();
  configTime(0, 0, ntpServer);
  dht1.begin();
  dht2.begin();
}

void loop()
{
  // Get current epoch time
  unsigned long epochTime = getTime();

  // Initialize Sun object with location coordinates
  Sun sun(45.501, -73.567); // Coordinates for Montreal,CA

  // Get sunrise and sunset times
  unsigned long sunrise = sun.getRise(epochTime);
  unsigned long sunset = sun.getSet(epochTime);

  // Initialize KASA utility object
  KASAUtil kasaUtil;

  // Scan for available KASA smart plugs
  kasaUtil.ScanDevices();

  // Get KASA smart plugs
  KASASmartPlug *Plug1 = kasaUtil.GetSmartPlug(LIGHT_NAME);
  KASASmartPlug *Plug2 = kasaUtil.GetSmartPlug(MIST_NAME);
  KASASmartPlug *Plug3 = kasaUtil.GetSmartPlug(FAN1_NAME);
  KASASmartPlug *Plug4 = kasaUtil.GetSmartPlug(FAN2_NAME);

  // Set state of light based on sunrise and sunset times
  if (epochTime >= sunrise && epochTime < sunset)
  {
    // Turn on light during the day
    Serial.println("It's day");
    if (Plug1->state == 0)
    {
      Plug1->SetRelayState(1);
    }
  }
  else
  {
    // Turn off light at night
    Serial.println("It's night");
    if (Plug1->state == 1)
    {
      Plug1->SetRelayState(0);
    }
  }

  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h1 = dht1.readHumidity();
  float h2 = dht2.readHumidity();
  // Read temperature as Celsius (the default)
  float t1 = dht1.readTemperature();
  float t2 = dht2.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(h1) || isnan(t1))
  {
    Serial.println(F("Failed to read from DHT 1 sensor!"));
    return;
  }
  if (isnan(h2) || isnan(t2))
  {
    Serial.println(F("Failed to read from DHT 2 sensor!"));
    return;
  }

  // Control fan1 and fan2
  Serial.print(F("Humidity 1: "));
  Serial.print(h1);
  Serial.print(F("%  Temperature 1: "));
  Serial.print(t1);
  Serial.print(F("°C "));

  Serial.print(F("Humidity 2: "));
  Serial.print(h2);
  Serial.print(F("%  Temperature 2 "));
  Serial.print(t2);
  Serial.print(F("°C "));

  if (h1 >= 80 || t1 >= 30) //turn ON FAN 1 if humdity > 80% or Temp > 30°C
  {
    Serial.println("Fan1 ON");
    Plug3->SetRelayState(1);
  }
  else
  {
    Serial.println("Fan1 OFF");
    Plug3->SetRelayState(0);
  }
  if (h2 >= 70 || t2 >= 28) //turn ON FAN 2 if humdity > 70% or Temp > 28°C
  {
    Serial.println("Fan2 ON");
    Plug4->SetRelayState(1);
  }
  else
  {
    Serial.println("Fan2 OFF");
    Plug4->SetRelayState(0);
  }
  delay(30000);
}
