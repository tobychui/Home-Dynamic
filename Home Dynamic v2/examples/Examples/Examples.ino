/*
 * Home Dynamic System v2
 * Designed by tobychui
 * 
 * This is an example of making use of all avaible endpoints definations
 * 
 */

#include <ESP8266WiFi.h>        // Include the Wi-Fi library
#include <ESP8266WiFiMulti.h>   // Include the Wi-Fi-Multi library
#include <ESP8266mDNS.h>        // Include the mDNS library
#include <ESP8266WebServer.h>   // Include the WebServer library

//Change the properties of the IoT device
const String DeviceName = "HDsv2-Test"; //The name of this IoT device
const int ListeningPort = 12110;        //The port where this IoT device listen

//Runtime values
String stringInput = "default";
int integerInput = 0;
float floatInput = 2.5;
bool booleanInput = true;

//Library Objects
ESP8266WiFiMulti wifiMulti;     // Create an instance of the ESP8266WiFiMulti class, called 'wifiMulti'
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
    MDNS.addDynamicServiceTxt(p_hService, "model","Test Unit");
    MDNS.addDynamicServiceTxt(p_hService, "vendor","HomeDynamic Project");
    MDNS.addDynamicServiceTxt(p_hService, "version_minor","0.00");
    MDNS.addDynamicServiceTxt(p_hService, "version_build","1");
}
  

void hostProbeResult(String p_pcDomainName, bool p_bProbeResult) {
  MDNS.setDynamicServiceTxtCallback(MDNSDynamicServiceTxtCallback);
}

void setup() {
  //Use 115200 baudrate on serial monitor if you want to see what is happening to the device
  Serial.begin(115200);
  delay(10);
  Serial.println('\n');

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
  server.on("/string", handle_string);
  server.on("/integer", handle_integer);
  server.on("/float", handle_float);
  server.on("/bool", handle_bool);
  server.on("/output", handle_output);
  
  server.begin();
  Serial.println("HTTP server started");
  Serial.print("Listening on port: ");
  Serial.println(ListeningPort);
}

//Handlers for Web Server
void handle_index() {
  server.send(200, "text/html", "index"); 
}

void handle_string() {
  String value = server.arg("value");
  Serial.println(value);
  stringInput = value;
  //Use value (String) to do something
  server.send(200, "text/html", "OK"); 
}

void handle_integer() {
  String value = server.arg("value");
  int number = value.toInt();
  Serial.println(number);
  integerInput = number;
  //Use number (int) to do something
  server.send(200, "text/html", "OK"); 
}

void handle_float() {
  String value = server.arg("value");
  float floatNumber = value.toFloat();
  Serial.println(floatNumber);
  floatInput = floatNumber;
  //Use floatNumber to do something else
  server.send(200, "text/html", "OK"); 
}

void handle_bool() {
  String value = server.arg("value");
  bool result = false;
  if (value == "true"){
    result = true;
    Serial.println("True");
  }else{
    Serial.println("False");
  }

  booleanInput = result;

  //Use result to do something else
  server.send(200, "text/html", "OK"); 
}

void handle_output() {
  Serial.println("Hello World");
  server.send(200, "text/html", "OK"); 
}

//This function return the status of the current device. 
//If the name of status matches one of the input endpoints, the value will be used as auto fill
void handle_status() {
  String boolValue = "true";
  if (booleanInput == false){
    boolValue = "false";
  }
  server.send(200, "application/json", "{\
  \"Global Status\":\"It is working\",\
  \"String Input\":\"" + stringInput + "\",\
  \"Integer Input\":" + String(integerInput) + ",\
  \"Float Input\":" + String(floatInput, 2) + ",\
  \"Boolean Input\":" + boolValue + "\
}"); 
}

//This function define all the endpoint accessiable in this device
void handle_endpoints() {
  server.send(200, "application/json", "[\
  {\
    \"Name\": \"String Input\",\
    \"RelPath\":\"string\",\
    \"Desc\":\"Print out your input string to serial\",\
    \"Type\":\"string\",\
    \"Regex\":\"/[A-Za-z0-9]/g\"\
  },\
  {\
    \"Name\": \"Integer Input\",\
    \"RelPath\":\"integer\",\
    \"Desc\":\"Print out your input integer to serial\",\
    \"Type\":\"integer\",\
  \"Min\":0,\
  \"Max\":10\
  },\
  {\
    \"Name\": \"Float Input\",\
    \"RelPath\":\"float\",\
    \"Desc\":\"Print out your input float to serial\",\
    \"Type\":\"float\",\
    \"Min\":0,\
    \"Max\":10,\
    \"Steps\":0.1\
  },\
  {\
    \"Name\": \"Boolean Input\",\
    \"RelPath\":\"bool\",\
    \"Desc\":\"Print out your input string to \",\
    \"Type\":\"bool\"\
  },\
  {\
    \"Name\": \"Print Hello World\",\
    \"RelPath\":\"output\",\
    \"Desc\":\"Reply your request with Hello World\",\
    \"Type\":\"none\"\
  }\
]"); 
}


//Main Loop
void loop() { 
   server.handleClient();
   MDNS.update();
 }
