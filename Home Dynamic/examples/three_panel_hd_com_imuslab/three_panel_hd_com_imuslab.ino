

#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <string>
#include <SoftwareSerial.h>

#include <ESP8266HTTPClient.h>

AsyncWebServer server(80);

//Global variables for module
String CurrentStatus = "ON";
String deviceUuid = "7837e1ac-485a-4b75-a30c-79c494c2ef07";
String deviceName = "Three Button Panel";
String identifier = "three.panel.hd.com.imuslab";
const char* ssid = "Toby Room Automation";
const char* password = "homedynamicsystem";

int val[] = {0,0,0};
String eps[] = {"","",""};

void httpGet(String ipa){
  if (ipa == ""){
    Serial.println("Press to undefined button. Ignoring request.");
    return;
  }
  String url = "http://" + ipa;
  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
    HTTPClient http;  //Declare an object of class HTTPClient
    http.begin(url);
    int httpCode = http.GET();                                                                  //Send the request
    if (httpCode > 0) { 
      String payload = http.getString();
      Serial.println(payload);
    }
    http.end();
  }
}

void setup() {
  //Initiate WiFi Setup
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

//Async functions for web services

//Handle index delivery
server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
  AsyncWebServerResponse *response = request->beginResponse(200, "text/html", "<table> <tbody> <tr> <td>Device Type: </td> <td>IMUS Standard Panel Controller</td> </tr> <tr> <td>Init Type:</td> <td>Static LAN</td> </tr> <tr> <td>Manufacture</td> <td>IMUS Laboratory, Prototype</td> </tr> <tr> <td>License </td> <td>CopyRight IMUS Laboratory, 2019</td> </tr> </tbody> </table> <p>&nbsp;</p>");
  response->addHeader("Access-Control-Allow-Origin","*");
  request->send(response);
});

//Handle UUID delivery
server.on("/uuid", HTTP_GET, [](AsyncWebServerRequest *request){
  AsyncWebServerResponse *response = request->beginResponse(200, "text/plain", deviceUuid);
  response->addHeader("Access-Control-Allow-Origin","*");
  request->send(response);
});

//Handle info delivery
server.on("/info", HTTP_GET, [](AsyncWebServerRequest *request){
  AsyncWebServerResponse *response = request->beginResponse(200, "text/plain", deviceName + "_" + identifier);
  response->addHeader("Access-Control-Allow-Origin","*");
  request->send(response);
});

//Handle status report
server.on("/status", HTTP_GET, [](AsyncWebServerRequest *request){
  String resp = "{\"ep0\":\"" + eps[0] + "\",\"ep1\":\"" + eps[1] + "\",\"ep2\":\"" + eps[2] + "\"}";
  AsyncWebServerResponse *response = request->beginResponse(200, "application/json", resp);
  response->addHeader("Access-Control-Allow-Origin","*");
  request->send(response);
});


//Handle endpoint set
server.on("/setep", HTTP_GET, [](AsyncWebServerRequest *request){
  int valid = true;
  int epNumber = 0;
  String newValue = "";
  String tmp = "";
  if(request->hasArg("ep")){
    tmp = request->arg("ep");
    if (tmp == "0"){
      epNumber = 0;
    }else if (tmp == "1"){
      epNumber = 1;
    }else if (tmp == "2"){
      epNumber = 2;
    }
  }else{
    valid = false;
  }

  if(request->hasArg("val")){
    newValue = request->arg("val");
  }else{
    valid = false;
  }

  String resp = "OK";
  if (!valid){
    //Data Invalid
    resp = "ERROR. Unset ep or val paramter.";
  }else{
    //Data valid. Update the endpoint value
    eps[epNumber] = newValue;
  }
  AsyncWebServerResponse *response = request->beginResponse(200, "text/plain", "OK");
  response->addHeader("Access-Control-Allow-Origin","*");
  request->send(response);
});



//Start Server
server.begin();

//Set required pins
pinMode(D1, INPUT);
pinMode(D2, INPUT);
pinMode(D3, INPUT);
}

void loop() {
  val[0] = digitalRead(D1);
  val[1] = digitalRead(D2);
  val[2] = digitalRead(D5);
  
  if (val[0] == 1){
    //Button pressed
    httpGet(eps[0]);
    Serial.println("Button pressed!");
    delay(1000);
  }

  if (val[1] == 1){
    //Button pressed
    httpGet(eps[1]);
    Serial.println("Button pressed!");
    delay(1000);
  }

  if (val[2] == 1){
    //Button pressed
    httpGet(eps[2]);
    Serial.println("Button pressed!");
    delay(1000);
  }
  
  
}
