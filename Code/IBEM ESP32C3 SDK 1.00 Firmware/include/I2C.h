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
#include <Wire.h>

// **************** FUNCTIONS AND ROUTINES ****************

// Initialize I2C
void InitialiseI2C()
{
    Wire.begin(I2C_SDA, I2C_SCL);
    Wire.setClock(I2C_CLOCK); // 400kHz | Standard-mode (Sm) and Fast-mode (Fm) which can go up to 100kHz and 400kHz respectively
} // InitialiseI2C

// I2C Bus Scanner
void ScanI2CBus()
{

    byte error, address;
    int nDevices;

    Serial.println("Scanning I2C Bus for Devices for Auto Configuration ...");

    nDevices = 0;
    for (address = 1; address < 127; address++)
    {
        Wire.beginTransmission(address);
        error = Wire.endTransmission();
        nDevices++;
        if (nDevices == 1)
            Serial.println(" I2C Device(s) found at Address:");
        if (error == 0)
        {
            Serial.print(" * Hex 0x");
            Serial.print(address, HEX);

            Serial.print(" (");
            if (address < 16)
            {
                Serial.print("0");
            }
            Serial.print(address);
            Serial.print(")\t");

            switch (address)
            {
            case 0x38:
                Serial.print(" Humidity/Temperature Sensor");

                break;
            case 0x48 ... 0x4B:
                Serial.print(" ADS1115 16-bit ADC");

                ADS1115Enabled = true;
                break;
            case 0x4C ... 0x4F:
                Serial.print(" Sensor");

                break;
            case 0x3C ... 0x3D:
                Serial.print(" OLED");

                OLEDEnabled = true;
                break;
            case 0x50 ... 0x5F:
                Serial.print(" EEPROM");

                EEPROMEnabled = true;
                break;
            case 0x76 ... 0x77:
                Serial.print(" BMP/BME Temerature/Humidity/Barometric");

                break;
            }
            Serial.println();
        }
        else if (error == 4)
        {
            Serial.print(" * Unknown error at address Decimal ");
            if (address < 16)
            {
                Serial.print("0");
            }
            Serial.print(address);
            Serial.print(" = Hex 0x");
            Serial.println(address, HEX);
        }
    }
    if (nDevices == 0)
    {
        Serial.println(" * No I2C devices found. Possible Hardware Issue?");
    }
    else
    {
        Serial.println(" I2C Bus Scan Complete\n");
    }
    Wire.endTransmission();
} // ScanI2CBus