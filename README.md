## IBEM ESP32C3 IoT Battery Energy Monitor SDK  (NEW for March 2024)

Prototype IBEM-ESP32C3-SDK v1 2403-100 PROTO PCA
![Display-Type-B](https://github.com/DitroniX/IBEM-IoT-Battery-Energy-Monitor/blob/main/Datasheets%20and%20Information/IBEM-ESP32C3-SDK%20v1%202403-100%20PROTO%20PCA.jpg?raw=true)

![Display-Type-B](https://github.com/DitroniX/IBEM-IoT-Battery-Energy-Monitor/blob/main/Datasheets%20and%20Information/IBEM-ESP32C3-SDK%20v1%202403-100%20PROTO%20PCA%20Rear.jpg?raw=true)

Prototype IBEM Proto Initial Power up Initial Commissioning and Testing.
![Display-Type-B](https://github.com/DitroniX/IBEM-IoT-Battery-Energy-Monitor/blob/main/Datasheets%20and%20Information/IBEM-ESP32C3-SDK%20v1%202403-100%20PROTO%20Studs%20Test.jpg?raw=true)

IBEM-ESP32C3-SDK v1 2403-100 PROTO Bring-Up Test
![Display-Type-B](https://github.com/DitroniX/IBEM-IoT-Battery-Energy-Monitor/blob/main/Datasheets%20and%20Information/IBEM-ESP32C3-SDK%20v1%202403-100%20PROTO%20Bring-Up%20Test.jpg?raw=true)

**IBEM Overview**

IBEM is a compact Solar System IoT Battery Energy Monitor, which can accurately measure up to 200A peak DC, nominally 100A continuous, complete with voltage and power monitoring.

Based on the new Espressif ESP32C3 Mini, which has both WiFi and BT, the SDK board is provided with a in-built bi-directional DC current sensor, via M8 bolt terminals, into a 16-bit ADS1115 four channel ADC. 

Using the on-board 4.5 to 80V DC-DC SMPS, the board can easily be safely powered direct from the battery source - an also continually monitor the battery voltage.

Interface for programming, and development, is thought the standard Type C USB, into the on-board ESP32C3 DevKit port.  So making development completely self-contained.

**IBEM Specification and Features**

- **Compatible Example of Battery Types**
  - LI, SLD, FLD, GEL, AGM, LFP, NiCd, NiMH, LiPO, VRLA.
- **MCU**
  - **Espressif ESP32-C3 Mini** - 2.4GHz Wi-Fi (802.11b/g/n) and Bluetooth 5
    - ESP32-C3-MINI-1-N4 *(PCB Antenna)*
    - ESP32-C3-MINI-1U-N4 *(External Antenna via U. FL cable)*
- **16 Bit ADC (I2C)**
  - ADS1115 (Analog to Digital Converter)
    - DC Current *(Two paralleled channels)*
    - DC Input Voltage Monitoring
    - NTC PCB Ambient Temperature
- **DC Bi-Directional Current Sensor**
  - Cross chip CC6921BSO-100A 
  - 2 Paralleled sensors providing continuous +- 100A or peak +- 200A
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
