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

// ****** THIS ONEWIRE IS UNDER DEVELOPMENT - AWAITING PRODUCTION BOARD AND IS NOT CURRENTLY IN USE. ******

#include <OneWire.h>
#include <DallasTemperature.h>

// ****************  VARIABLES / DEFINES / STATIC / STRUCTURES / CONSTANTS ****************

// OneWire Bus and DS18B20 Port
OneWire ds(DOW);
DallasTemperature sensors(&ds);

// arrays to hold device address
DeviceAddress SensorAddress;

// **************** FUNCTIONS AND ROUTINES ****************

// Initialise OneWire Bus
void Initialise_OneWire()
{
    byte i;
    byte addr[8];
    int x;

    OneWireEnabled = false;

    // Locate OneWire Devices
    Serial.print("\nScanning for OneWire Device(s) ... ");
    sensors.begin();
    Serial.print("Found ");
    SensorCount = sensors.getDeviceCount();
    Serial.print(SensorCount, DEC);
    Serial.println(" device(s):");

    while (ds.search(addr))
    {
        OneWireEnabled = true;
        x++;
        Serial.print(" * [" + String(x) + "] ");

        for (i = 0; i < 8; i++)
        {
            if (addr[i] < 16)
            {
                Serial.print('0');
            }
            Serial.print(addr[i], HEX);
            if (i < 7)
            {
                Serial.print(":");
            }
        }
        if (OneWire::crc8(addr, 7) != addr[7])
        {
            Serial.print("(OneWire Device CRC is Not Valid!)\n");
        }

        if (addr[0] == 0x10)
        {
            Serial.print("\t(DS18S20 Family Device)\n");
        }
        else if (addr[0] == 0x28)
        {
            Serial.print("\t(DS18B20 Family Device)\n");
        }
        else
        {
            Serial.print("\t(Device Family Unknown)");
            Serial.println(addr[0], HEX);
        }
    }

    Serial.print("\n");

    // ds.reset_search();
    // sensors.begin();
}

// Format Sensor Address and Print
void printAddress(DeviceAddress deviceAddress)
{
    for (uint8_t i = 0; i < 8; i++)
    {
        if (deviceAddress[i] < 16)
            Serial.print("0");
        Serial.print(deviceAddress[i], HEX);
    }
}

// Display First Sensor Readings
void ReadOneWire()
{
    if (OneWireEnabled == true)
    {
        // Read First Sensor Only
        sensors.getAddress(SensorAddress, 0);
        sensors.requestTemperatures();

        // Store Readings
        ProbeTemperatureC = sensors.getTempC(SensorAddress);
        ProbeTemperatureF = sensors.getTempF(SensorAddress);

        // Display Values
        Serial.print("Probe ");
        printAddress(SensorAddress);
        Serial.print(" Temperature:\t");
        Serial.print(ProbeTemperatureC);
        Serial.print(" ºC\t");
        Serial.print(ProbeTemperatureF);
        Serial.println(" ºF\n");
    }
    else
    {
        ProbeTemperatureC = 0;
        ProbeTemperatureF = 0;
    }
}
