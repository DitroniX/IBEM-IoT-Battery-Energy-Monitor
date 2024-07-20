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
#include <GyverOLED.h>

// ****************  VARIABLES / DEFINES / STATIC / STRUCTURES / CONSTANTS ****************

// OLED Instance. You will need to select your OLED I2C Display.   Uncomment/Comment as needed.
GyverOLED<SSD1306_128x64, OLED_BUFFER> oled;
// GyverOLED<SSD1306_128x32, OLED_NO_BUFFER> oled; // 0.96"
// GyverOLED<SSD1306_128x32, OLED_BUFFER> oled; // 0.96"
// GyverOLED<SSD1306_128x64, OLED_BUFFER> oled; // 0.96"
// GyverOLED<SSD1306_128x64, OLED_NO_BUFFER> oled; // 0.96"
// GyverOLED<SSH1106_128x64> oled; // 1.1" 1.3"

// **************** FUNCTIONS AND ROUTINES ****************

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

// Initialise OLED
void InitialiseOLED()
{
    if (OLEDEnabled == true)
    {
        Serial.println("Initialising OLED Display\n");

        oled.init(I2C_SDA, I2C_SCL);

        oled.setContrast(5); // 0..15
        oled.setPower(true); // Default True. Set to false to turn off display.

        // Set both to true or false to flip 180 degrees.
        oled.flipH(false); // default false
        oled.flipV(false); // default false

        // App and DitroniX
        oled.clear();
        OLEDPrint(AppAcronym, 4, 0);
        OLEDPrint("DitroniX ", 2, 4);
        OLEDPrint(".net", 2, 6);
        oled.update();
        delay(OLEDDelay * 1000);

        // App and Version
        oled.clear();
        OLEDPrint(AppAcronym, 4, 0);
        OLEDPrint("v " + AppVersion, 2, 6);
        oled.update();
        delay(OLEDDelay * 1000);

        // App and Location
        oled.clear();
        OLEDPrint(AppAcronym, 4, 0);
        OLEDPrint(LocationName, 2, 6);
        oled.update();
        delay(OLEDDelay * 1000);

        // App and Starting...
        oled.clear();
        OLEDPrint(AppAcronym, 4, 0);
        OLEDPrint("Starting..", 2, 6);
        oled.update();
        delay(OLEDDelay * 1000);
    }
} // InitialiseOLED

// Status OLED
void StatusOLED()
{
    if (OLEDEnabled == true)
    {
        // Status
        oled.clear();

        if (ADS1115Enabled == true)
        {
            ReadADC();

            CheckDCVINVoltage();

            // Readings
            char buff[10];
            dtostrf(BoardTemperatureC, 2, 1, buff);

            String header = AppAcronym + " " + buff + "C " + NTP_RTC;
            OLEDPrint(header, 1, 0);

            dtostrf(DCPower, 4, 1, buff);
            strcat(buff, " W");
            OLEDPrint(buff, 2, 2);

            dtostrf(DCVoltage, 4, 1, buff);
            strcat(buff, " V");
            OLEDPrint(buff, 2, 4);

            dtostrf(DCCurrentAccumulative, 4, 1, buff);
            strcat(buff, " A");
            OLEDPrint(buff, 2, 6);
        }
        else
        {
            oled.clear();
            OLEDPrint(AppAcronym, 4, 0);
            OLEDPrint("Error!", 2, 4);
            OLEDPrint("No ADC", 2, 6);
        }
        oled.update();

        delay(OLEDDelay * 1000);
    }
} // Status OLED

// Rough and Ready Underline Tesxt.
void PrintUnderline(String sText)
{
    int count = 0;
    Serial.println(sText);

    while (count <= sText.length())
    {
        Serial.print("─");
        count++;
    }
    Serial.println("");
} // PrintUnderline

// Equally Rough and Ready Dash Separator
void PrintSeparator(String sText)
{
    int count = 0;

    while (count <= (sText.length() / 2) + 1)
    {
        Serial.print("- ");
        count++;
    }
    Serial.println("");

    Serial.println(sText);
} // PrintSeparator