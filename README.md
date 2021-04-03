![](img/hdsv2.png)

# Home Dynamic

The Home Dynamic is a very simple protocol for ESP8266 based IoT control and communication. Designed to be run under a trusted local area network with NAT gateway and a Raspberry Pi running [ArozOS](https://github.com/tobychui/arozos) as the control server.



### Hardware / System Requirement

- Arduino IDE 1.18.13 or above with ESP8266 core installed
- ESP8266 powered microcontroller development board (e.g. Wemos D1, NodeMCU)
- WiFi router running with Open (aka no password) or *WPA2*-*PSK* encryption
- A Raspberry Pi with [ArozOS](https://github.com/tobychui/arozos) v1.111 installed (Or other equivalent of IoT Hub function on ArozOS)



### Installation

1. Download the repo as zip file
2. Select the version of HDS you want to use (recommend the latest one for more features)
3. Open its example folder. select one best fit your needs and open it with Arduino IDE
4. Add in your WiFi SSID and password, change the device name and add in your required features
5. Flash the program into the ESP8266 using build in / external USB to TTL programmer
6. Reset the ESP8266 development board and scan the device in [ArozOS](https://github.com/tobychui/arozos) IoT Hub 





