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
#include <OneWire.h>
#include <RGBLed.h>
#include <GyverOLED.h>

// OneWire DS18S20, DS18B20, DS1822 Temperature Example
//
// http://www.pjrc.com/teensy/td_libs_OneWire.html
//
// Options to explore DallasTemperature library
// https://github.com/milesburton/Arduino-Temperature-Control-Library

// ****************  VARIABLES / DEFINES / STATIC / STRUCTURES / CONSTANTS ****************

int sensorcount;

// **************** OUTPUTS ****************
#define LED_Red 10   // User RGB
#define LED_Green 18 // User RGB
#define LED_Blue 19  // User RGB

// **************** INPUTS ****************
#define DOW 6 // Dallas OneWire

// **************** DATA ****************

// Define I2C (Expansion Port)
#define I2C_SDA 4
#define I2C_SCL 5

// **************** INSTANCES ****************
// Define OneWire (Expansion Port)
OneWire ds(DOW); // OneWire Interface

// Define RDG Port
RGBLed led(LED_Red, LED_Green, LED_Blue, RGBLed::COMMON_CATHODE);

// OLED Instance. You will need to select your OLED I2C Display.   Uncomment/Comment as needed.
GyverOLED<SSD1306_128x64, OLED_BUFFER> oled;
// GyverOLED<SSD1306_128x32, OLED_NO_BUFFER> oled; // 0.96"
// GyverOLED<SSD1306_128x32, OLED_BUFFER> oled; // 0.96"
// GyverOLED<SSD1306_128x64, OLED_BUFFER> oled; // 0.96"
// GyverOLED<SSD1306_128x64, OLED_NO_BUFFER> oled; // 0.96"
// GyverOLED<SSH1106_128x64> oled; // 1.1" 1.3"

// **************** FUNCTIONS AND ROUTINES ****************

void LED_Test()
{ // Test RGB LEDs

    led.flash(RGBLed::RED, 250);
    led.flash(RGBLed::GREEN, 250);
    led.flash(RGBLed::BLUE, 250);
}

// Print Text to OLED.  Font 1-4.
void OLEDPrint(String TextS, int FontSize = 2, int PosY = 0, boolean FontCentre = true, float DisplayR = 0.6)
{
    int PosX;
    oled.setScale(FontSize);

    if (FontCentre == true)
    {
        // Font Fudge Factor
        unsigned int ValueCount = TextS.length() / 2;

        switch (FontSize)
        {
        case 1:                               // Small Font
            PosX = (58 - ((ValueCount * 5))); // Approximate Middle (Default 5)
            break;
        case 2:                                // Medium Font
            PosX = (58 - ((ValueCount * 10))); // Approximate Middle (Default 10)
            break;
        case 3:                                // Bit Larger Font
            PosX = (58 - ((ValueCount * 17))); // Approximate Middle (Default 17)
            PosY = 0;                          // Y can be 0 or 1.  Leave at 0.
            break;
        case 4:                                // Large Font
            PosX = (58 - ((ValueCount * 23))); // Approximate Middle (Default 23)
            PosY = 0;                          // Y needs to be 0.
            break;
        default:
            PosX = (58 - ((ValueCount * 5))); // Approximate Middle
            FontSize = 1;
            PosY = 0;
            break;
        }

        if (PosX < 0)
            PosX = 0;

        oled.setCursor(PosX, PosY);
        oled.setScale(FontSize);
        oled.print(TextS);
    }
} // Print Text to OLED.  Font 1-4.

// **************** SETUP ****************
void setup()
{
    // Stabalise
    delay(250);

    // Initialise UART
    Serial.begin(115200, SERIAL_8N1); // 115200
    while (!Serial)
        ;
    Serial.println("");

    // OLED
    oled.init(I2C_SDA, I2C_SCL);

    // Hardware Tests
    LED_Test();
}

void loop(void)
{
    byte i;
    byte present = 0;
    byte type_s;
    byte data[9];
    byte addr[8];
    float celsius, fahrenheit;

    OLEDPrint("Sensor Readings", 1, 0);
    sensorcount++;

    if (!ds.search(addr))
    {
        oled.update();
        Serial.println("\n---------------------------\n");
        ds.reset_search();
        delay(1000);
        oled.clear();
        sensorcount = 0;
        return;
    }

    Serial.print("ROM =");
    for (i = 0; i < 8; i++)
    {
        Serial.write(' ');
        Serial.print(addr[i], HEX);
    }

    if (OneWire::crc8(addr, 7) != addr[7])
    {
        Serial.println("CRC is not valid!");
        return;
    }

    // the first ROM byte indicates which chip
    switch (addr[0])
    {
    case 0x10:
        Serial.println("\tChip = DS18S20"); // or old DS1820
        type_s = 1;
        break;
    case 0x28:
        Serial.println("\tChip = DS18B20");
        type_s = 0;
        break;
    case 0x22:
        Serial.println("\tChip = DS1822");
        type_s = 0;
        break;
    default:
        Serial.println("\tDevice is not a DS18x20 family device.");
        return;
    }

    ds.reset();
    ds.select(addr);
    ds.write(0x44, 1); // start conversion, with parasite power on at the end

    delay(1000); // maybe 750ms is enough, maybe not
    // we might do a ds.depower() here, but the reset will take care of it.

    present = ds.reset();
    ds.select(addr);
    ds.write(0xBE); // Read Scratchpad

    Serial.print("  Data = ");
    Serial.print(present, HEX);
    Serial.print(" ");
    for (i = 0; i < 9; i++)
    { // we need 9 bytes
        data[i] = ds.read();
        Serial.print(data[i], HEX);
        Serial.print(" ");
    }
    Serial.print(" CRC=");
    Serial.print(OneWire::crc8(data, 8), HEX);
    Serial.println();

    // Convert the data to actual temperature because the result is a 16 bit signed integer.
    // It should be stored to an "int16_t" type, which is always 16 bits even when compiled on a 32 bit processor.
    int16_t raw = (data[1] << 8) | data[0];
    if (type_s)
    {
        raw = raw << 3; // 9 bit resolution default
        if (data[7] == 0x10)
        {
            // "count remain" gives full 12 bit resolution
            raw = (raw & 0xFFF0) + 12 - data[6];
        }
    }
    else
    {
        byte cfg = (data[4] & 0x60);
        // at lower res, the low bits are undefined, so let's zero them
        if (cfg == 0x00)
            raw = raw & ~7; // 9 bit resolution, 93.75 ms
        else if (cfg == 0x20)
            raw = raw & ~3; // 10 bit res, 187.5 ms
        else if (cfg == 0x40)
            raw = raw & ~1; // 11 bit res, 375 ms
                            //// default is 12 bit resolution, 750 ms conversion time
    }
    celsius = (float)raw / 16.0;
    fahrenheit = celsius * 1.8 + 32.0;
    Serial.print("  Temperature:  ");
    Serial.print(celsius);
    Serial.print(" Celsius\t");
    Serial.print(fahrenheit);
    Serial.println(" Fahrenheit\n");

    OLEDPrint(String(sensorcount) + ": Temp " + String(celsius) + " C", 1, sensorcount + 1);

    // Heatbeat LED
    led.flash(RGBLed::RED, 20);
    // delay (1000);
}