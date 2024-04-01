/*
  Dave Williams, DitroniX 2019-2024 (ditronix.net)
  Full header information in README

  This test code is OPEN SOURCE and formatted for easier viewing.  Although is is not intended for real world use, it may be freely used, or modified as needed.
  It is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.

  Further information, details and examples can be found on our website and also GitHub wiki pages:
  * github.com/DitroniX
  * github.com/DitroniX/IBEM-IoT-Battery-Energy-Monitor
  * hackster.io/DitroniX/ibem-esp32c3-iot-battery-energy-monitor-solar-inverters-0342b1
*/

// Libraries
#include <Arduino.h>
#include <ADS1115_WE.h> // https://github.com/wollewald/ADS1115_WE?utm_source=platformio&utm_medium=piohome

// ****************  VARIABLES / DEFINES / STATIC / STRUCTURES / CONSTANTS ****************

// ADC Instance
ADS1115_WE adc = ADS1115_WE(IBEM_ADS1115); // 16bit ADC

// ADC Channels
// * Channel 0 - DC Voltage Input
// * Channel 1 - Current Sensor 1
// * Channel 2 - Current Sensor 1
// * Channel 3 - NTC Temperature

// **************** FUNCTIONS AND ROUTINES ****************

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

// Read Single Value
float readSingleChannel(ADS1115_MUX channel)
{
  float voltage = 0.0;
  adc.setCompareChannels(channel);
  voltage = adc.getResult_V();
  return voltage;
} // readSingleChannel

// Initialise IBEM ADS1115 ADC (Default Address 0x49)
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

  if (InvertReading == false)
    Serial.println("ADC Current/Power Inverted");
}

void ReadADC()
{ // Read ADC

  float ADCV0, ADCV1, ADCV2, ADCV3, VoltageRaw;

  ADCV0 = readChannel(ADS1115_COMP_0_GND);
  ADCV1 = readChannel(ADS1115_COMP_1_GND);
  ADCV2 = readChannel(ADS1115_COMP_2_GND);
  ADCV3 = readChannel(ADS1115_COMP_3_GND);

  Serial.print("ADC Raw Read \t0: ");
  Serial.print(ADCV0);

  Serial.print(",   1: ");
  Serial.print(ADCV1);

  Serial.print(",   2: ");
  Serial.print(ADCV2);

  Serial.print(",   T: ");
  Serial.print(ADCV1 + ADCV2);

  Serial.print(",   3: ");
  Serial.println(ADCV3);

  // DC Input Voltage
  float VoltageFactor = 30.9396; // DC Input Multiplier (Default 30.9396)
  float VoltageFudge = 0.53;     // DC Input Fudge Factor for Diode (Default 0.53)
  DCVoltage = ((ADCV0 * VoltageFactor) + VoltageFudge) + DCVoltageOffset;

  // DC Current
  VoltageRaw = ADCV1 + ADCV2;
  VoltageRaw = VoltageRaw - 3.3;

  DCCurrentAccumulative = VoltageRaw / 0.0132;
  DCCurrentAccumulative = DCCurrentAccumulative + DCCurrentOffset;

  // Remove Noise
  if (DCCurrentAccumulative > 0 && DCCurrentAccumulative < DCCurrentThreshold)
    DCCurrentAccumulative = 0;

  if (DCCurrentAccumulative < 0 && DCCurrentAccumulative > DCCurrentThreshold)
    DCCurrentAccumulative = 0;

  DCPower = DCVoltage * DCCurrentAccumulative;

  // Invert reading values ending direction - default FALSE (i.e. Battery Negative to Battery Negative)
  if (InvertReading == false)
  {
    DCCurrentAccumulative = -DCCurrentAccumulative;
    DCPower = -DCPower;
  }

  // Output Values
  Serial.print("ADC Calculated: ");
  Serial.print(DCCurrentAccumulative);
  Serial.print(" A\t");
  Serial.print(DCVoltage);
  Serial.print(" V\t");
  Serial.print(DCPower);
  Serial.println(" W\n");

  // Calculate NTC Resistance
  float Vo = readChannel(ADS1115_COMP_3_GND);
  // Serial.print("NTC Voltage:\t");
  // Serial.print(Vo);

  float FIX_R = 8000; // Nominal 10K
  float NTC_B = 0;    // β K (Default 3450)
  int NTC_R = (FIX_R) / (Vo - 1);
  // Serial.print("\tNTC Resistance:\t");
  // Serial.println(NTC_R);

  // Calculate Temperature from NTC
  float logNTC;
  float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;

  logNTC = log(NTC_R + NTC_B);
  BoardTemperatureK = (1.0 / (c1 + c2 * logNTC + c3 * logNTC * logNTC * logNTC));
  BoardTemperatureC = BoardTemperatureK - 273.15;       // Celsius
  BoardTemperatureF = (BoardTemperatureC * 1.8) + 32.0; // Fahrenheit

  // Output Values
  Serial.print("PCB NTC Temperature:\t");
  Serial.print(BoardTemperatureC);
  Serial.print(" ºC\t");
  Serial.print(BoardTemperatureF);
  Serial.println(" ºF");
}

// // Check DCV_IN DC Voltage
void CheckDCVINVoltage()
{
  Serial.println();
  if (DCVoltage < 5.2)
    Serial.println("* " + AppAcronym + " Board appears to be only USB Powered " + String(DCVoltage) + "V\n* Power Calculations will not work in this mode");

  if (DCVoltage >= 5.2)
    Serial.println("DC Voltage Input Detected " + String(DCVoltage) + "V");

  if (DCVoltage > 80)
    Serial.print("\n* WARNING: Please Check Input Voltage 80V MAX.  Too High! Value ");

  Serial.println();
} // CheckDCVINVoltage

// Test Read ADC and Display Raw Values
void TestADS1115()
{
  float voltage = 0.0;
  float voltage2 = 0.0; // test

  Serial.print("ADC Raw Read \t0: ");
  voltage = readChannel(ADS1115_COMP_0_GND);
  Serial.print(voltage);

  Serial.print(",   1: ");
  voltage = readChannel(ADS1115_COMP_1_GND);
  Serial.print(voltage);

  Serial.print(",   2: ");
  voltage2 = readChannel(ADS1115_COMP_2_GND);
  Serial.print(voltage2);

  Serial.print(",   T: ");
  Serial.print(voltage + voltage2);

  Serial.print(",   3: ");
  voltage = readChannel(ADS1115_COMP_3_GND);
  Serial.println(voltage);

} // TestADS1115