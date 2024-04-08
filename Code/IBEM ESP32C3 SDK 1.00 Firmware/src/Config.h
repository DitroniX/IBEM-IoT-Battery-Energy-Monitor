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

// ****************  GLOBAL VARIABLES / DEFINES / STATIC / STRUCTURES / CONSTANTS ****************

// ******************************** Main ********************************

// User Constants
int LoopDelay = 1; // Loop Delay in Seconds
int OLEDDelay = 2; // OLED Delay in Seconds

// Application Constants
const String AppVersion = "240408"; // Internal Firmware Date Code
const String AppAcronym = "IBEM";   // Board Acronym - Do Not Change
const String AppName = AppAcronym + " ESP32-C3 IoT Battery Energy Monitor";
const String LocationName = "Battery 1"; // Enter Name Location of Device. Used for Serial Monitor and OLED.
const String FirmwarePath = "IBEM ESP32C3 SDK 1.00 Firmware > pio > build > esp32-c3-devkitm-1 > firmware.bin";

// ******************************** ADC ********************************

// Variables User
const int AverageSamples = 20; // Average Multi-Samples on each Channel Read.  (Default 20)
const int AverageDelay = 20;   // Average Inter Multi-Sample ms Delay.  (Default 20)

// Voltage User
const float DCVoltageThreshold = 6.00; // Minimum Voltage > USB - removing noise. (Default 6.00)
const float DCVoltageOffset = 0.00;    // Battery Voltage Zero Calibration Offset.  (Default 0.00)

// Current User
const float DCCurrentThreshold = 0.20; // Minimum Current - removing noise.  (Default 0.20)
const float DCCurrentOffset = 0;       // Battery Current Zero Calibration Offset [Remember the InvertReading option below]
const boolean InvertReading = false;   // Invert Current and Power (Default false) - * Default = IBEM Negative to Battery Negative *

// Externs - No Change
float DCCurrent1;               // Current Sensor 1
float DCCurrent2;               // Current Sensor 2
float DCVoltage;                // Battery or DC Input Voltage Sensor
float DCCurrentOffsetEEPROM;    // Stored DC Current Offset from EEPROM
float DCCurrentAccumulative;    // Accumulative Current (1+2) Formulated
float DCCurrentAccumulativeRaw; // Accumulative Current (1+2) Raw
float DCPower;                  // Accumulative Power ((1+2) * DCVoltage)
float BoardTemperatureC;        // PCB Temperature NTC C
float BoardTemperatureF;        // BoardTemperature NTC F
float BoardTemperatureK;        // BoardTemperature NTC K

// ******************************** WiFi ********************************

// WiFi.  Setup with your Wireless Information.  Add more as needed.
const char *ssid1 = "";     // WiFi Network SSID - Case Sensitive
const char *password1 = ""; // WiFi Network password - Case Sensitive
const char *ssid2 = "";     // WiFi Network SSID - Case Sensitive
const char *password2 = ""; // WiFi Network password - Case Sensitive
const char *ssid3 = "";     // WiFi Network SSID - Case Sensitive
const char *password3 = ""; // WiFi Network password - Case Sensitive

// WiFi. Force Disable for Testing.  !!!!! BEWARE Outside of Local Developmet Do NOT Disable as Network OTA will NOT work !!!!!
const boolean WiFiEnabled = true; // Enable/Disable WiFi for Local USB Development and Testing Only.  You can leave SSID/IP etc populated.

// Set your static or fixed IP address and gateway - Alternatively leave at (0, 0, 0, 0) for standard DHCP
IPAddress local_IP(0, 0, 0, 0);     // Leave at (0, 0, 0, 0) if DHCP required.  (Default 0, 0, 0, 0)
IPAddress gateway(0, 0, 0, 0);      // Set to your Router IP = Gateway.  (Default 0, 0, 0, 0)
IPAddress subnet(255, 255, 255, 0); // Should not need to change subnet from default (255, 255, 255, 0)

// Optional DNS Defaults.
IPAddress primaryDNS(0, 0, 0, 0);   // Defaults to your above Gateway IP if left as (0, 0, 0, 0)
IPAddress secondaryDNS(8, 8, 4, 4); // For Google Public DNS use for Primary or Secondary (8,8,8,8) and/or (8,8,4,4)

// WiFi Other User Options
int WifiMaxAPScan = 5;                   // Multiple WiFi Scan Maximum APs (Default 5)
const uint32_t connectTimeoutMs = 10000; // WiFi connect timeout per AP. Increase when connecting takes longer. (Default 10000)

// WebServer User Options
boolean EnableAutoWebRefresh = true; // Change to false, to stop WebServer Page Auto Refresh.  (Default true)

// Externs - No Change
String HostName;    // Hostname
int RSSI_Level;     // Wifi RSSI Signal Level
String RSSI_Detail; // Wifi RSSI Signal Information
String MACAddress;  // MAC Address
String WiFi_SSID;   // WiFi SSID

// ******************************** MQTT ********************************

// Enable Publishing
boolean EnableMQTT = false; // Change to true, to enable Loop reading and sending data to MQTT.  (Default false)

// MQTT Server info.  Setup with your MQTT Configration
const char *MQTTServer = "0.0.0.0"; // MQTT Server IP Address (Typically a Fixed Local Address).  (Default 0.0.0.0)
int MQTTPort = 1883;                // MQTT Network Port (Default 1883)
const char *MQTTUser = "";          // MQTT User - if applicable
const char *MQTTPassword = "";      // MQTT Password - if applicable
String MQTTTopicMain = "ibem";      // MQTT Default Topic. Exclude the trailing .  (Default ibem)

// Callback
boolean EnableMQTTCallBack = false; // Under Development. (Default false)

// ******************************** Domoticz ********************************

// Enable Publishing
const boolean EnableDomoticz = false; // Change to true, to enable Loop reading and sending data to Domoticz.  (Default false)

// Domoticz Server info.  Setup with your Domoticz IP and Port
const char *DomoticzServer = "0.0.0.0"; // Domoticz Server IP Address (Typically a Fixed Local Address)  (Default 0.0.0.0)
const int DomoticzPort = 8080;          // Domoticz Network Port (Default 8080)
const char *Domoticz_User = "";         // Domoticz User - if applicable  (Not currently implimented in the connection)
const char *Domoticz_Password = "";     // Domoticz Password - if applicable  (Not currently implimented in the connection)

// Domoticz Hardware Device Indexes

// Set these values to the Domoticz Devices Indexes (IDX).  If Zero, then entry is ignored.
// Remember a device first needs to be created in Domoticz, then use related IDX number here.   i.e.  129 = Domoticz hardware index 129.
const int idxDCPower = 0;               // DCPower
const int idxDCVoltage = 0;             // PCB DC Input (Derived from Battery)
const int idxDCCurrentAccumulative = 0; // DCCurrentAccumulative

const int idxBoardTemperatureC = 0; // PCB NTC °C
const int idxBoardTemperatureF = 0; // PCB NTC °F

const int idxProbeTemperatureC = 0; // Battery NTC °C
const int idxProbeTemperatureF = 0; // Battery NTC °F

// Set this value to the Domoticz Device Group Index (IDX) - Note: Currently Unused Virtual Device.
const int idxDomoticzBase = 0; // If Zero, then entry is ignored.  Group device needs to be created in Domoticz. WIP.

// ******************************** ThingSpeak ********************************

// Enable Publishing
boolean EnableThingSpeak = false; // Change to true, to enable Loop reading and sending data to ThingSpeak.  (Default false).

// ThingSpeak Server info.  Setup with your ThingSpeak Configuration
unsigned long myChannelNumber1 = 0; // ThingSpeak Channel ID 1 (Default 0)
const char *myWriteAPIKey1 = "";    // ThingSpeak Write API Key for Channel ID 1
unsigned long myChannelNumber2 = 0; // ThingSpeak Channel ID 2 (Default 0)
const char *yWriteAPIKey2 = "";     // ThingSpeak Write API Key for Channel ID 2
unsigned long myChannelNumber3 = 0; // ThingSpeak Channel ID 3 (Default 0)
const char *myWriteAPIKey3 = "";    // ThingSpeak Write API Key for Channel ID 3
unsigned long myChannelNumber4 = 0; // ThingSpeak Channel ID 4 (Default 0)
const char *myWriteAPIKey4 = "";    // ThingSpeak Write API Key for Channel ID 4

// ******************************** Meter ********************************

// Development and testing Use.  EnablePWMTestOutput will over-ride EnablePWMLocal and EnablePWMRemote
boolean EnablePWMTestOutput = false; // Enable/TurnOn Continuous PWM Cycle Test.  Will Bypass Actual Readings. Default(false)
const int PWM_TestOutput = 0;        // Base Test Routine Output (If EnablePWMTestOutput is True.  Options 0-3. Default 0)

// Constants Power USER - These values work in hand with Input Power and PWM Voltage Output Ranges / Mapping
const float PWM_PowerThreshold = 5;     // Minimum Power Threshold (in Watts), Before Outputting PWM.  Also used for Noise and Jitter Control. (Default 5)
const float PWM_PowerUpperLimit = 4000; // Maximum CT Power Upper Range DAC_MinimumPowerThreshold to x (in Watts). Upper Limit of Measured Power for Scaling (Default 4000)

// Constants PWM Duty Cycle USER - These values work in hand with Input Power and PWM Duty Cycle Output Ranges / Mapping
const float PWM_DutyCycleOutputThreshold = 0.0; // Duty Cycle Range Minimum Before Outputting from DAC. (Default 0.0 - Minimum 0)

// Constants Power Percentage USER - These values work in hand with Input Power and DAC PM Output Ranges / Mapping
const int PWM_PercentageMinimum = 0;   // Power Percentage Minimum Displayed. (Default 0)
const int PWM_PercentageMaximum = 100; // Power Percentage Maximum Displayed. (Default 100)

// Constants PWM ESP32C3
const int PWM_ChannelOut = 0;     // Output Channel 0-15. (Default 0)
const int PWM_FrequencyHz = 1000; // PWM Default Frequency (Default 1000).  PWM Frequency Range 10Hz to 40MHz.
const int PWM_ResolutionBit = 10; // PWM bit Range 1-16 bits (Default 10) 8bit = 256-1 | 10bit = 1024-1 | 12bit = 4096-1 | 14bit = 16384-1 | 16bit = 65536-1
const int PWM_PCA_ChannelOut = 0; // Output Channel 0-15. (Default 0)

// Externs - No Change
float PWMPowerOutput;                // Last Power Value Used For DAC
float PWMDutyCycleOutput;            // Last Duty Cycle Value Used For DAC
int PWMChannelOutput;                // Last Channel Value Used For DAC
boolean PWMOutputOff;                // Last DAC Output Off Base State
int PWMPowerPercentage;              // Base Power Percentage
float PWM_DutyCycleOutputUpperLimit; // // PWM Maximum Duty Cycle Based on ResolutionBit (Maximum 100.00)
int PWM_MaximumResolution;           // 8bit = 256-1 | 10bit = 1024-1 | 12bit = 4096-1 | 14bit = 16384-1 | 16bit = 65536-1.

// ******************************** ReBoot ********************************

// 0 reset disabled
// 300000UL call reset every 300 seconds (5 Minutes)
// 3600000UL call reset every 60 mins (1 Hour)
// 86400000UL call reset every 24 hours (1 Day)
// 604800000UL call reset every 7 days (1 Week).
// 2592000000UL call reset every 30 days (1 Month)
time_t RebootTime = 604800000UL; // Configure as needed. (Default 604800000UL)

// ******************************** NTP ********************************

// Externs - No Change
String NTP_RTC;
time_t timenow;

// ******************************** I2C ********************************

// Externs - No Change
boolean EEPROMEnabled = false;  // Auto Configuration Use - Do  Not Change
boolean ADS1115Enabled = false; // Auto Configuration Use - Do  Not Change
boolean OLEDEnabled = false;    // Auto Configuration Use - Do  Not Change

// Externs - No Change
float ProbeTemperatureC; // Battery Temperature Probe C
float ProbeTemperatureF; // Battery Temperature Probe F
int SensorCount;         // OneWire Sensor Count
boolean OneWireEnabled;  // OneWire Auto Config

// ******************************** DATA ********************************

// Define I2C (Expansion Port)
#define I2C_SDA 4
#define I2C_SCL 5
#define I2C_CLOCK 4000000UL

// Define OneWire (Expansion Port)
#define DOW 6 // Dallas OneWire (PROTO = 3, Production = 6)

// Default IBEM ADS1115 Address
#define IBEM_ADS1115 0x48

// ******************************** INPUTS ********************************
#define ALRT 8        // ADS1115 Alert Out
#define User_Button 9 // User Button also Manual PGM
#define VZCR 7        // Current Sensor Zero Crossing Out (Not Used)

// ******************************** OUTPUTS ********************************
#define LED_Red 10       // User RGB
#define LED_Green 18     // User RGB
#define LED_Blue 19      // User RGB
#define PWM_Meter_GPIO 3 // DCMeter (PWM)

// ******************************** RTC ********************************

// # RTC Clock Config WIP
// #
// # CONFIG_RTC_CLK_SRC_INT_RC is not set
// # CONFIG_RTC_CLK_SRC_EXT_CRYS is not set
// CONFIG_RTC_CLK_SRC_EXT_OSC=y
// # CONFIG_RTC_CLK_SRC_INT_8MD256 is not set
// CONFIG_RTC_CLK_CAL_CYCLES=5000
// # end of RTC Clock Config