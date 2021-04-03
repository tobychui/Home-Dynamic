/*
 * Home Dynamic System v2
 * Designed by tobychui
 * 
 * This is a basic IoT Switch that support single channel ON / OFF function only
 * 
 */

#include <ESP8266WiFi.h>        // Include the Wi-Fi library
#include <ESP8266WiFiMulti.h>   // Include the Wi-Fi-Multi library
#include <ESP8266mDNS.h>        // Include the mDNS library
#include <ESP8266WebServer.h>   // Include the WebServer library

//Change the properties of the IoT device
const String DeviceName = "Switch";     //The name of this IoT device
const int ListeningPort = 12110;        //The port where this IoT device listen
int signalOutputPin = LED_BUILTIN;      //The pin to activate during on, default LED pins as demo
bool poweredOn = true;                  //The current power state of the switch

//Library Objects
ESP8266WiFiMulti wifiMulti;                // Create an instance of the ESP8266WiFiMulti class, called 'wifiMulti'
ESP8266WebServer server(ListeningPort);    //Create an Web Server on the listening port

//Change the WiFi Settings
void WiFiConfig(){
  wifiMulti.addAP("Toby Room Automation", "homedynamicsystem"); 
  //Add more WiFi AP here if nessary
  //wifiMulti.addAP("ssid_from_AP_2", "your_password_for_AP_2");
  //wifiMulti.addAP("ssid_from_AP_3", "your_password_for_AP_3");
}

//Inject zeroconf attr into the MDNS respond (For scanning by ArozOS)
void MDNSDynamicServiceTxtCallback(const MDNSResponder::hMDNSService p_hService) {
    //Define the domain of the HDSv2 devices
    MDNS.addDynamicServiceTxt(p_hService, "domain","hds.arozos.com");
    MDNS.addDynamicServiceTxt(p_hService, "protocol","hdsv2");

    //Define the OEM written values
    MDNS.addDynamicServiceTxt(p_hService, "uuid",getMacAddress());
    MDNS.addDynamicServiceTxt(p_hService, "model","Switch");
    MDNS.addDynamicServiceTxt(p_hService, "vendor","HomeDynamic Project");
    MDNS.addDynamicServiceTxt(p_hService, "version_minor","0.00");
    MDNS.addDynamicServiceTxt(p_hService, "version_build","0");
}
  

void hostProbeResult(String p_pcDomainName, bool p_bProbeResult) {
  MDNS.setDynamicServiceTxtCallback(MDNSDynamicServiceTxtCallback);
}

void setup() {
  //Use 115200 baudrate on serial monitor if you want to see what is happening to the device
  Serial.begin(115200);
  delay(10);
  Serial.println('\n');

  //Set output pins as OUTPUT and default it to HIGH
  pinMode(signalOutputPin, OUTPUT);
  digitalWrite(signalOutputPin, HIGH);
 
  //Start WiFi Conenction Routines
  WiFiConfig();
  
  Serial.println("Connecting ...");
  while (wifiMulti.run() != WL_CONNECTED) {
    delay(500);
    Serial.print('.');
  }
  Serial.println('\n');
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());
  
  //Startup MDNS Responder
  MDNS.setHostProbeResultCallback(hostProbeResult);
  
  if (!MDNS.begin(DeviceName)) {             // Start the mDNS responder for esp8266.local
    Serial.println("Error setting up MDNS responder!");
  }

  //Advertise the port that you are using
  MDNS.addService("http", "tcp", ListeningPort);
  Serial.println("mDNS responder started");

  //Startup the Web Server Endpoints
  delay(100);
  server.on("/", handle_index);
  server.on("/status", handle_status);
  server.on("/eps", handle_endpoints);
  server.on("/on", handle_on);
  server.on("/off", handle_off);
  
  server.begin();
  Serial.println("HTTP server started");
  Serial.print("Listening on port: ");
  Serial.println(ListeningPort);
}

//Handlers for Web Server
void handle_index() {
  server.send(200, "text/html", ""); 
}

//Handle turning on the switch
void handle_on() {
  Serial.println("Turned ON");
  digitalWrite(signalOutputPin, HIGH);
  poweredOn = true;
  server.send(200, "text/html", "OK"); 
}

//Handle turning off the switch
void handle_off() {
  Serial.println("Turned OFF");
  digitalWrite(signalOutputPin, LOW);
  poweredOn = false;
  server.send(200, "text/html", "OK"); 
}



void handle_status() {
  String powerState = "ON";
  if (poweredOn == false){
    powerState = "OFF";
  }
  server.send(200, "application/json", "{\"Power\":\"" + powerState + "\"\}"); 
}

void handle_endpoints() {
  server.send(200, "application/json", "[{\
  \"Name\": \"ON\",\
  \"RelPath\":\"on\",\
  \"Desc\":\"Switch on the device attached to the switch\",\
  \"Type\":\"none\",\
  \"AllowRead\":false,\
  \"AllowWrite\":true\
},{\
  \"Name\": \"OFF\",\
  \"RelPath\":\"off\",\
  \"Desc\":\"Switch off the device attached to the switch\",\
  \"Type\":\"none\",\
  \"AllowRead\":false,\
  \"AllowWrite\":true\
}\
]"); 
}


//Main Loop
void loop() { 
   server.handleClient();
   MDNS.update();
 }
