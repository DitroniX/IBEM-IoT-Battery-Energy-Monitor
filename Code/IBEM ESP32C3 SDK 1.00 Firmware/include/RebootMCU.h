/*
  Dave Williams, DitroniX 2019-2024 (ditronix.net)
  Full header information in README

  This test code is OPEN SOURCE and formatted for easier viewing.  Although is is not intended for real world use, it may be freely used, or modified as needed.
  It is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.

  Further information, details and examples can be found on our website and also GitHub wiki pages:
  * github.com/DitroniX
  * github.com/DitroniX/IBEM-IoT-Battery-Energy-Monitor
  * github.com/DitroniX/IBEM-IoT-Battery-Energy-Monitor/wiki
  * hackster.io/DitroniX/ibem-esp32c3-iot-battery-energy-monitor-solar-inverters-0342b1
*/

// Libraries
#include <Arduino.h>

// ****************  VARIABLES / DEFINES / STATIC / STRUCTURES ****************

// 0 reset disabled
// 300000UL call reset every 300 seconds (5 Minutes)
// 3600000UL call reset every 60 mins (1 Hour)
// 86400000UL call reset every 24 hours (1 Day)
// 604800000UL call reset every 7 days (1 Week).
// 2592000000UL call reset every 30 days (1 Month)

// time_t RebootTime = 604800000UL; // Configure as needed.

// **************** FUNCTIONS AND ROUTINES ****************
// Reset MCU
void (*resetFunc)(void) = 0; // DO NOT CHANGE. Declare Reset Function at Address 0x0000

// Force Reboot
void ForceReboot()
{
  Serial.println("\nForced Reboot ...");
  oled.clear();
  OLEDPrint(AppAcronym, 2, 0);
  OLEDPrint("Forced", 2, 3);
  OLEDPrint("Reboot", 2, 5);
  oled.update();
  delay(OLEDDelay * 2000);

  resetFunc();
} // ForceReboot

// Check Time Period and Reset
void RebootCheck()
{
  // Routing polled and is used to optionally reboot the ESP32C3 after a defined period of time.

  if (millis() >= RebootTime && RebootTime != 0)
    ForceReboot();

} // RebootCheck
