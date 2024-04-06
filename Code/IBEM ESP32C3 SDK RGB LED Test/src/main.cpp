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

// ****************  VARIABLES / DEFINES / STATIC / STRUCTURES / CONSTANTS ****************

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

  // ** Note: Below not needed if using the RGBLed Library **

  // Configures the specified LED GPIO as outputs.
  // pinMode(LED_Red, OUTPUT);
  // pinMode(LED_Green, OUTPUT);
  // pinMode(LED_Blue, OUTPUT);

  // // LEDs Default Off State
  // digitalWrite(LED_Red, LOW);
  // digitalWrite(LED_Green, LOW);
  // digitalWrite(LED_Blue, LOW);
}

// **************** LOOP ****************
void loop(void)
{

  // Manually Test RGB LEDs

  // // Red
  // digitalWrite(LED_Red, HIGH);
  // delay(500);
  // digitalWrite(LED_Red, LOW);
  // delay(100);

  // // Green
  // digitalWrite(LED_Green, HIGH);
  // delay(500);
  // digitalWrite(LED_Green, LOW);
  // delay(100);

  // // Blue
  // digitalWrite(LED_Blue, HIGH);
  // delay(500);
  // digitalWrite(LED_Blue, LOW);
  // delay(100);

  // // White
  // digitalWrite(LED_Red, HIGH);
  // digitalWrite(LED_Green, HIGH);
  // digitalWrite(LED_Blue, HIGH);
  // delay(500);
  // digitalWrite(LED_Red, LOW);
  // digitalWrite(LED_Green, LOW);
  // digitalWrite(LED_Blue, LOW);
  // delay(1000);

  // Using RGB Library for flashing and controlling the RGB LED

  // Reset Brigthness Back To 100%
  led.brightness(RGBLed::RED, 100);
  led.brightness(RGBLed::GREEN, 100);
  led.brightness(RGBLed::BLUE, 100);

  // ## FLASHING
  led.flash(RGBLed::RED, 500);
  delay(250);
  led.flash(RGBLed::GREEN, 500);
  delay(250);
  led.flash(RGBLed::BLUE, 500);
  delay(1000);

  // Heatbeat
  led.flash(RGBLed::RED, 50);
  delay(500);
  led.flash(RGBLed::GREEN, 50);
  delay(500);
  led.flash(RGBLed::BLUE, 50);
  delay(1000);

  // ## FADES
  //  Fade out red color, with 5 steps during 100ms
  led.fadeOut(RGBLed::RED, 5, 1000);
  delay(250);
  // Fade out green color, with 5 steps during 100ms
  led.fadeOut(0, 255, 0, 5, 1000);
  delay(250);
  // Fade in red color, with 5 steps during 100ms
  led.fadeIn(RGBLed::RED, 5, 1000);
  delay(250);
  // Fade in green color, with 5 steps during 100ms
  led.fadeIn(0, 255, 0, 5, 1000);
  delay(1000);

  // ## BRIGHTNESS
  //  Color is red with brigthness 100%
  led.setColor(RGBLed::RED);
  delay(1000);

  // Color is red with brigthness 10%
  led.brightness(RGBLed::RED, 10);
  delay(1000);

  // Light off RGB led
  led.off();

  delay(1000);
}
