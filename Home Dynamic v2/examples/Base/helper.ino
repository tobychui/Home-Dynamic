//Get MAC Address of the ESP8266, require WiFi
String MACString;
const char* getMacAddress(){
  unsigned char mac[6];
  WiFi.macAddress(mac);
  MACString = "";
  for (int i = 0; i < 6; ++i) {
    MACString += String(mac[i], 16);
    if (i < 5){
      MACString += '-';
    }
  }
  
  const char* _result = MACString.c_str();
  return _result;
}
