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

// ****** THIS PWM IS UNDER DEVELOPMENT - AWAITING PRODUCTION BOARD AND IS NOT CURRENTLY IN USE. ******

// Future Check https://github.com/Dlloydev/ESP32-ESP32S2-AnalogWrite

// **************** FUNCTIONS AND ROUTINES ****************

// Initialise IPEM/IPEC PWM
void Initialise_PWM(int PWM_GPIO)
{
    // PWM Integration Status
    if (EnablePWMTestOutput == false)
    {
        Serial.println("PWM DC Meter Output is Disabled\n");
    }
    else
    {
        ledcSetup(PWM_PCA_ChannelOut, PWM_FrequencyHz, PWM_ResolutionBit);
        ledcAttachPin(PWM_GPIO, PWM_ChannelOut);
        PWM_DutyCycleOutputUpperLimit = (int)(pow(2, PWM_ResolutionBit) - 1); // 8bit = 256-1 | 10bit = 1024-1 | 12bit = 4096-1 | 14bit = 16384-1 | 16bit = 65536-1.
        ledcWrite(PWM_PCA_ChannelOut, 0);

        Serial.println("PWM DC Meter Output Initialised\n");

        if (EnablePWMTestOutput == true)
            Serial.println("PWM DC Meter Output is Enabled from PWM Test Output");

        // Configuration Info
        Serial.printf("PWM Power Minimum Threshold W:\t\t%04.0f\n", PWM_PowerThreshold);
        Serial.printf("PWM Power Maximum Limit W:\t\t%04.0f\n", PWM_PowerUpperLimit);
        Serial.printf("PWM Duty Cycle Threshold:\t\t%04.3f\n", PWM_DutyCycleOutputThreshold);
        Serial.printf("PWM Duty Cycle Max Limit V:\t\t%04.3f\n", PWM_DutyCycleOutputUpperLimit);
        Serial.printf("PWM Percentage Display Lower:\t\t%d\n", PWM_PercentageMinimum);
        // Serial.printf("PWM Percentage Display Upper:\t\t%d\n", DAC_PercentageUpper);
        Serial.printf("PWM Resolution Bit:\t\t\t%d\n", PWM_ResolutionBit);
        Serial.printf("PWM Maximum Resolution:\t\t\t%d\n", PWM_MaximumResolution);
        Serial.printf("PWM Channel Output:\t\t\t%d\n", PWM_PCA_ChannelOut);
        Serial.printf("PWM Port Output GPIO :\t\t\t%d\n", PWM_GPIO);

        Serial.println("");
    }

} // Initialise_PWM

// PWM Duty Cycle Output Based on Input Base Power - But Only On Value Change or Zero
void PWM_WriteDutyCycle(float PWM_PowerPercentage = 0, float PWM_PowerMap = 0, int PWM_Channel = 0) // PWM_OutputDutyCycle 0 = OFF, PWM_PowerMap, PWM_Channel 0
{
    if (PWM_PowerPercentage != PWMPowerPercentage)
    {
        // Store Last Change
        PWMPowerPercentage = PWM_PowerPercentage;
        PWMPowerOutput = PWM_PowerMap;

        // Set Max Duty Cycle limit
        if (PWMDutyCycleOutput >= PWM_DutyCycleOutputUpperLimit)
            PWMDutyCycleOutput = PWM_DutyCycleOutputUpperLimit;

        // Set Min Duty Cycle limit.  If Zero then Ignore Lower Limit as this is PWM Off.
        if (PWMDutyCycleOutput <= PWM_DutyCycleOutputThreshold && PWMDutyCycleOutput > 0)
            PWMDutyCycleOutput = 0;

        // Calculate Values
        int WriteValue = map(PWM_PowerPercentage, PWM_PercentageMinimum, PWM_PercentageMaximum, PWM_DutyCycleOutputThreshold, PWM_DutyCycleOutputUpperLimit);

        // Update PWM Port
        ledcWrite(PWM_PCA_ChannelOut, WriteValue);

        // Display Value
        if (PWMPowerPercentage > 0)
        {
            Serial.print("Updated PWM Power: " + String(PWM_PowerMap) + "\t\t");
            Serial.print("Duty Cycle: " + String(WriteValue) + "\t\t");
            PWMPowerPercentage = map(PWMPowerPercentage, PWM_PercentageMinimum, PWM_PercentageMaximum, PWM_PowerThreshold, PWM_PowerUpperLimit); // Scaled example 0-4kW = 0-100% = 255
            Serial.println(String(PWMPowerPercentage) + " %");
        }
        else
        {
            Serial.println("Updated PWM Power Duty Cycle: \tPWM Off");
        }
    }

} // PWM_WriteDutyCycle

// Output PWM Value Based on Input Power. DutyCycle Approx Values: 256=1kW, 512=2kW, 769=3kW and 1023=4kW
void PWM_Power(float PWM_PowerOutput = 0, int PWM_Channel = 0)
{

    if (int(PWM_PowerOutput) != int(PWMPowerOutput) || PWM_PowerOutput == PWM_PowerThreshold)
    {
        // Store Last Change
        PWMPowerOutput = PWM_PowerOutput;

        // Set and Store Max and Min Power Window Limits (Top and Tail)
        if (PWMPowerOutput >= PWM_PowerUpperLimit)
            PWMPowerOutput = PWM_PowerUpperLimit;

        if (PWMPowerOutput <= PWM_PowerThreshold)
            PWMPowerOutput = PWM_PowerThreshold;

        // Map Power to PWM and Write
        float Local_Value = map(PWMPowerOutput, PWM_PowerThreshold, PWM_PowerUpperLimit, 0, 100);
        // float Local_Value = map(PWMPowerOutput, PWM_PowerThreshold, PWM_PowerUpperLimit, PWM_DutyCycleOutputThreshold, PWM_DutyCycleOutputUpperLimit);

        Serial.println(Local_Value);
        Serial.println(PWMPowerOutput);

        PWM_WriteDutyCycle(Local_Value, PWMPowerOutput);
    }

} // PWM_Power

// **************** Calibration Functions ****************

// 0 PWM Fixed Outputs for Calibration
void PWM_Calibrate()
{
    int Local_Value = 50; // % Duty Cycle

    Serial.println("PWM Fixed Outputs for Calibration");

    PWM_WriteDutyCycle(Local_Value);
    delay(3000);

    PWM_WriteDutyCycle(0);

} // PWM_Calibrate

// 1 PWM PWM Ramping Cycle
void PWM_RampingCycle()
{
    Serial.println("PWM PWM Ramping Cycle");

    /* Increasing the PWM Duty Cycle Output */
    // for (float Local_Value = 0; Local_Value <= 100; Local_Value++)
    for (float Local_Value = PWM_DutyCycleOutputThreshold; Local_Value <= PWM_DutyCycleOutputUpperLimit; Local_Value++)
    {
        PWM_WriteDutyCycle(Local_Value);

        Serial.println(Local_Value);

        delay(50);
    }
    /* Decreasing the PWM Duty Cycle Output */
    // for (float Local_Value = 100; Local_Value >= 0; Local_Value--)
    for (float Local_Value = PWM_DutyCycleOutputUpperLimit; Local_Value >= PWM_DutyCycleOutputThreshold; Local_Value--)
    {
        PWM_WriteDutyCycle(Local_Value);

        Serial.println(Local_Value);
        delay(50);
    }
} // PWM_RampingCycle

// 3 Fixed Ramped Power Outputs for Calibration
void PWM_CalibrateStepPowerRamp()
{
    int Delay_ms = 200;    // Inter Step Delay in ms (Default 200 = 0.2 Seconds)
    float StepPower = 100; // Inter Step Power Change in Watts (Default 100W)

    Serial.println("Fixed Ramped Power Outputs for Calibration");

    for (float Local_Value = 0; Local_Value <= (PWM_PowerUpperLimit + PWM_PowerThreshold); Local_Value += StepPower)
    {
        PWM_Power(Local_Value);
        delay(Delay_ms);
    }

    Serial.println("");
} // PWM_CalibrateStepPowerRamp
