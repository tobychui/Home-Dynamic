#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <string>
#include <SoftwareSerial.h>
#include <ESP8266HTTPClient.h>

AsyncWebServer server(80);

//Global variables for module
String CurrentStatus = "ON";
String deviceUuid = "a2f0e5d3-bbfc-4fa6-a98a-5c353d520162";
String deviceName = "Standard Panel Controller";
String identifier = "panel.std.hd.com.imuslab";
const char* ssid = "Toby Room Automation";
const char* password = "homedynamicsystem";

String endpointURL = ""; //Record the endpoint where the btn press event should fires. Must be in HTTP.
int buttonStates[] = {1,1,1}; //Record the button state before the change of page
String inString;

SoftwareSerial HMIserial(D6,D7); // RX, TX

void sendCommand(String cmd){
  HMIserial.print(cmd);
  HMIserial.write(0XFF);
  HMIserial.write(0XFF);
  HMIserial.write(0XFF);
}

//Function to update GUI Button States given button (0,1,2) and state (0,1)
void updateButtonStates(int b, int s){
  if (b == 0){
    //Update the first button
    if (s == 1){
      sendCommand("b0.pic=4");
      sendCommand("b0.pic2=3");
    }else{
      sendCommand("b0.pic=3");
      sendCommand("b0.pic2=4");
    }
    buttonStates[0] = s;
  }else if (b == 1){
    //Update the 2nd button
    if (s == 1){
      sendCommand("b1.pic=4");
      sendCommand("b1.pic2=3");
    }else{
      sendCommand("b1.pic=3");
      sendCommand("b1.pic2=4");
    }
    buttonStates[1] = s;
  }else if (b == 2){
    //Update the 3rd button
    if (s == 1){
      sendCommand("b2.pic=4");
      sendCommand("b2.pic2=3");
    }else{
      sendCommand("b2.pic=3");
      sendCommand("b2.pic2=4");
    }
    buttonStates[2] = s;
  }
  
}


void setup() {
  //Start HMIserial
  HMIserial.begin(9600);
  
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

  //Initiate HMI screen display to control
  sendCommand("page 1");

//Display Wifi Connection Information
Serial.println("WiFi connected");  
Serial.println("IP address: ");
Serial.println(WiFi.localIP());

//Async functions for web services

//Handle index delivery
server.on("/uuid", HTTP_GET, [](AsyncWebServerRequest *request){
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
  String resp = String("{\"btn1\":" + String(buttonStates[0]) + ",\"btn2\":" + String(buttonStates[1]) + ",\"btn3\":" + String(buttonStates[2]) + "}");
  AsyncWebServerResponse *response = request->beginResponse(200, "application/json", resp);
  response->addHeader("Access-Control-Allow-Origin","*");
  request->send(response);
});


//Handle button states update
server.on("/setbtn", HTTP_GET, [](AsyncWebServerRequest *request){
  int valid = true;
  //Get the argument from request
  int button = 0;
  int state = 0;
  String inarg;
  if(request->hasArg("b")){
    inarg = request->arg("b");
    if (inarg.indexOf("0") >= 0){
      button = 0;
    }else if (inarg.indexOf("1") >= 0){
      button = 1;
    }else if (inarg.indexOf("2") >= 0){
      button = 2;
    }
  }else{
    valid = false;
  }
  if(request->hasArg("s")){
    inarg = request->arg("s");
     if (inarg.indexOf("0") >= 0){
      state = 0;
    }else if (inarg.indexOf("1") >= 0){
      state = 1;
    }
  }else{
    valid = false;
  }
  String resptxt = "OK";
  if (!valid){
    //Missing arguments.
    resptxt = "ERROR. Missing arguments.";
  }else{
    //Selection valid. Update the UI
    updateButtonStates(button,state);
  }
  AsyncWebServerResponse *response = request->beginResponse(200, "text/plain",resptxt);
  response->addHeader("Access-Control-Allow-Origin","*");
  request->send(response);

});

//Handle endpoint set
server.on("/setep", HTTP_GET, [](AsyncWebServerRequest *request){
  int valid = true;
  if(request->hasArg("ep")){
    endpointURL = request->arg("ep");
    Serial.println("Updating action endpoint to " + endpointURL);
  }else{
    valid = false;
  }

  String resp = "OK";
  if (!valid){
    resp = "ERROR. Unset ep paramter.";
  }
  AsyncWebServerResponse *response = request->beginResponse(200, "text/plain", "OK");
  response->addHeader("Access-Control-Allow-Origin","*");
  request->send(response);
});

//Start Server
server.begin();
}

/*
void INIT(){
  Serial.println("Init requested");
  sendCommand("page 1");
  sendDone();
}

void showLicense(){
  int size=1000;
  char temp[size];
  snprintf( temp, size, "<table> <tbody> <tr> <td>Device Type: </td> <td>IMUS Standard Panel Controller</td> </tr> <tr> <td>Init Type:</td> <td>Static LAN</td> </tr> <tr> <td>Manufacture</td> <td>IMUS Laboratory, Prototype</td> </tr> <tr> <td>License </td> <td>CopyRight IMUS Laboratory, 2019</td> </tr> </tbody> </table> <p>&nbsp;</p>");
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

void toggle(){
  Serial.println("Ignore Toggle Request.");
  sendDone();
}


void Status(){
  char charBuf[50];
  String resp = String("{\"btn1\":" + String(btnStates[0]) + ",\"btn2\":" + String(btnStates[1]) + ",\"btn3\":" + String(btnStates[2]) + "}");
  resp.toCharArray(charBuf, 50);
  server.sendHeader("Access-Control-Allow-Origin","*");
  server.send(200, "application/json", charBuf);
}

*/

String IpAddress2String(const IPAddress& ipAddress)
{
  return String(ipAddress[0]) + String(".") +\
  String(ipAddress[1]) + String(".") +\
  String(ipAddress[2]) + String(".") +\
  String(ipAddress[3])  ;
}

void HTTPGET(String url){  
  HTTPClient http;
  Serial.println("Requesting: " + url);
  http.begin(url);
  int httpCode = http.GET();
   
  if (httpCode > 0) {
    if (httpCode == 404){
      Serial.print("ERROR. Endpoint not found or offline: ");
    }else if (httpCode == 200){
      String resp = http.getString();
      Serial.println(resp);
    }
    Serial.println(httpCode);
  }
   
  http.end();
}

void loop() {
  //Check if there anythings input from HMI Display
  while (HMIserial.available()) {
    delay(3);  //delay to allow buffer to fill
    if (HMIserial.available() >0) {
      char c = HMIserial.read();  //gets one byte from serial buffer
      inString += c; //makes the string readString
    }
  }
  
  if (inString != ""){
    Serial.println(inString);
    if (inString.indexOf("btn1") >= 0){
      if (endpointURL != ""){
        HTTPGET(endpointURL + "?pid=" + deviceUuid + "&btn=0");
        Serial.println("Button 1 pressed");
      }
    }else if (inString.indexOf("btn2") >= 0){
      if (endpointURL != ""){
        HTTPGET(endpointURL + "?pid=" + deviceUuid + "&btn=1");
        Serial.println("Button 2 pressed");
      }
    }else if (inString.indexOf("btn3") >= 0 ){
      if (endpointURL != ""){
        HTTPGET(endpointURL + "?pid=" + deviceUuid + "&btn=2");
        Serial.println("Button 3 pressed");
      }
    }else if (inString.indexOf("info") >= 0){
      //Switching to info page. Updates with information in ram
      //Update WiFi SSID
      sendCommand("t3.txt=\"" + String(ssid) + "\"");

      //Update Device UUID
      sendCommand("t2.txt=\"" + deviceUuid + "\"");

      //Update Device IP
      sendCommand("t1.txt=\"" + IpAddress2String(WiFi.localIP()) + "\"");

      sendCommand("t0.txt=\"Connected\"");
      
    }else if (inString.indexOf("main") >= 0){
      //Restore button states according to button state info
      updateButtonStates(0,buttonStates[0]);
      updateButtonStates(1,buttonStates[1]);
      updateButtonStates(2,buttonStates[2]);
    }
    inString = "";
  }
  
}
