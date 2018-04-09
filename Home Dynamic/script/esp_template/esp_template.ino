//Sample code provided by Ace44@instructable
#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>

const char *ssid = "%SSID%";
const char *password = "%WIFIPASSWORD%";
char wiFiHostname[] = "%HOSTNAME%";

ESP8266WebServer server(80);
const char* www_username = "%WEB_USERNAME%";
const char* www_password = "%WEB_PASSWORD%";

const int led = 2; //GPIO2
void handleRoot() {
if(!server.authenticate(www_username, www_password))
  return server.requestAuthentication();
int size=1000;
char temp[size];
int sec = millis() / 1000;
int min = sec / 60;
int hr = min / 60;
snprintf ( temp, size,
"<p>%MODULE_NAME%_On:switch|on_Off:switch|off</p>");

server.send (200,"text/html",temp); 
}
void setup() {
delay(1000);
WiFi.softAP(ssid, password);
IPAddress myIP = WiFi.softAPIP();
pinMode(led, OUTPUT);
digitalWrite ( led, HIGH );
//URLs available to query

server.on("/info", handleRoot);
server.on ( "/switch/on", turnON ); 
server.on ( "/switch/off", turnOFF );
server.begin();
Serial.println("HTTP server started");
}
void turnON(){
if(!server.authenticate(www_username, www_password))
  return server.requestAuthentication();
digitalWrite ( led, HIGH );
int size=1000;
char temp[size];
int sec = millis() / 1000;
int min = sec / 60;
int hr = min / 60;
snprintf ( temp, size,
"<p>ESP-1 is now on</p>");
server.send ( 200, "text/html", temp);
}
void turnOFF(){
if(!server.authenticate(www_username, www_password))
  return server.requestAuthentication();
digitalWrite ( led, LOW );
int size=1000;
char temp[size];
int sec = millis() / 1000;
int min = sec / 60;
int hr = min / 60;
snprintf ( temp, size,
"<p>ESP-1 is now off</p>"
);
server.send ( 200, "text/html", temp);
}
void loop() {
server.handleClient();
}
