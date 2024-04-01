/*
  Dave Williams, DitroniX 2019-2024 (ditronix.net)
  Full header information in README

  This test code is OPEN SOURCE and formatted for easier viewing.  Although is is not intended for real world use, it may be freely used, or modified as needed.
  It is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.

  Further information, details and examples can be found on our website and also GitHub wiki pages:
  * github.com/DitroniX
  * github.com/DitroniX/IBEM-IoT-Battery-Energy-Monitor
  * github.com/DitroniX/IBEM-IoT-Battery-Energy-Monitor/wiki
  * hackster.io/DitroniX/ibem-esp32c3-iot-battery-energy-monitor-solar-inverters-0342b11
*/

// Libraries
#include <Arduino.h>
#include <NTPClient.h>
#include <Timezone.h>
#include <WiFi.h>
#include <WiFiUdp.h>

// ****************  VARIABLES / DEFINES / STATIC / STRUCTURES / CONSTANTS ****************

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 3600, 60000);

// WIP https://www.circuitbasics.com/using-an-arduino-ethernet-shield-for-timekeeping/

// **************** FUNCTIONS AND ROUTINES ****************

// Initialise NTP
void InitialiseNTP()
{
    if (WiFi.status() == WL_CONNECTED)
    {
        // Time Server
        timeClient.begin();
        // timeClient.setTimeOffset(0);

        // https://github.com/JChristensen/Timezone
        // United Kingdom (London) https://en.wikipedia.org/wiki/British_Summer_Time
        TimeChangeRule BST = {"BST", Last, Sun, Mar, 1, 60}; // British Summer Time
        TimeChangeRule GMT = {"GMT", Last, Sun, Oct, 1, 0};  // Standard Time
        Timezone UK(BST, GMT);
    }
} // Initialise NTP

// Update NTP # To be updated to local RTC Updating and testing of Xtal
void UpdateNTP()
{
    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.println(F("Getting time from NTP server..."));
        while (!timeClient.update())
        {
            timeClient.forceUpdate();
        }

        NTP_RTC = timeClient.getFormattedTime();
        Serial.println("NTP: " + NTP_RTC + "\n");
    }
} // UpdateNTP

// Read Time
void ReadTime()
{
    // Could be used to set, or read, internal RTC
    NTP_RTC = timeClient.getFormattedTime();
} // ReadTime
