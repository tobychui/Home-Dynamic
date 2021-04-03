#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <string>
#define OUTPIN 0

ESP8266WebServer server(80);

//Global variables for module
String CurrentStatus = "ON";
String deviceUuid = "e1d8b793-02d6-4b2d-8eb0-e7378dae4a72";
String deviceName = "Standard Relay Module";
String identifier = "relay.std.hd.com.imuslab";
const char* ssid = "Toby Room Automation";
const char* password = "homedynamicsystem";

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

//Declare Output Pins
pinMode(OUTPIN, OUTPUT);

//Start Server
server.begin();
}

void showLicense(){
  int size=1000;
  char temp[size];
  snprintf( temp, size, "<table> <tbody> <tr> <td>Device Type: </td> <td>IMUS Standard Relay Module</td> </tr> <tr> <td>Init Type:</td> <td>Static LAN</td> </tr> <tr> <td>Manufacture</td> <td>IMUS Laboratory, Prototype</td> </tr> <tr> <td>License </td> <td>CopyRight IMUS Laboratory, 2018</td> </tr> </tbody> </table> <p>&nbsp;</p>");
  server.sendHeader("Access-Control-Allow-Origin","*");
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
  digitalWrite(OUTPIN,LOW);
  Serial.println("Turned ON");
  CurrentStatus = "ON";
  sendDone();
}

void OFF(){
  digitalWrite(OUTPIN,HIGH);
  Serial.println("Turned OFF");
  CurrentStatus = "OFF";
  sendDone();
}

void toggle(){
  if (strcmp(CurrentStatus.c_str(),"ON") == 0){
    digitalWrite(OUTPIN,HIGH);
    Serial.println("Toggled OFF");
    CurrentStatus = "OFF";
  }else{
    digitalWrite(OUTPIN,LOW);
    Serial.println("Toggled ON");
    CurrentStatus = "ON";
  }
  sendDone();
}


void Status(){
  int size=100;
  char temp[size];
  snprintf( temp, size, "");
  server.sendHeader("Access-Control-Allow-Origin","*");
  server.send(200, "text/html", CurrentStatus);
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
