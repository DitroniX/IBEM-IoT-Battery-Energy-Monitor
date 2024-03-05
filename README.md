## IBEM ESP32C3 IoT Battery Energy Monitor SDK  (NEW for March 2024)

Prototype 3D Render
![Display-Type-B](https://github.com/DitroniX/IBEM-IoT-Battery-Energy-Monitor/blob/main/IBEM-ESP32C3-SDK%20v1%202403-100%20PROTO%20PCA%20BETA%20Layout.jpg?raw=true)

![Display-Type-B](https://github.com/DitroniX/IBEM-IoT-Battery-Energy-Monitor/blob/main/Datasheets%20and%20Information/IBEM-ESP32C3-SDK%20v1%202403-100%20PROTO%203D.png?raw=true)

**IBEM Overview**

IBEM is a compact Solar System IoT Battery Energy Monitor, which can accurately measure up to 200A peak DC, nominally 100A continuous, complete with voltage and power monitoring.

Based on the new Espressif ESP32C3 Mini, which has both WiFi and BT, the SDK board is provided with a in-built bi-directional DC current sensor, via M8 bolt terminals, into a 16-bit ADS1115 four channel ADC. 

Using the on-board 4.5 to 80V DC-DC SMPS, the board can easily be safely powered direct from the battery source - an also continually monitor the battery voltage.

Interface for programming, and development, is thought the standard Type C USB, into the on-board ESP32C3 DevKit port.  So making development completely self-contained.

**Features**

 - **Espressif ESP32-C3 Mini** - 2.4GHz WiFi (802.11b/g/n) and Bluetooth 5
	 - ESP32-C3-MINI-1-N4
	 - ESP32-C3-MINI-1U-N4
 - **16Bit ADC I2C**
	 - ADS1115 (Analog To Digital Converter)
		 - DC Current 
		 - DC Voltage
		 - NTC PCB Ambient Temperature
 - **DC Bi-Directional Current Sensor**
	 - Cross chip CC6921BSO-100A (2 Paralleled providing continuous 100A or peak 200A)
	 - M8 Stud Terminations
 - **Type C Programming / Debug**
	 - CH340K USB UART 2Mbps Transceiver
 - **EEPROM I2C** 
	 - AT24C64 64Kbit 
 - **High Voltage SMPS DC-DC - 4.5V~80V**
	 - SSP9459	
 - **I2C OLED Connector**
	 - Configurable 3V3 polarity
 - **Compact and Easy  To Use**
	 - PCB 53mm x 70mm

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
