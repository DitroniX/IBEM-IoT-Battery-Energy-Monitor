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

// **************** FUNCTIONS AND ROUTINES ****************

// Initialise Domoticz
void InitialiseDomoticz()
{
    // Domoticz Integration Status
    if (EnableDomoticz == true && WiFi.status() == WL_CONNECTED)
    {
        Serial.println("Domoticz Enabled - Register Values Will be Published");
    }
    else
    {
        Serial.println("Domoticz Publishing is Disabled");
    }
}

// Publish to Domoticz - Single String Values
void PublishDomoticzString(int Sensor_Index, float Sensor_Value, String Sensor_Name = "")
{
    if (Sensor_Index > 0 && EnableDomoticz == true)
    {
        if (wlan_client.connect(DomoticzServer, DomoticzPort))
        {
            // Green LED
            led.flash(RGBLed::GREEN, 50);

            Serial.print("Sending Message to Domoticz #");
            Serial.print(Sensor_Index);
            Serial.print(" ");
            Serial.print(Sensor_Value);
            Serial.print(" \t");
            Serial.println(Sensor_Name);

            wlan_client.print("GET /json.htm?type=command&param=udevice&idx=");
            wlan_client.print(Sensor_Index);

            wlan_client.print("&svalue=");
            wlan_client.print(Sensor_Value);

            wlan_client.println(" HTTP/1.1");
            wlan_client.print("Host: ");
            wlan_client.print(DomoticzServer);
            wlan_client.print(":");

            wlan_client.println(DomoticzPort);
            wlan_client.println("User-Agent: Arduino-ethernet");
            wlan_client.println("Connection: close");
            wlan_client.println();

            wlan_client.stop();
        }
        else
        {
            // Red LED
            led.flash(RGBLed::RED, 20);

            Serial.println("Domoticz Server Not Connected");

            // Update OLED
            oled.clear();
            OLEDPrint("Error", 2, 0);
            OLEDPrint("Domoticz", 2, 2);
            OLEDPrint("Server", 2, 4);
            oled.update();
            delay(1000);

            // Stabalise for slow Access Points
            InitialiseWiFi();
        }
    }
} // PublishDomoticz

// Publish to Domoticz - Single Numeric Values
void PublishDomoticzNumeric(int Sensor_Index, float Sensor_Value, String Sensor_Name = "")
{
    if (Sensor_Index > 0 && EnableDomoticz == true)
    {
        if (wlan_client.connect(DomoticzServer, DomoticzPort))
        {
            // Green LED
            led.flash(RGBLed::GREEN, 50);

            Serial.print("Sending Message to Domoticz #");
            Serial.print(Sensor_Index);
            Serial.print(" ");
            Serial.print(Sensor_Value);
            Serial.print(" \t");
            Serial.println(Sensor_Name);

            wlan_client.print("GET /json.htm?type=command&param=udevice&idx=");
            wlan_client.print(Sensor_Index);

            wlan_client.print("&nvalue=");
            wlan_client.print(Sensor_Value);

            wlan_client.println(" HTTP/1.1");
            wlan_client.print("Host: ");
            wlan_client.print(DomoticzServer);
            wlan_client.print(":");

            wlan_client.println(DomoticzPort);
            wlan_client.println("User-Agent: Arduino-ethernet");
            wlan_client.println("Connection: close");
            wlan_client.println();

            wlan_client.stop();
        }
        else
        {
            // Red LED
            led.flash(RGBLed::RED, 20);

            Serial.println("Domoticz Server Not Connected");

            // Update OLED
            oled.clear();
            OLEDPrint("Error", 2, 0);
            OLEDPrint("Domoticz", 2, 2);
            OLEDPrint("Server", 2, 4);
            oled.update();
            delay(1000);

            // Stabalise for slow Access Points
            InitialiseWiFi();
        }
    }
} // PublishDomoticzNumeric

// Publish to Domoticz - Listed Values
void PublishDomoticzValues()
{
    if (EnableDomoticz == true)
    {
        if (wlan_client.connect(DomoticzServer, DomoticzPort) && WiFi.status() == WL_CONNECTED)
        {
            // IBEM Readings
            if (idxDCCurrentAccumulative > 0)
            {
                PublishDomoticzString(idxDCCurrentAccumulative, DCCurrentAccumulative, "DCCurrentAccumulative");
            }

            if (idxDCPower > 0)
            {
                PublishDomoticzString(idxDCPower, DCPower, "DCPower");
            }

            if (idxDCVoltage > 0)
            {
                PublishDomoticzString(idxDCVoltage, DCVoltage, "DCVoltage");
            }

            if (idxBoardTemperatureC > 0)
            {
                PublishDomoticzString(idxBoardTemperatureC, BoardTemperatureC, "BoardTemperatureC");
            }

            if (idxBoardTemperatureF > 0)
            {
                PublishDomoticzString(idxBoardTemperatureF, BoardTemperatureF, "BoardTemperatureF");
            }

            if (idxProbeTemperatureC > 0)
            {
                PublishDomoticzString(idxProbeTemperatureC, ProbeTemperatureC, "ProbeTemperatureC");
            }

            if (idxProbeTemperatureF > 0)
            {
                PublishDomoticzString(idxProbeTemperatureF, ProbeTemperatureF, "ProbeTemperatureF");
            }

            // NTP Time
            timeClient.update();
            Serial.println(timeClient.getFormattedTime() + "> Published to Domoticz\n");
        }
        else
        {
            // Red LED
            led.flash(RGBLed::RED, 20);

            Serial.println("Domoticz Server Not Connected");

            // Update OLED
            oled.clear();
            OLEDPrint("Error", 2, 0);
            OLEDPrint("Domoticz", 2, 2);
            OLEDPrint("Server", 2, 4);
            oled.update();
            delay(1000);

            // Stabalise for slow Access Points
            InitialiseWiFi();
        }
    }
} // PublishDomoticzValues

// It is possible to post a Group of Values to a single Virtual Sensor and graph it (i.e. Voltage, Current, Wattage).
// This will require some coding in Domoticz (possible plugin/sensor type).  Feedback welcomed!.
// Publish to Domoticz EXAMPLE - Batch or Group Values Example to Virtual Sensor.  Update as needed.  Future WIP Option.
void PublishDomoticzATM(int Sensor_Index)
{
    if (Sensor_Index > 0 && EnableDomoticz == true)
    {
        if (wlan_client.connect(DomoticzServer, DomoticzPort) && EnableDomoticz == true && WiFi.status() == WL_CONNECTED)
        {
            // Green LED
            led.flash(RGBLed::GREEN, 50);

            Serial.print("Sending ATM Group Message to Domoticz #");
            Serial.print(Sensor_Index);

            wlan_client.print("GET /json.htm?type=command&param=udevice&idx=");
            wlan_client.print(Sensor_Index);

            // Code here

            wlan_client.println(" HTTP/1.1");
            wlan_client.print("Host: ");
            wlan_client.print(DomoticzServer);
            wlan_client.print(":");

            wlan_client.println(DomoticzPort);
            wlan_client.println("User-Agent: Arduino-ethernet");
            wlan_client.println("Connection: close");
            wlan_client.println();

            wlan_client.stop();
        }
        else
        {
            // Red LED
            led.flash(RGBLed::RED, 20);

            Serial.println("Domoticz Server Not Connected");

            // Update OLED
            oled.clear();
            OLEDPrint("Error", 2, 0);
            OLEDPrint("Domoticz", 2, 2);
            oled.update();
            delay(1000);

            // Stabalise for slow Access Points
            InitialiseWiFi();
        }
    }
} // PublishDomoticzATM
