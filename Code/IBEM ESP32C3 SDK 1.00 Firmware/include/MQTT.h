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
#include <PubSubClient.h> // https://pubsubclient.knolleary.net/api

// ****************  VARIABLES / DEFINES / STATIC / STRUCTURES ****************

// MQTT Object / Client Instance
PubSubClient mqtt_client(wlan_client);

// **************** FUNCTIONS AND ROUTINES ****************

// Callback for Receiving MQTT Messages
void MQTTCallback(char *topic, byte *message, unsigned int length)
{
    Serial.print("MQTT Message arrived on topic: ");
    Serial.print(topic);
    Serial.print(". Message: ");
    String messageTemp;

    for (int i = 0; i < length; i++)
    {
        Serial.print((char)message[i]);
        messageTemp += (char)message[i];
    }
    Serial.println();
} // MQTTCallback

// Publish a Formatted MQTT Message
void PublishMQTTMessage(String SensorName, float SensorValue, int dPlaces = 1, int dWidth = 5, int dQOS = 0)
{
    if (mqtt_client.connect("wlan_client", MQTTUser, MQTTPassword))
    // if (mqtt_client.connected)
    {

        // Green LED
        led.flash(RGBLed::GREEN, 50);

        // Format Sensor Message to MQTTTopic
        String SensorMessage = MQTTTopicMain + "/" + SensorName;
        int str_len = SensorMessage.length() + 1;
        char MQTTTopic[str_len];
        SensorMessage.toCharArray(MQTTTopic, str_len);

        // Structure MQTT Payload
        char MQTTPayload[10];
        dtostrf(SensorValue, dWidth, dPlaces, MQTTPayload);

        // Update Serial Monitor
        Serial.print("Sending MQTT Message: ");
        Serial.print(MQTTTopic);
        Serial.print("\t");
        Serial.println(MQTTPayload);

        // Publish
        mqtt_client.publish(MQTTTopic, MQTTPayload);
        // mqtt_client.publish("ipem/test/1", 0);
    }
    else
    {
        // Red LED
        led.flash(RGBLed::RED, 50);

        Serial.println("WiFi or MQTT Broker Not Connected");

        // Update OLED
        oled.clear();
        OLEDPrint("Error", 2, 0);
        OLEDPrint("MQTT", 2, 2);
        OLEDPrint("Server", 2, 4);
        oled.update();
        delay(1000);

        // Stabalise for slow Access Points
        InitialiseWiFi();
    }
} // PublishMQTTMessage

// Initialise MQTT
void InitialiseMQTT()
{
    if (EnableMQTT == true && WiFi.status() == WL_CONNECTED)
    {
        // Initialise MQTT Connection and Instance
        mqtt_client.setServer(MQTTServer, MQTTPort);

        // if (EnableMQTTCallBack == true)
        //     mqtt_client.setCallback(MQTTCallback);

        Serial.println("Initialised MQTT");
        Serial.println("MQTT Enabled - Register Values Will be Published");

        // mqtt_client.subscribe("Light/Switch");
        // mqtt_client.setCallback(MQTTCallback);
    }
    else
    {
        Serial.println("MQTT Publishing is Disabled");

        // Red LED
        led.flash(RGBLed::RED, 20);
    }
} // InitialiseMQTT

// Publish MQTT Values
void PublishMQTTValues()
{
    // Test Message Example.
    // PublishMQTTMessage("DCCurrentAccumulative", DCCurrentAccumulative, 0);
    // PublishMQTTMessage("DCPower", DCPower);
    // PublishMQTTMessage("DCVoltage", DCVoltage, 0);

    // PublishMQTTMessage("BoardTemperatureC", BoardTemperatureC, 0);
    // PublishMQTTMessage("BoardTemperatureF", BoardTemperatureF, 0);

    // PublishMQTTMessage("ProbeTemperatureC", ProbeTemperatureC, 0);
    // PublishMQTTMessage("ProbeTemperatureF", ProbeTemperatureF, 0);

    Serial.println("");

} // PublishMQTTValues

// while (!client.connected())
// {
//     Serial.println("Connecting to MQTT...");
//     if (client.connect("ESP32Client", mqttUser, mqttPassword))
//     {
//         Serial.println("connected " + String(client.state()));
//         client.subscribe(topic, "XXXX");
//     }
//     else
//     {
//         // Red LED
//         led.flash(RGBLed::RED, 20);
//         Serial.print("failed with state ");
//         Serial.print(client.state());
//         delay(2000);
//     }
// }
