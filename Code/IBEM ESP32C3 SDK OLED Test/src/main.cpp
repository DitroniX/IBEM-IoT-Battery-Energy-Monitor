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
#include <RGBLed.h> //https://github.com/wilmouths/RGBLed
#include <GyverOLED.h>

// ****************  VARIABLES / DEFINES / STATIC / STRUCTURES / CONSTANTS ****************

// ******************************** DATA ********************************

// Define I2C (Expansion Port)
#define I2C_SDA 4
#define I2C_SCL 5
#define I2C_CLOCK 4000000UL

// ******************************** OUTPUTS ********************************
#define LED_Red 10   // User RGB
#define LED_Green 18 // User RGB
#define LED_Blue 19  // User RGB

// **************** INSTANCES ****************

RGBLed led(LED_Red, LED_Green, LED_Blue, RGBLed::COMMON_CATHODE);

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
    case 1:                             // Small Font
      PosX = (58 - ((ValueCount * 5))); // Approximate Middle (Default 5)
      break;
    case 2:                              // Medium Font
      PosX = (58 - ((ValueCount * 10))); // Approximate Middle (Default 10)
      break;
    case 3:                              // Bit Larger Font
      PosX = (58 - ((ValueCount * 17))); // Approximate Middle (Default 17)
      PosY = 0;                          // Y can be 0 or 1.  Leave at 0.
      break;
    case 4:                              // Large Font
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

  // Initialise I2C
  Wire.begin(I2C_SDA, I2C_SCL);
  Wire.setClock(I2C_CLOCK); // 400kHz | Standard-mode (Sm) and Fast-mode (Fm) which can go up to 100kHz and 400kHz respectively

  Serial.println("Initialising OLED Display\n");
  oled.init(I2C_SDA, I2C_SCL);
}

// **************** LOOP ****************
void loop(void)
{

  // App and DitroniX
  oled.clear();
  OLEDPrint("IBEM", 4, 0);
  OLEDPrint("DitroniX ", 2, 4);
  OLEDPrint(".net", 2, 6);
  oled.update();
  delay(1000);

  // App and Version
  oled.clear();
  OLEDPrint("IBEM", 4, 0);
  OLEDPrint(".. Hello", 2, 4);
  OLEDPrint("World ..", 2, 6);
  oled.update();
  delay(1000);

  // Heatbeat
  led.flash(RGBLed::RED, 50);

  delay(1000);
}
