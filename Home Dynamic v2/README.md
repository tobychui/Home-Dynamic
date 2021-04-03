![](/Home Dynamic v2/img/hdsv2.png)

# Home Dynamic v2

Home Dynamic v2 protocol is the main protocol used in ArozOS IoT Hub since v1.111. This version support the following features that v1 did not provide: 

- MDNS Discovery 
- Multiple WiFi SSID and Password
- Support Device defined input / output endpoint
- Support input type of String, Float, Integer and Boolean
- Much faster respond speed on ArozOS + HDSv2 then ArOZ Online Beta + HDSv1 (1 - 2 sec --> 500 - 800ms)



## Usage / Installation

1. Download the whole repo as zip file and open the Home Dynamic v2 folder
2. Pick one of the examples from the example folder that best fits your need
3. Modify the WiFi SSID, device name and implement the function you need using Arduino IDE
4. Flash your firmware into an ESP8266 development board 
5. Restart your ESP8266 by pressing the restart button



## Customization

### Change Device Status Return

To show the user what status this device current is, you can modify the ```handle_status()``` function and return a JSON object that fits your needs. The structure of the JSON object should be a key- value object where the key is the item name and the value is the value you want to show. Following is an example that shows a string.

```
{
  "status": "It is working"
}
```

Here is an example code of such function

```
server.send(200, "application/json", "{\
  \"status\": \"It is working\"\
}";
```



#### Updating input fields with status value

If you have defined a status key same as the endpoint description name, the endpoint input field will be used to show the status value instead of creating a new status interface. For example, you have defined a endpoint as follow:

```
{
  "Name": "Set Temp",
  "RelPath":"tmp",
  "Desc":"Set the temperature of the cooler",
  "Type":"integer",
  "AllowWrite":true
}
```

And you have a status as follow

```
{
	"Set Temp": "25"
}
```

Then the integer "25" will be shown inside the input field for "Set Temp"  if any update is called to the device.



### Change Endpoints / Functions Description

ArozOS use this function description to generate an User Interface for you to control the device. By changing the return value of this function, you can modify the control user interface to fit your needs. 

To change the endpoint supply by the IoT device, see  ```handle_endpoints()``` functions. In simple words, the endpoint should return a JSON object that contains the following structure

- Name (The name of the endpoint / function)
- Relpath (The relative path of the endpoint from the device root )
- Desc (The description on what this endpoint / function do)
- Type (A string that represent a type of response this endpoint expect to receive. Allow {string / integer/ float / bool / none})



Example return from this endpoint

```
[{
  "Name": "ON",
  "RelPath":"on",
  "Desc":"Switch on the device attached to the switch",
  "Type":"none",
  "AllowRead": false,
  "AllowWrite":true
},{
  "Name": "OFF",
  "RelPath":"off",
  "Desc":"Switch off the device attached to the switch",
  "Type":"none",
  "AllowRead": false,
  "AllowWrite":true
}
]

```



Example code in sketch

```
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
```





You can also supply additional requirement for the input. 

| Endpoint Data Type | Supported Additional Limitation Key | Example Value      |
| ------------------ | ----------------------------------- | ------------------ |
| string             | Regex                               | /[A-Za-z0-9]/g     |
| integer            | Min<br />Max                        | 0<br />10          |
| float              | Min<br />Max<br />Step              | 0<br />10<br />0.1 |

