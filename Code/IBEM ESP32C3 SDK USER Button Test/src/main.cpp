/*
  Dave Williams, DitroniX 2019-2024 (ditronix.net)
  Full header information in README

  This test code is OPEN SOURCE and formatted for easier viewing.  Although is is not intended for real world use, it may be freely used, or modified as needed.
  It is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.

  Further information, details and examples can be found on our website and also GitHub wiki pages:
  * github.com/DitroniX
  * github.com/DitroniX/IBEM-IoT-Battery-Energy-Monitor
  * github.com/DitroniX/IBEM-IoT-Battery-Energy-Monitor/wiki
  * hackster.io/DitroniX/ibem-esp32c3-iot-battery-energy-monitor-solar-inverters-0342b12b1
*/

// Libraries
#include <Arduino.h>
#include <RGBLed.h> //https://github.com/wilmouths/RGBLed

// ****************  VARIABLES / DEFINES / STATIC / STRUCTURES / CONSTANTS ****************

// ******************************** INPUTS ********************************
#define User_Button 9 // User Button also Manual PGM

// ******************************** OUTPUTS ********************************
#define LED_Red 10   // User RGB
#define LED_Green 18 // User RGB
#define LED_Blue 19  // User RGB

// **************** INSTANCES ****************

RGBLed led(LED_Red, LED_Green, LED_Blue, RGBLed::COMMON_CATHODE);

// **************** FUNCTIONS AND ROUTINES ****************

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

  // Button Input.  INPUT_PULLUP Optional as board has a pullup resistor
  pinMode(User_Button, INPUT);
}

// **************** LOOP ****************
void loop(void)
{

  // Basic Check for Button Press (Non Interrupt Driven)
  if (digitalRead(User_Button) == LOW)
  {
    led.flash(RGBLed::RED, 2000);
  }

  // Heatbeat
  led.flash(RGBLed::BLUE, 500);
}
