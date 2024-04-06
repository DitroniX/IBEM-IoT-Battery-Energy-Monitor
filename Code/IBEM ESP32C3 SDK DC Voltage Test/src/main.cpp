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
#include <RGBLed.h>     //https://github.com/wilmouths/RGBLed
#include <ADS1115_WE.h> // https://github.com/wollewald/ADS1115_WE?utm_source=platformio&utm_medium=piohome

// ****************  VARIABLES / DEFINES / STATIC / STRUCTURES / CONSTANTS ****************

// ******************************** DATA ********************************

// Define I2C (Expansion Port)
#define I2C_SDA 4
#define I2C_SCL 5
#define I2C_CLOCK 4000000UL

// ******************************** INPUTS ********************************
#define User_Button 9 // User Button also Manual PGM

// ******************************** OUTPUTS ********************************
#define LED_Red 10   // User RGB
#define LED_Green 18 // User RGB
#define LED_Blue 19  // User RGB

// ******************************** ADC ********************************
// Default IBEM ADS1115 Address
#define IBEM_ADS1115 0x48

// Variables User
const int AverageSamples = 20; // Average Multi-Samples on each Channel Read.  (Default 20)
const int AverageDelay = 10;   // Average Inter Multi-Sample ms Delay.  (Default 10)

// Voltage User
const float DCVoltageThreshold = 6.00; // Minimum Voltage > USB - removing noise. (Default 6.00)
const float DCVoltageOffset = 0.00;    // Battery Voltage Zero Calibration Offset.  (Default 0.00)

// Voltage Other
float VoltageFactor = 30.9396; // DC Input Multiplier (Default 30.9396)
float VoltageFudge = 0.53;     // DC Input Fudge Factor for Diode (Default 0.53)

// **************** INSTANCES ****************

RGBLed led(LED_Red, LED_Green, LED_Blue, RGBLed::COMMON_CATHODE);

ADS1115_WE adc = ADS1115_WE(IBEM_ADS1115); // 16bit ADC

// ADC Channels
// * Channel 0 - DC Voltage Input
// * Channel 1 - Current Sensor 1
// * Channel 2 - Current Sensor 1
// * Channel 3 - NTC Temperature

// **************** FUNCTIONS AND ROUTINES ****************

// Initialise IBEM ADS1115 ADC (Default Address 0x48)
void Initialise_ADS1115_ADC()
{

  Serial.println("Initialising ADC\n");

  // Functions https://github.com/wollewald/ADS1115_WE/blob/master/List%20of%20public%20functions.pdf

  /* Set the voltage range of the ADC to adjust the gain
   * ADS1115_RANGE_6144  ->  +/- 6144 mV 2/3x gain +/- 6.144V  1 bit = 0.1875mV
   * ADS1115_RANGE_4096  ->  +/- 4096 mV (default) 1x gain   +/- 4.096V  1 bit = 0.125mV
   * ADS1115_RANGE_2048  ->  +/- 2048 mV +/- 2.048V  1 bit = 0.0625mV
   * ADS1115_RANGE_1024  ->  +/- 1024 mV +/- 1.024V  1 bit = 0.03125mV
   * ADS1115_RANGE_0512  ->  +/- 512 mV +/- 0.512V  1 bit = 0.015625mV
   * ADS1115_RANGE_0256  ->  +/- 256 mV +/- 0.256V  1 bit = 0.0078125mV
   */

  adc.setVoltageRange_mV(ADS1115_RANGE_4096); // (Default ADS1115_RANGE_4096)
  adc.setCompareChannels(ADS1115_COMP_0_GND);
  adc.setMeasureMode(ADS1115_CONTINUOUS);
  adc.setConvRate(ADS1115_128_SPS);

} // Initialise_ADS1115_ADC

// Read and Calculate Average Value with Reduce Jitter
float readChannel(ADS1115_MUX channel)
{
  float VoltageRaw = 0.0;

  adc.setCompareChannels(channel);

  for (int i = 0; i < AverageSamples; i++)
  {
    VoltageRaw = VoltageRaw + adc.getResult_V();
    delay(AverageDelay);
  }
  VoltageRaw = VoltageRaw / AverageSamples;
  return VoltageRaw;
} // readChannel

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

  Initialise_ADS1115_ADC();
}

// **************** LOOP ****************
void loop(void)
{
  float ADCV0, DCVoltage;

  // DC Input Voltage
  ADCV0 = readChannel(ADS1115_COMP_0_GND);
  DCVoltage = ((ADCV0 * VoltageFactor) + VoltageFudge) + DCVoltageOffset;
  Serial.print("DC Voltage: \t");
  Serial.println(DCVoltage);

  // Heatbeat
  led.flash(RGBLed::BLUE, 500);
  delay(1000);
}
