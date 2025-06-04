# IoT-Front

## Table of contents
* [Description](#description)
* [Demostration](#demostration)
* [Technologies](#technologies)
* [Configure Container](#configure-container)
* [Flash Esp32](#flash-esp32)
* [Licence](#licence)

## Description
This project demonstrates how to use an ESP32 microcontroller to read data from two sensors, a photoresistor (LDR) and a DHT22 temperature and humidity sensor and publish the sensor readings via MQTT.
The LDR measures ambient light levels, and the DHT22 measures temperature and humidity. The ESP32 connects to a Wi-Fi network and uses the PubSubClient library to send the sensor data to an MQTT broker.
This project can be integrated with Node-RED for visualization, such as displaying the data on a UI gauge.

## Demostration

TODO

## Technologies
This project was created with:
* Docker and Docker compose
* Platformio
* Node-RED

## Configure Container
To run the container with NodeRed and Mosquitto you need to have Docker and Docker compose installed. After installing the required programs you can run the containers:

```
$ cd IoT-Front
$ docker compose up --build
```

Now Node-RED is in  [http://localhost:1880](http://localhost:1880) and Node-RED Dashboard in [http://localhost:1880/ui](http://localhost:1880/ui).

INFO: It is possible that Node-RED dashboard doen't shows the published info, in this case you have to modify MQTT in for all 3 sensors in Node-RED and change Mosquitto ip to your computer ip address.

## Flash Esp32
To flash you Esp32 you must have Visual Studio Code installed with docker extension. Now you have to run the container specifying the `--flash` flag with your respective `/dev/ttyUSB#` port:
```
$ ./run.sh --flash /dev/ttyUSB0
```

To check wich is your /dev/ttyUSB# you can use `ls /dev/ttyUSB*`. One of the most common problems with flashing Esp32 is that you don't have permissions to do this, to check if this is your case you can run `groups`, if you are on dialout group you are ok, in case you don't run this command and reboot your system.:
```
$ sudo usermod -aG dialout ${USER}
$ newgrp dialout
```

Now you need to configure your code to use your credentials.
First you need to open in Visual Studio Code src/main.cpp file and modify your SSID, Password and MQTT Broker IP address in lines 11, 12 and 15:

![image](https://github.com/user-attachments/assets/054ec77a-fcf7-4b12-acb9-061ae74340fd)

Now you have to hold BOOT button while clicking the flash button in vscode (this button only shows up when you have PlatformIO extension installed):

![image](https://github.com/user-attachments/assets/f67c9f84-7678-4873-b638-d49ab4057015)

## Licence
IoT-Front is available under the BSD-2-Clause license. See the LICENSE file for more details.
