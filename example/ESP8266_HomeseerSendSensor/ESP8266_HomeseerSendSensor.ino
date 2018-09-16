#include <ESP8266WiFi.h>
#include <HomeseerSend.h>

int analogInPin = A0;       
int digitalOutPin = 13;     
int analogValue = 0;        
int Off_certainty = 0;
int ON_certainty = 0;

HSSend HSSend(false);

void setup() {
  pinMode(digitalOutPin, OUTPUT);
  digitalWrite(digitalOutPin, HIGH);
  Serial.begin(115200);
  delay(10);

  //HSSend.SetWiFiIP(IPAddress(192,168,100,69), IPAddress(192,168,100,1), IPAddress(255,255,255,0));  //for static IP
  HSSend.SetWiFiPswd("[SSID]", "]Password]");
  HSSend.SetHost("[HomeseerServerIP]", [Homeseer Port]);
  HSSend.HSSWiFiConnect(false); //set to true for static IP
}

void loop() {

  // read the analog in value:
  analogValue = analogRead(analogInPin);

  if (analogValue == 1024) {
    Off_certainty ++;
  } else {
    Off_certainty = 0;
  }

  if (Off_certainty >= 5) {
    int val = digitalRead(digitalOutPin);
    if (val==1) {
      Serial.println("Sending Off......");
      String url = HSSend.GetJsonURL("850", "0" );
      HSSend.SendData(url);
    }
    Serial.print("sensor = ");
    Serial.println("OFF");
    Serial.print(" ");
    digitalWrite(digitalOutPin, LOW);
    
  }

  if (analogValue < 700) {
    ON_certainty++;
  } else {
    ON_certainty = 0;
  }

  if (ON_certainty >= 5) {
    int val = digitalRead(digitalOutPin);
    if (val == 0) {
      Serial.println("Sending On.......");
      String url = HSSend.GetJsonURL("850", "1" );
      HSSend.SendData(url);
    }
    Serial.print("sensor = ");
    Serial.println("ON");
    Serial.print(" ");
    digitalWrite(digitalOutPin, HIGH);
  }
  delay(100);
}
