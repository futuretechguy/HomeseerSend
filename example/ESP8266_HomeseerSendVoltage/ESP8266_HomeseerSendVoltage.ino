//***********************************************************************************
//* This example shows how to talk to talk to Homeseer ftom ESP using this library  *
//*                                                                                 *
//*********************************************************************************** 


#include <HomeseerSend.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <WiFiClient.h>

****************************************************************
unsigned long StartTime = 0;  //millis();
unsigned long Interval = 900000; //15 Minutes
String strVoltage = "0";
float lastVoltage = 0;        //previous voltage

ADC_MODE(ADC_VCC);

HSSend HSSend(true); //initilize HomeseerSend

void setup() {
  Serial.begin(115200);
  Serial.println("");
  delay(500);

  WiFiManager wifiManager; //This library alows you to avoid using SSID and passeord in sketch see instructions at https://github.com/tzapu/WiFiManager
  wifiManager.setSTAStaticIPConfig(IPAddress(192, 168, 10, 98), IPAddress(192, 168, 10, 1), IPAddress(255, 255, 255, 0)); //use local environment IP address
  wifiManager.autoConnect("AutoConnectAP");

  //Set the homeseer server port and IP
  HSSend.SetHost("192.168.10.46", 80);

}


void loop() {

  float voltageRef = 0.00f;
  voltageRef = ESP.getVcc();
  float voltage = voltageRef / 1023.00f;

  unsigned long CurrentTime = millis();

  if (voltage != lastVoltage) {
    Serial.print("Bat: ");
    Serial.print(voltage);
    Serial.println(" V");

    if (CurrentTime - StartTime > Interval) {
      String strVoltage;
      strVoltage += voltage;
      Serial.println("Sending current voltage......");
      Serial.println(" ");
      String url = HSSend.GetJsonDevCtrlURL("865", strVoltage);
      
      //String url = HSSend.GetJsonSpeakURL("talking to homeseer from ESP", "MediaCenterExpress:*");
      HSSend.SendData(url);
      strVoltage = String();
      StartTime = CurrentTime;
    }
    lastVoltage = voltage;
    delay(1000);
  }


}
