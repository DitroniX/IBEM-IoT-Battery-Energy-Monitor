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

// The LED PWM controller can generate independent digital waveform on six channels on Any GPIO pins.  The accuracy of duty cycle can be up to 18 bits.

// ****************  VARIABLES / DEFINES / STATIC / STRUCTURES / CONSTANTS ****************

// ******************************** OUTPUTS ********************************
#define LED_Red 10        // User RGB
#define LED_Green 18      // User RGB
#define LED_Blue 19       // User RGB
#define PWM_Meter_GPIO 3  // DCMeter (PWM)

// **************** INSTANCES ****************

// **************** FUNCTIONS AND ROUTINES ****************

// **************** SETUP ****************
void setup() {
  // Stabalise
  delay(250);

  // Initialise UART
  Serial.begin(115200, SERIAL_8N1);  // 115200
  while (!Serial)
    ;
  Serial.println("");

  //  Configures the specified LED GPIO as outputs
  pinMode(LED_Red, OUTPUT);
  pinMode(LED_Green, OUTPUT);
  pinMode(LED_Blue, OUTPUT);

  // LEDs Default Off State
  digitalWrite(LED_Red, LOW);
  digitalWrite(LED_Green, LOW);
  digitalWrite(LED_Blue, LOW);

  // PWM
  pinMode(PWM_Meter_GPIO, OUTPUT);
}

// **************** LOOP ****************
void loop(void) {

  // Updates both Green LED and the DC Meter Output (100uA)

  // PWM Test
  analogWrite(PWM_Meter_GPIO, 0);  //generates PWM of 0% duty cycle
  analogWrite(LED_Green, 0);
  delay(2000);

  analogWrite(PWM_Meter_GPIO, 64);  //generates PWM of 25% duty cycle
  analogWrite(LED_Green, 64);
  delay(2000);

  analogWrite(PWM_Meter_GPIO, 127);  //generates PWM of 50% duty cycle
  analogWrite(LED_Green, 127);
  delay(2000);

  analogWrite(PWM_Meter_GPIO, 191);  //generates PWM of 75% duty cycle
  analogWrite(LED_Green, 191);
  delay(2000);

  analogWrite(PWM_Meter_GPIO, 255);  //generates PWM of 100% duty cycle
  analogWrite(LED_Green, 255);
  delay(2000);
}
