#ifndef hs
#define hs
#include <ESP8266WiFi.h>

#if (ARDUINO >=100)
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

class HSSend {

    public:
    // Constructor
    HSSend(bool isDisplayStatus=false);

    // Methods
    void begin(int baudRate = 115200);
    void SendData (String urlrequest);
    String GetJsonDevCtrlURL (String devID, String devValue);
	String GetJsonSpeakURL(String sptext, String hostname);
    void SetHost(char* hostIP, int hostPort);
    void SetWiFiIP(IPAddress StationIP, IPAddress StationGateway, IPAddress StationSubnet);
    void SetWiFiPswd(char* ssid, char* password);
    void HSSWiFiConnect(bool isStaticIP=false);
    String GetJsonURL(String devID, float devValue);
    
    private:
    bool _StaticIP;
    bool _DisplayStatus;
};
#endif
