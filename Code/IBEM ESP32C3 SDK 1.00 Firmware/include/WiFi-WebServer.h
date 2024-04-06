/*
  Dave Williams, DitroniX 2019-2024 (ditronix.net)
  Full header information in README

  This test code is OPEN SOURCE and formatted for easier viewing.  Although is is not intended for real world use, it may be freely used, or modified as needed.
  It is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.

  Note: Adding an SSID will Automatically ENABLE WiFi attempt to Log-In.

     Preamble.  This WiFi-WebServer will:
    * Display Web Information Page (Put IP in Browser)
    * Display Push OTA Page (Remote Uploading of BIN Files)

    WebServer URL Commands.  An example is http://ipaddress/pwm-on  e.g. http:192.168.0.5/pwm-on
   * /xx - Not currently enabled

   To setup the Push OTA and Reflash The Board:
    * Once flashed, Open Web Page using IP Address in Web Browser (PC, Tablet, Phone).
    * Upload a BIN file. Compiled from PlatformIO. In Folder  IBEM_ESP32C3_SDK_v1_Test_Code > pio > build > esp32-c3-devkitm-1 > firmware.bin

  Further information, details and examples can be found on our website and also GitHub wiki pages:
  * github.com/DitroniX
  * github.com/DitroniX/IBEM-IoT-Battery-Energy-Monitor
  * github.com/DitroniX/IBEM-IoT-Battery-Energy-Monitor/wiki
  * hackster.io/DitroniX/ibem-esp32c3-iot-battery-energy-monitor-solar-inverters-0342b1
*/

// Libraries
#include <Arduino.h>
#include <WebServer.h>
#include <ElegantOTA.h>

// ****************  VARIABLES / DEFINES / STATIC / STRUCTURES ****************

// Web Server.
WebServer server(80);
String page_header, page_css, page_title, page_banner, page_time, page_board, page_wifi, page_detail_header, page_detail, page_detail_footer, page_temperature, page_updater, page_footer;

// **************** FUNCTIONS AND ROUTINES ****************

// WebServer Page Content is based on basic HTML/CSS Code
void WebServerPageContent(void)
{
  // This section is made up of HTML/CSS Strings, which the ESP32 Web Server will hopefully deliver the page content.
  if (EnableAutoWebRefresh == true)
    page_header = "<!DOCTYPE HTML><html><head><meta http-equiv='refresh' content='5'>";
  else
    page_header = "<!DOCTYPE HTML><html><head>";

  page_title = "<title>" + AppAcronym + " (" + LocationName + ". " + HostName + ") " + "</title><meta name='viewport' content='width=device-width, initial-scale=1'></head>";
  page_css = "<style>h1 {text-align: center;}h2 {text-align: center;}p {text-align: center;}div {text-align: center;}body {background-color: MintCream; font-family: Arial, Helvetica, sans-serif;}</style>";
  page_banner = "<body><h1>Welcome to the " + AppAcronym + " - Local Web Portal</h1><p><strong>" + AppName + "</strong><br><small>(Firmware Version " + AppVersion + ")</small></p>";
  page_time = "<p>" + AppAcronym + " Time: <strong>" + NTP_RTC + " </strong><small>(Auto Refresh)</small></p>";
  page_board = "<p>Board Location:<strong> " + LocationName + "</strong>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Device:<strong>" + AppAcronym + "</strong>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;MAC Address: <strong>" + MACAddress + "</strong></p>";
  page_wifi = "<p>Board Hostname:<strong> " + HostName + "</strong>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Wi-Fi SSID:<strong> " + WiFi_SSID + "</strong>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Wi-Fi RSSI:<strong> " + String(RSSI_Level) + " dBm (" + RSSI_Detail + ")" + "</strong></p><BR>";

  page_detail_header = "<table style='border:0px solid black;margin-left:auto;margin-right:auto;font-size:30px;'>";
  page_detail = "";
  page_detail = page_detail + "<tr><td align='left'>Power </td><td align='right'><strong>" + String(DCPower) + "</td><td align='center'> W</strong></td></tr>";
  page_detail = page_detail + "<tr><td align='left'>Voltage </td><td align='right'><strong>" + String(DCVoltage) + "</td><td align='center'> V</strong></td></tr>";
  page_detail = page_detail + "<tr><td align='left' width='40%'>Current </td><td align='right'width='35%'><strong>" + String(DCCurrentAccumulative) + "</td><td align='center' width='25%'> A</strong></td></tr>";

  page_detail_footer = "</table>";

  page_temperature = "<p><h2>Board Temperature:  <strong>" + String(BoardTemperatureC) + " &#xb0;C<BR>Probe Temperature:  <strong>" + String(ProbeTemperatureC) + " &#xb0;C</strong></h2></p>";

  page_updater = "<br><p><strong>To Update the " + AppAcronym + " Firmware via Push OTA - <a href='http://localhost/update' target='_blank'>Click Here</a></strong><br></p><p><small>(Load compiled <i>firmware.bin</i> file - located: <strong> " + FirmwarePath + "</small></strong>)</p>";
  page_footer = "<br><hr><p><small>Dave Williams&nbsp;&nbsp; | &nbsp;&nbsp;G8PUO&nbsp;&nbsp; | &nbsp;&nbsp;DitroniX&nbsp; | &nbsp;<a href='http://ditronix.net' target='_blank'>DitroniX.net</a>&nbsp; | &nbsp;<a href='https://github.com/DitroniX' target='_blank'>github.com/DitroniX</a>&nbsp; | &nbsp;<a href='https://www.hackster.io/ditronix' target='_blank'>hackster.io/DitroniX</a>&nbsp;&nbsp; | &nbsp;&nbsp;<a href='https://www.buymeacoffee.com/DitroniX' target='_blank'>Buy Me A Coffee</a></P></small></body></html>";
} // WebServerPageContent

// WebServer Pages 200 (and 400).
void WebServerRoot()
{
  page_updater.replace("localhost", WiFi.localIP().toString().c_str());
  server.send(200, "text/html", page_header + page_title + page_css + page_banner + page_time + page_board + page_wifi + page_detail_header + page_detail + page_detail_footer + page_temperature + page_updater + page_footer);
} // WebServerRoot

// Initialise WebServer
void InitialiseWebServer()
{
  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.println("Initialise WebServer");

    WebServerPageContent();

    // // Web Server Listener
    // server.on("/pwm-local", []() // Local Test

    //     // Serial.println("Receiving Message: PWM Turn On\n");
    //     // DAC_WriteVoltage(0); // Turn Output Off
    //     //     EnablePWMTestOutput = false;
    //     // delay(100); WebServerRoot(); });

    //     // Display Web Page Upon Client Request
    //     server.on("", WebServerRoot);

    // Display Web Page Upon Invalid URL Client Request.  Faster than redirecting a 404 error.
    server.onNotFound(WebServerRoot);

    // Ignore SerialMonitor _handleRequest().  Future Options.

    ElegantOTA.begin(&server); // Start ElegantOTA
    server.begin();
    Serial.println("HTTP Web Server Started");
    Serial.println("Webpage can be found http://" + WiFi.localIP().toString() + "\n");
  }
} // InitialiseWebServer

// Display Web Server Page for Information and OTA
void CheckOTA()
{
  if (WiFi.status() == WL_CONNECTED)
  {
    WebServerPageContent(); // Refresh Content.  NB.Temporary Solution.
    server.handleClient();
  }

} // CheckOTA