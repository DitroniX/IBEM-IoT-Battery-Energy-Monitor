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

// Example Publish (play data), can be found on https://thingspeak.com/channels/2491848

// Libraries
#include <Arduino.h>
#include "ThingSpeak.h" // https://github.com/mathworks/thingspeak-arduino/blob/master/README.md

// Initialise ThingSpeak
void InitialiseThingSpeak()
{
  if (EnableThingSpeak == true && WiFi.status() == WL_CONNECTED)
  {
    ThingSpeak.begin(wlan_client); // Initialize ThingSpeak #################################################################
    Serial.println("Initialised ThingSpeak");
  }

} // void InitialiseThingSpeak()

// Publish a Multi-Field Formatted ThingSpeak Message
void SetThingSpeakField(unsigned long ChannelID, float SensorValue, int dPlaces = 1, int dWidth = 5)
{
  // Green LED
  digitalWrite(LED_Green, LOW);

  // Structure ThingSpeak Payload
  char MQTTPayload[10];
  dtostrf(SensorValue, dWidth, dPlaces, MQTTPayload);

  // Update Serial Monitor
  Serial.print("Sending ThingSpeak Message: ");
  Serial.print(ChannelID);
  Serial.print("\t");
  Serial.println(MQTTPayload);

  ThingSpeak.setField(ChannelID, MQTTPayload);

  // Green LED
  digitalWrite(LED_Green, HIGH);

} // PublishThingSpeakMessage

// Publish ThingSpeak Values and Channels
void PublishThingSpeakValues()
{
  // publish approximately every 5 sec
  if (EnableThingSpeak == true && WiFi.status() == WL_CONNECTED)
  {
    // Examples to Set the Fields with Values [fields 1-8 available, variable, Decimal Places]
    // Just uncomment the fields you wish to publish from this board.
    SetThingSpeakField(1, DCPower, 1);
    SetThingSpeakField(2, DCVoltage, 1);
    SetThingSpeakField(3, DCCurrentAccumulative, 1);
    SetThingSpeakField(4, BoardTemperatureC), 2;
    // SetThingSpeakField(5, DCPower, 1);
    // SetThingSpeakField(6, DCVoltage, 2);
    // SetThingSpeakField(7, DCCurrentAccumulative, 2);
    // SetThingSpeakField(8, BoardTemperatureC), 1;

    // NTP Time and Pre-Format Example Message for Channel Status Updates
    ThingSpeak.setStatus(HostName + " Last Test Publish: " + NTP_RTC);

    // Publish a Multi-Field Formatted ThingSpeak Channel.  Return Codes
    int x = ThingSpeak.writeFields(myChannelNumber1, myWriteAPIKey1);

    if (x == 200) // OK / Success
    {
      Serial.println("ThingSpeak Channel " + String(myChannelNumber1) + " Successfully Updated");
    }
    else if (x == 404) //	Incorrect API key (or invalid ThingSpeak server address)
    {
      // Red LED
      led.flash(RGBLed::RED, 20);

      Serial.println("ThingSpeak Channel " + String(myChannelNumber1) + " Not Updated. Key Error (" + String(x) + ")");

      // Update OLED
      oled.clear();
      OLEDPrint("Error 404", 2, 0);
      OLEDPrint("ThingSpeak", 2, 2);
      oled.update();
      delay(1000);
    }
    else if (x == -401) //	Point was not inserted (most probable cause is the rate limit of once every 15 seconds)
    {
      // Red LED
      led.flash(RGBLed::RED, 20);

      Serial.println("ThingSpeak Channel " + String(myChannelNumber1) + " Not Updated. Rate Error (" + String(x) + ")");

      // Update OLED
      oled.clear();
      OLEDPrint("Error 401", 2, 0);
      OLEDPrint("ThingSpeak", 2, 2);
      oled.update();
      delay(1000);
    }
    else
    {
      // Red LED
      led.flash(RGBLed::RED, 20);

      Serial.println("ThingSpeak Channel " + String(myChannelNumber1) + " Not Updated. Other Error (" + String(x) + ")");

      // Update OLED
      oled.clear();
      OLEDPrint("Error ?", 2, 0);
      OLEDPrint("ThingSpeak", 2, 2);
      oled.update();
      delay(1000);
    }

    Serial.println("");
  }

} // PublishThingSpeakValues
