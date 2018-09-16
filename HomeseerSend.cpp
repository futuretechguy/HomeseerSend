#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include "HomeseerSend.h"


int Processed = 0;
char* HSHostIP;
int HSHostPort;

char* ST_SSID;
char* ST_PASSWORD;

IPAddress STATION_IP;
IPAddress STATION_GATEWAY;
IPAddress STATION_SUBNET;

HSSend::HSSend(bool IsDisplayStatus) {
	// Display Status
	_DisplayStatus = IsDisplayStatus;
}

// this is the serial begin' function
void HSSend::begin(int baudRate) {
	Serial.begin(baudRate);
	delay(500);
	if (_DisplayStatus) {
		Serial.print("Using: ");
		Serial.println(baudRate);
		Serial.println();
	}
}

// device controll json builder
String HSSend::GetJsonDevCtrlURL(String devID, String devValue) {

	String hurl = "/JSON?request=";
	hurl += "controldevicebyvalue";
	hurl += "&";
	hurl += "ref";
	hurl += "=";
	hurl += devID;
	hurl += "&";
	hurl += "value";
	hurl += "=";
	hurl += devValue;
	return hurl;

}
//Speach controll json builder
String HSSend::GetJsonSpeakURL(String sptext, String hostname) {

	String hurl = "/JSON?request=";
	hurl += "speak";
	hurl += "&";
	hurl += "phrase";
	hurl += "=";
	hurl += "\'" + sptext + "\'";
	hurl += "&";
	hurl += "host";
	hurl += "=";
	hurl += hostname;
	return hurl;

}

//set the host Homeseer server IP and Port
void HSSend::SetHost(char* HostIP, int HostPort = 80) {
	HSHostIP = HostIP;
	HSHostPort = HostPort;
}

//set static IP for WIFI connection
void HSSend::SetWiFiIP(IPAddress oStationIP, IPAddress oStationGateway, IPAddress oStationSubnet) {
	STATION_IP = oStationIP;
	STATION_GATEWAY = oStationGateway;
	STATION_SUBNET = oStationSubnet;
}

//Set WiFI SSID and Password
void HSSend::SetWiFiPswd(char* ossid, char* opassword) {
	ST_SSID = ossid;
	ST_PASSWORD = opassword;
}

//Connect to WiFI, this is only necessaye if not using own or anothe  WiFI cvonnection function
void HSSend::HSSWiFiConnect(bool IsStaticIP) {
	//Connect to a WiFi network
	_StaticIP = IsStaticIP;

	if (_DisplayStatus) {
		Serial.println();
		Serial.println();
		Serial.print("Connecting to ");
		Serial.println(ST_SSID);
	}


	if (_StaticIP) {
		WiFi.config(STATION_IP, STATION_GATEWAY, STATION_SUBNET);
	}

	WiFi.begin(ST_SSID, ST_PASSWORD);

	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}

	if (_DisplayStatus) {
		Serial.println("");
		Serial.println("WiFi connected");
		Serial.println("IP address: ");
		Serial.println(WiFi.localIP());
	}

}

//Data sending function
void HSSend::SendData(String urlrequest) {

	if (WiFi.status() != WL_CONNECTED) {
		if (_DisplayStatus) {
			Serial.println("WiFi connection detected");
			Serial.println(" ");
		}
	}

	String weburl = urlrequest;
	Serial.print("connecting to HS Server ");
	Serial.println(HSHostIP);

	// Use WiFiClient class to create TCP connections
	WiFiClient client;

	int httpPort = HSHostPort;

	if (!client.connect(HSHostIP, httpPort)) {
		Serial.println("connection failed");
		return;
	}

	if (_DisplayStatus) {
		Serial.print("Requesting URL: ");
		Serial.println(weburl);
	}


	// This will send the request to the server
	client.print(String("GET ") + weburl + " HTTP/1.1\r\n" + "Host: " + HSHostIP + "\r\n" + "Connection: close\r\n\r\n");
	delay(10);

	// Read all the lines of the reply from server and print them to Serial
	while (client.available()) {
		String line = client.readStringUntil('\r');
		Serial.print(line);
		int hsdev = line.indexOf('HomeSeer Devices');
		if (hsdev > -1) {
			Serial.println("");
			Serial.println("Correct response received");
			Serial.println("");
			Processed = 1;
		}
		else {
			Processed = 0;
		}
	}

	Serial.println();
	Serial.println("closing connection");
	Serial.println("");
	Serial.println("");
}


