## IBEM ESP32C3 IoT Battery Energy Monitor SDK  (NEW for March 2024)

IBEM Connections and Overview
![Display-Type-B](https://github.com/DitroniX/IBEM-IoT-Battery-Energy-Monitor/blob/main/Datasheets%20and%20Information/IBEM%20-%20IoT%20Battery%20Energy%20Monitor%20Board%20SDK%20Connections.jpg?raw=true)

IBEM-ESP32C3-SDK v1 2403-101P
![Display-Type-B](https://github.com/DitroniX/IBEM-IoT-Battery-Energy-Monitor/blob/main/Datasheets%20and%20Information/IBEM-ESP32C3-SDK%20v1%202403-101P%20TopBottom.jpg?raw=true)

_IBEM Connection Examples_
![IBEM Expanded Example Connections](https://github.com/DitroniX/IBEM-IoT-Battery-Energy-Monitor/blob/main/Datasheets%20and%20Information/IBEM%20Expanded%20Example%20Connections.png)


Prototype IBEM Proto Initial Power up Initial Commissioning and Testing.
![Display-Type-B](https://github.com/DitroniX/IBEM-IoT-Battery-Energy-Monitor/blob/main/Datasheets%20and%20Information/IBEM-ESP32C3-SDK%20v1%202403-100%20PROTO%20Studs%20Test.jpg?raw=true)

IBEM-ESP32C3-SDK v1 2403-100 PROTO Bring-Up Test 
![Display-Type-B](https://github.com/DitroniX/IBEM-IoT-Battery-Energy-Monitor/blob/main/Datasheets%20and%20Information/IBEM-ESP32C3-SDK%20v1%202403-100%20PROTO%20Bring-Up%20Test.jpg?raw=true)


IBEM-ESP32C3-SDK U.FL Version 3D (New April 2024)
![Display-Type-B](https://github.com/DitroniX/IBEM-IoT-Battery-Energy-Monitor/blob/main/Datasheets%20and%20Information/IBEM-1%20ESP32-C3-1U%20IoT%20Battery%20Energy%20Monitor%203D.jpg?raw=true)

**IBEM Overview**

IBEM is a compact, self-contained, Solar System IoT Battery Energy Monitor, which can accurately measure current flow nominally up to +-100A (+-200A Peak), and voltage up to 80V DC.  This board essentially providing a complete voltage and power monitoring solution for your 48V, 36V, 24V or 12V batteries.

Based on the new Espressif ESP32C3 Mini, which has both Wi-Fi and BT, the IBEM SDK board is provided with onboard bi-directional DC current sensor, via M8 bolt terminals, into a 16-bit ADS1115 four channel ADC.  No external shunts required.

Using the onboard 4.5 to 80V DC-DC SMPS, the board can easily be safely powered directly from the battery source – This also providing continuous monitoring of the battery voltage.

Interface for programming, and development, is thought a standard Type C USB, into the onboard UART and ESP32C3, so making development and use, completely self-contained.

Using the provided I2C interface, an OLED display can be plugged directly into the board, if required, for local monitoring and status information.

Opensource IBEM Firmware is provided on our GitHub, using the standard PlatformIO platform. This monitors Current, Voltage (and Temperature), to derive Power Energy information, which can then be sent to your Home Automation, or cloud service, via provided MQTT or Domoticz, as standard.


**IBEM Specification and Features**

- **Compatible Examples of Battery Types**
  - LI, SLD, FLD, GEL, AGM, LFP, NiCd, NiMH, LiPO, LIFePO4, VRLA.
- **MCU**
  - **Espressif ESP32-C3 Mini** - 2.4GHz Wi-Fi (802.11b/g/n) and Bluetooth 5
    - ESP32-C3-MINI-1-N4 *(PCB Antenna)*
    - ESP32-C3-MINI-1U-N4 *(External Antenna via U. FL cable)*
- **16 Bit ADC (I2C)**
  - ADS1115 (Analog to Digital Converter)
    - DC Current *(Two paralleled channels)*
    - DC Input Voltage Monitoring
    - NTC PCB Ambient Temperature
- **DC Bi-Directional Current Sensor**
  - Cross chip CC6921BSO-100A 
  - 2 Paralleled sensors providing up to +- 100A (peak +- 150A).
  - 2 x M8 Stud Terminations
  - Low Side DC Monitoring for Safety
  - Current Sensor can be configured to be FULLY electrically isolated *(PCB link)*
- **Type C Programming / Debug**
  - Type C USB Connector
  - CH340K USB UART 2Mbps Transceiver
  - Easy programming.  Select ‘ESP32C3 Dev Module’
  - BAUD rate up to 921600 to speed up Flashing
  - Serial Monitoring Debug and Remote Logging
  - Power Can Be Taken from USB 5V (when Flashing).
- **EEPROM (I2C)**
  - AT24C64 64Kbit
  - On-board logging
  - Firmware setup user parameters and configuration
- **High Voltage SMPS DC-DC**
  - Large Input Voltage Range 4.5V to 80V DC
  - SSP9481 step-down switch-mode with a built-in power MOSFET
  - Can be used on 12V, 24V, 36V and 48V Systems.
  - Resettable PCB DC Input Fuse *(100mA continuous, 250mA Trip)*
  - Board can be also powered from USB when programming
  - DC input taken from battery under test (separate positive terminal)
- **On-Board Temperature**
  - NTC Ambient PCB Temperature Sensor
- **External Temperature**
  - Dallas OneWire Interface for External Temperatures
    - *Example Probe Sensor to Battery Cells*
- **DC Moving Coil Meter**
  - PWM Output Interface to External Moving Coil Meter
  - 100uA single full scale, or centre zero
- **RGB LED - Status**
  - Configured to three GPIO’s for easy Firmware control
- **RGB LED – Power**
  - Power, UART TXD and USB Power Indicator
- **I2C OLED Connector**
  - Configurable 3V3 Pin 1 and 2 polarities
  - PCB 4 Pin Header Connector
- **Buttons**
  - Reset
  - User (Program or GP9 Detect)
- **Compact Design and Easy to Use**
  - PCB 53mm x 70mm
  - 4 x M3 mounting holes (one grounded to DC-DC ground)
  - Operating Ambient Temperature -40 to +85 °C
  - Optional Conformal Coating – on request.

## **Ordering**

- **ESP32-C3-MINI-1-N4** *(PCB Antenna)*
  - eBay: https://www.ebay.co.uk/itm/326080939227
  - Amazon: *URL To follow*
- **ESP32-C3-MINI-1U-N4** *(External Antenna via U. FL)
  - eBay: *URL To follow*
  - Amazon: *URL To follow*

## **Further Information**

Additional information, and other technical details on this project, maybe found in the related repository pages.

**Repository Folders**

 - **Code** *(Code examples for Arduino  IDE and PlatformIO)*
 -  **Datasheets and Information** *(Component Datasheets, Schematics, Board Layouts, Photos, Technical Documentation)*
 - **Certification** *(Related Repository Project or Part, Certification Information)*

**Repository Tabs**

 - **Wiki** *(Related Repository Wiki pages and Technical User Information)*
 - **Discussions** *(Related Repository User Discussion Forum)*
 - **Issues** *(Related Repository Technical Issues and Fixes)*

***

We value our Customers, Users of our designs and STEM Communities, all over the World . Should you have any other questions, or feedback to share to others, please feel free to:

* Visit the related [Project](https://github.com/DitroniX?tab=repositories) *plus the related* **Discussions** and **Wiki** Pages.  See tab in each separate repository.
* **Project Community Information** can be found at https://www.hackster.io/DitroniX
* [DitroniX.net Website - Contact Us](https://ditronix.net/contact/)
* **Twitter**: [https://twitter.com/DitroniX](https://twitter.com/DitroniX)
* [Supporting the STEM Projects - BuyMeACoffee](https://www.buymeacoffee.com/DitroniX)
*  **LinkedIN**: [https://www.linkedin.com/in/g8puo/](https://www.linkedin.com/in/g8puo/)

***Dave Williams, Eastbourne, UK.***

Electronics Engineer | Software Developer | R&D Support | RF Engineering | Product Certification and Testing | STEM Ambassador

## STEM

**Supporting [STEM Learning](https://www.stem.org.uk/)**

Life is one long exciting learning curve, help others by setting the seed to knowledge.

![DitroniX Supporting STEM](https://hackster.imgix.net/uploads/attachments/1606838/stem_ambassador_-_100_volunteer_badge_edxfxlrfbc1_bjdqharfoe1_xbqi2KUcri.png?auto=compress%2Cformat&w=540&fit=max)
