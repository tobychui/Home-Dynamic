#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <string>
#include <DHT.h>
#define DHTPIN 2
#define DHTTYPE DHT11

ESP8266WebServer server(80);

//Global variables for module
String CurrentStatus = "ON";
String deviceUuid = "1e2aea28-493d-474b-b2f3-d5d8e0ee8fc8";
String deviceName = "Standard DHT11 Module";
String identifier = "dht11.std.hd.com.imuslab";
const char* ssid = "";
const char* password = "";

DHT dht(DHTPIN, DHTTYPE);

void setup() {
Serial.begin(9600);
Serial.println("");
WiFi.mode(WIFI_STA);
WiFi.begin(ssid, password);
Serial.println(deviceName);
Serial.println(identifier);
Serial.println(deviceUuid);
Serial.print("Trying to connect");
while (WiFi.status() != WL_CONNECTED) {  //Wait for the WiFI connection completion
  delay(500); 
  Serial.print(".");
}
//Start DHT Sensor
dht.begin();
//Display Wifi Connection Information
Serial.println("WiFi connected");  
Serial.println("IP address: ");
Serial.println(WiFi.localIP());
  
//Declare System Function for NodeMCU
server.on("/", showLicense);
server.on("/uuid", uuid);
server.on("/info", info);
server.on("/on", ON);
server.on("/off", OFF);
server.on("/toggle", toggle);
server.on("/status", Status);

//Start Server
server.begin();
}

void showLicense(){
  int size=1000;
  char temp[size];
  snprintf( temp, size, "<table> <tbody> <tr> <td>Device Type: </td> <td>IMUS Standard DHT11 Sensor Module</td> </tr> <tr> <td>Init Type:</td> <td>Static LAN</td> </tr> <tr> <td>Manufacture</td> <td>IMUS Laboratory, Prototype</td> </tr> <tr> <td>License </td> <td>CopyRight IMUS Laboratory, 2019</td> </tr> </tbody> </table> <p>&nbsp;</p>");
  //server.sendHeader("Access-Control-Allow-Origin","*");
  server.send(200, "text/html", temp);
}


void info(){
  int size=1000;
  char temp[size];
  snprintf( temp, size, (deviceName + "_" + identifier).c_str ());
  server.sendHeader("Access-Control-Allow-Origin","*");
  server.send(200, "text/html", temp);
}

void uuid(){
  int size=100;
  char temp[size];
  snprintf( temp, size, deviceUuid.c_str ());
  server.sendHeader("Access-Control-Allow-Origin","*");
  server.send(200, "text/html", temp);
}

void ON(){
  Serial.println("WARNING! Sensor cannot be toggled. Ignore ON Request.");
  sendDone();
}

void OFF(){
  Serial.println("WARNING! Sensor cannot be toggled. Ignore OFF Request.");
  sendDone();
}

void toggle(){
  Serial.println("Ignore Toggle Request.");
  sendDone();
}


void Status(){
  int size=100;
  char temp[size];
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  char charBuf[50];
  ("{\"temp\":" + String(t) + ",\"humi\":" + String(h) +"}").toCharArray(charBuf, 50);
  server.sendHeader("Access-Control-Allow-Origin","*");
  server.send(200, "application/json", charBuf);
}

void sendDone(){
  int size=100;
  char temp[size];
  snprintf( temp, size, "DONE");
  server.sendHeader("Access-Control-Allow-Origin","*");
  server.send(200, "text/html", temp);
}

void loop() {
  server.handleClient();
}
