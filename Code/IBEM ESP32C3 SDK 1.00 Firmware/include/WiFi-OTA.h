/*
  Dave Williams, DitroniX 2019-2024 (ditronix.net)
  Full header information in README

  This test code is OPEN SOURCE and formatted for easier viewing.  Although is is not intended for real world use, it may be freely used, or modified as needed.
  It is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.

  Note: Adding an SSID will Automatically ENABLE WiFi attempt to Log-In.

  Preamble.  This WiFi-OTA will:
  * Configure WiFi Parameters (e.g. ssid etc.)
  * Configure Optional Static IP Address (leave 0,0,0,0 for DHCP settings)
  * Connect to WiFi
  * Create a Network Port in Serial Devices
  * Display WiFI detail on OLED
    
  Further information, details and examples can be found on our website and also GitHub wiki pages:
  * github.com/DitroniX
  * github.com/DitroniX/IBEM-IoT-Battery-Energy-Monitor
  * hackster.io/DitroniX/ibem-esp32c3-iot-battery-energy-monitor-solar-inverters-0342b1
*/

// Libraries
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <WiFiUdp.h>
#include <AsyncTCP.h>
#include <WiFiClient.h>
#include "WiFi-WebServer.h"

// ****************  VARIABLES / DEFINES / STATIC / STRUCTURES ****************

uint64_t chipid = ESP.getEfuseMac(); // Get ChipID (essentially the MAC address)
WiFiClient wlan_client;              // Initialize the Client Library  / Client Instance
WiFiMulti wifiMulti;                 // Multiple WiFi Options - Auto Scanning

// ######### FUNCTIONS #########

// Return Some Meaningful Information From RSSI
String RSSI_Info(int RSSI_Value)
{
    switch (-RSSI_Value) // Inverted dBm Level ;)
    {
        {
        case 0 ... 30:
            return "Signal Very Strong";
            break;
        case 31 ... 50:
            return "Signal Excellent";
            break;
        case 51 ... 60:
            return "Signal Healthy";
            break;
        case 61 ... 70:
            return "Signal Very Good";
            break;
        case 71 ... 80:
            return "Signal Good";
            break;
        case 81 ... 90:
            return "Signal Poor - Try Moving Position";
            break;
        case 91 ... 100:
            return "Signal Very Low! - Move Position";
            break;
        default:
            return "No Signal :(";
            break;
        }
    }

} // RSSI_Info

// Wifi Information
void DisplayWiFiConfiguration()
{
    RSSI_Level = WiFi.RSSI();
    RSSI_Detail = RSSI_Info(RSSI_Level);
    MACAddress = String(WiFi.macAddress());
    WiFi_SSID = String(WiFi.SSID());

    // Blue LED
    led.flash(RGBLed::BLUE, 50);

    PrintUnderline("Connection Details:");
    Serial.println("WiFi SSID \t " + WiFi_SSID + " (Wifi Station Mode)");

    if (local_IP.toString() == WiFi.localIP().toString().c_str())
    {
        Serial.printf("WiFi IP \t %s (Static)\n", WiFi.localIP().toString().c_str());
    }
    else
    {
        Serial.printf("WiFi IP \t %s (DHCP)\n", WiFi.localIP().toString().c_str());
    }

    Serial.printf("WiFi MASK \t %s\n", WiFi.subnetMask().toString().c_str());
    Serial.printf("WiFi GW \t %s\n", WiFi.gatewayIP().toString().c_str());
    Serial.printf("WiFi DNS 1 \t %s\n", WiFi.dnsIP().toString().c_str());
    Serial.printf("WiFi DNS 2 \t %s\n", WiFi.dnsIP(1).toString().c_str());
    Serial.println("WiFi MAC \t " + MACAddress);
    Serial.printf("WiFi Hostname \t %s\n", WiFi.getHostname());
    Serial.println("WiFi RSSI \t " + String(RSSI_Level) + " dBm (" + RSSI_Detail + ")");

    Serial.println("");
} // DisplayWiFiConfiguration

// Force Hostname
void ForceHostName(void)
{
    HostName = AppAcronym + "-";
    HostName.concat(WiFi.macAddress().substring(WiFi.macAddress().length() - 9, WiFi.macAddress().length()));
    HostName.replace(":", "");
    WiFi.setHostname(HostName.c_str());
    HostName = WiFi.getHostname();
} // ForceHostName

// Configure Static IP address (if not 0.0.0.0)
void InitialiseStaticIP()

{
    if (local_IP.toString() != "0.0.0.0")
    {
        // Serial.printf("WiFI Current IP\t \t %s\n", WiFi.localIP().toString().c_str());

        if (primaryDNS.toString() != "0.0.0.0")
        {
            if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS))
                ;
            {
                Serial.println("STA Static IP Failed to configure");
            }
        }
        else // Use Gateway IP for DNS 1
        {
            if (!WiFi.config(local_IP, gateway, subnet, gateway, secondaryDNS))
            {
                Serial.println("STA Static IP Failed to configure");
            }
        }
    }
} // InitialiseStaticIP

// Initialise WiFi
void InitialiseWiFi()
{
    if (WiFiEnabled == true)
    {
        // WiFi Integration Status
        if (WiFi.status() == WL_CONNECTED)
        {
            Serial.println("Wi-Fi is Connected\n");
        }
        else
        {
            Serial.println("Wi-Fi is Not Connected\n");

            // Red LED
            led.flash(RGBLed::RED, 250);

            // App and Starting...
            oled.clear();
            OLEDPrint(AppAcronym, 2, 0);
            OLEDPrint("Wi-Fi", 2, 3);
            OLEDPrint("Disabled", 2, 5);
            oled.update();
            delay(OLEDDelay * 1000);
        }

        // Connect or reconnect to WiFi
        if (WiFi.status() != WL_CONNECTED && strlen(ssid1))
        {
            // Blue LED
            led.flash(RGBLed::BLUE, 50);

            // App and Starting...
            oled.clear();
            OLEDPrint(AppAcronym, 2, 0);
            OLEDPrint("Trying", 2, 3);
            OLEDPrint("Wi-Fi", 2, 5);
            oled.update();

            // Wifi Settings
            WiFi.mode(WIFI_STA);
            WiFi.setAutoReconnect(true);
            WiFi.persistent(true);

            // Multi-WiFi Connections.  Add more as needed.
            if (strlen(ssid1))
                wifiMulti.addAP(ssid1, password1);
            if (strlen(ssid2))
                wifiMulti.addAP(ssid2, password2);
            if (strlen(ssid3))
                wifiMulti.addAP(ssid3, password3);

            Serial.println("Scanning WiFi Networks - Excluding Hidden... Please wait...");
            int n = WiFi.scanNetworks(false, false);

            if (n == 0)
            {
                Serial.println("No Wi-FI Networks Found");

                // App and Connected...
                oled.clear();
                OLEDPrint(AppAcronym, 2, 0);
                OLEDPrint("No", 2, 3);
                OLEDPrint("Wi-Fi", 2, 5);
                oled.update();
                delay(OLEDDelay * 1000);

                // Red LED
                led.flash(RGBLed::RED, 250);
            }
            else
            {
                Serial.print(n);
                Serial.print(" x Wi-Fi Networks Found.\t");
                Serial.println("Scanning for " + String(WifiMaxAPScan) + " SSIDs Only.");
                for (int i = 0; i < n; ++i)
                {
                    // Print SSID and RSSI for Each Network Found
                    Serial.print(" ");
                    Serial.print(i + 1);
                    Serial.print(": ");
                    Serial.printf("%25s", WiFi.SSID(i).c_str());
                    Serial.print(" (");
                    Serial.print(WiFi.RSSI(i));
                    Serial.print(" dBm " + RSSI_Info(WiFi.RSSI(i)) + ")");
                    Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? " " : "*");

                    // Blue LED
                    led.flash(RGBLed::BLUE, 50);

                    // Do Not Unnecessarily Dither - Scan first X Access Points Only
                    if (i >= (WifiMaxAPScan - 1))
                        break;
                }
            }

            // Blue LED
            led.flash(RGBLed::BLUE, 50);

            // Connect to Wi-Fi using wifiMulti (connects to the SSID with strongest connection)
            Serial.println("\nConnecting WiFi...");

            // Auto Reconnect Timeout
            unsigned long ReconnectTimeout;
            ReconnectTimeout = millis();

            while (wifiMulti.run() != WL_CONNECTED)
            { // Wait for the Wi-Fi to connect: scan for Wi-Fi networks, and connect to the strongest of the networks above
                delay(250);
                Serial.print('.');

                // Blue LED
                led.flash(RGBLed::BLUE, 50);

                // Auto Reboot if not connected in 1 minute.  Occassionally needed after reflash.
                if ((millis() - ReconnectTimeout) > 60000UL)
                    ForceReboot();
            }

            // if (wifiMulti.run() == WL_CONNECTED)
            {
                // // Stabalise for Slow Access Points
                // while (WiFi.status() != WL_CONNECTED)
                // {
                //     delay(500);
                //     Serial.print(".");
                // }

                if (WiFi.status() == WL_CONNECTED)
                {
                    // App and Connected...
                    oled.clear();
                    OLEDPrint(AppAcronym, 2, 0);
                    OLEDPrint("Connected", 2, 3);
                    OLEDPrint("Wi-Fi", 2, 5);
                    oled.update();
                    delay(OLEDDelay * 1000);

                    Serial.println("Connected to " + String(WiFi.SSID()) + "\n");

                    delay(500);

                    // Force Local IP and Make Static
                    InitialiseStaticIP();

                    ForceHostName();

                    // Blue LED
                    led.flash(RGBLed::BLUE, 50);

                    // Wifi Information
                    DisplayWiFiConfiguration();

                    // Update OLED
                    oled.clear();
                    OLEDPrint(AppAcronym, 2, 0);
                    OLEDPrint("Connected to Wi-Fi ", 1, 3);
                    OLEDPrint(WiFi.SSID(), 1, 4);
                    OLEDPrint(WiFi.localIP().toString().c_str(), 1, 5);
                    OLEDPrint(WiFi.getHostname(), 1, 6);
                    oled.update();
                    delay(OLEDDelay * 2000);
                }
                // Blue LED
                led.flash(RGBLed::BLUE, 50);

                // }
            }
        }
    }
    else
    {
        // Red LED
        led.flash(RGBLed::RED, 250);

        // App and Starting...
        oled.clear();
        OLEDPrint(AppAcronym, 2, 0);
        OLEDPrint("Wi-Fi", 2, 3);
        OLEDPrint("Disabled", 2, 5);
        oled.update();
        delay(OLEDDelay * 1000);
    }

} // InitialiseWiFi
