<a href="https://imgbb.com/"><img src="https://i.ibb.co/JHpNY2n/bonsaio.png" alt="bonsaio" border="0"></a><br>

## Controlling Smart Plugs Based on Sunrise and Sunset Time

**Description:**\
This script is designed to automatically control smart plugs based on the sunrise and sunset time. It uses an ESP32 board with a WiFi module to access the current time from an NTP (Network Time Protocol) server and a sunrise-sunset calculation module.

**How the script works:**

1.  **WiFi Initialization:** The script connects to a WiFi network using the provided credentials (SSID and password).

2.  **Getting Time from NTP Server:** The script uses NTP protocol to synchronize the Arduino board's local time with a time server. This allows obtaining accurate current time.

3.  **Scanning Smart Plugs:** The script utilizes a library (KasaSmartPlug.h) to discover and identify available smart plugs on the network.

4.  **Controlling Plugs Based on Sun:** Once the plugs are identified, the script uses geographical coordinates (latitude and longitude) to determine the sunrise and sunset time for the specified location.

5.  **Controlling Plugs During the Day:** During the day (between sunrise and sunset), the script turns ON a specific smart plug (Light) to represent that it's daytime.

6.  **Controlling Fans During the Night:** During the night (outside the sunrise and sunset period), the script turns ON two other smart plugs (Fan1 and Fan2) to represent the operation of fans. After 5 seconds, it turns OFF Fan1, and after another 5 seconds, it turns OFF Fan2. Then, it waits for 5 minutes before repeating the cycle.

**Required Configuration:**

-   An Arduino board with a WiFi module (e.g., ESP8266).
-   Sun.h library for sunrise and sunset calculation.
-   Time.h library for time management.
-   KasaSmartPlug.h library for controlling Kasa smart plugs.

**Note:**

Ensure to replace the following values in the script with your own WiFi network information:

-   `myssid`: Your WiFi network name (SSID).
-   `mywifipassword`: Your WiFi password.

You can also adjust the names of the smart plugs if yours have different names.

**Note:** Please be aware that this summary is based on an analysis of the provided code and may not include all details of the script's operation. For correct usage, make sure to have the appropriate libraries and set up your environment correctly.

# links
Libraries: <br>
https://github.com/kj831ca/KasaSmartPlug<br>
https://github.com/nplan/Arduino-Sun<br>
https://playground.arduino.cc/Code/Time/<br>
