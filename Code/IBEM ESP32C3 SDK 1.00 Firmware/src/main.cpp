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
#include <Config.h> // User Configuration Variables

#include <I2C.h>
#include <ADC.h>
#include <EEPROM.h>
#include <OLED.h>
#include <Meter.h>
#include <LED.h>
#include <OneWireSensor.h>
#include <RebootMCU.h>

#include <WiFi-OTA.h>
#include <NTP.h>

#include <Domoticz.h>
#include <MQTT.h>
#include <ThingSpeakIoT.h>

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

  PrintUnderline("Hardware Firmware");

  // Heading
  Serial.println(AppAcronym + " ESP32C3 Firmware Version: " + AppVersion);
  Serial.println("Location: \t" + LocationName);
  Serial.printf("Chip Model: \t%s\n", ESP.getChipModel());
  Serial.printf("Chip SN ID: \t%04X", (uint16_t)(chipid >> 32));
  Serial.printf("%08X\n", (uint32_t)chipid);
  Serial.println("\n");

  // Initialise Hardware
  PrintUnderline("Initialise");

  // Configure GPIO Inputs
  pinMode(ALRT, INPUT);
  pinMode(User_Button, INPUT);
  pinMode(VZCR, INPUT);

  // Initialise I2C
  InitialiseI2C();

  // Initialise OneWire
  Initialise_OneWire();
  ReadOneWire();

  // Hardware Tests
  LED_Test();
  ScanI2CBus();

  // Initialize EEPROM (Format if needed)
  if (EEPROMEnabled == true)
    InitialiseEEPROM();

  // Initialize OLED
  if (OLEDEnabled == true)
    InitialiseOLED();

  // Initialise WiFi OTA NTP
  InitialiseWiFi();
  InitialiseNTP();
  UpdateNTP();
  InitialiseWebServer();

  // Initialise Publishing to Domoticz MQTT ThingSpeak.  Comment out sections if needed.
  InitialiseDomoticz();
  InitialiseMQTT();
  InitialiseThingSpeak();

  // Initialise PWM for Meter or Safety Isolator
  Initialise_PWM(PWM_Meter_GPIO);

  // Initialise ADS1115
  if (ADS1115Enabled == true)
  {
    // ads.begin();
    Initialise_ADS1115_ADC();

    // ADC Test
    PrintUnderline("ADC Test");
    ReadADC();
    CheckDCVINVoltage();
  }

  StatusOLED();
}

// **************** LOOP ****************
void loop()
{
  // Basic Handler for WebServer
  if (WiFiEnabled == true)
  {
    CheckOTA();
  }

  // Check if Reboot Needed
  RebootCheck();

  // Update RTC
  ReadTime();

  // Sensors
  ReadADC();
  ReadOneWire();
  Serial.println("---------------------------------\n");

  StatusOLED();

  // Publish values to Domoticz
  if (WiFiEnabled == true)
  {
    PublishDomoticzValues();
    PublishThingSpeakValues();
  }

  // Check for Button Press
  if (digitalRead(User_Button) == LOW)
  {
    Serial.println("Button Pressed");
    led.flash(RGBLed::GREEN, 250);
    LED_Test();
    TestADS1115();
    delay(2000);
  }

  // Heatbeat LED
  if (WiFi.status() == WL_CONNECTED)
    led.flash(RGBLed::BLUE, 20);
  else
    led.flash(RGBLed::RED, 20);

  // Loop Delay
  delay(LoopDelay * 1000);
}
