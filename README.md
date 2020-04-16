
# RollerShutterFirmware: Firmware for an esp32 to control VELUX INTEGRA shutters
Authors: Simon Bogutzky

License: [MIT](https://opensource.org/licenses/MIT)

Version: 1.0.0

Document version: 1.0.0 

Date: 16/04/2020

## What is the RollerShutterFirmware?
The RollerShutterFirmware is a firmware for an esp32 to control for example VELUX INTEGRA via an integrated web server or the [RollerShutterApp](https://github.com/sbogutzky/RollerShutterApp/).

## Using the RollerShutterFirmware

### Hardware Installation Instructions
I use for the wireless connection a NodeMCU-32S ESP32 development board (similar to [Waveshare NodeMCU-32S ESP32 Development Board](https://www.amazon.com/dp/B07TXNK6P1/ref=cm_sw_em_r_mt_dp_U_DbcMEbPJSWKTA)), based on the popular [ESP32](http://espressif.com/) chipset.

I open the remote of the VELUX INTEGRA shutters and add wires to the three buttons, to the power supply and the ground. The buttons are connected (up, green wire) to GPIO14, (stop, blue wire) to GPIO13 and (down, orange wire) to GPI012 of the WiFi3 click. 

Putting the NodeMCU-32S ESP32 development board:  

1. press and hold the BOOT button
2. press the Upload button in the Arduino IDE
3. after the message "Connecting..." appears in the Arduino IDE, release the BOOT button

### Programming ESP32 Development Board in the Arduino IDE

Download the [Arduino IDE](https://www.arduino.cc/en/Main/Software). 

*I use macOS Catalina and the Version 1.8.12. The IDE uses Python and you have to create the right links to it. Type in the terminal:*

```
sudo ln -s /usr/bin/python3 /usr/local/bin/python3
sudo mkdir /usr/local/bin/
```
Arduino IDE offers support for esp32. All you have to do is to go to File / Preferences, and on Additional Boards Manager URL add “https://dl.espressif.com/dl/package_esp32_index.json”. In Arduino IDE, Tools, setup the communication to board. I select: DOIT ESP32 DEVKIT V1. Do not forget to put the development board into firmware download mode before uploading your code.

### Example Usage

Open the *.ino file and upload the firmware. Replace 

```
const char* ssid = "";
const char* password = "";
```

with your WiFi settings.

In the serial monitor you will see the IP address of the web server e. g. 192.168.2.56.

![Arduino serial monitor](images/arduino-serial-monitor.jpg)

*Arduino serial monitor*

In your favorite browser you can type the following commands in the address bar:

```
http://192.168.2.56/up - for moving the shutters up
http://192.168.2.56/stop - for stopping the shutters
http://192.168.2.56/up - for moving the shutters down
http://192.168.2.56/version - getting the firmware version
```

You can also use the [RollerShutterApp](https://github.com/sbogutzky/RollerShutterApp/). Make sure you change the IP address in the source code of the platform code.

## Author and Contribution
As by the License this is free software released by the Simon Bogutzky. The author (Simon Bogutzky) welcomes external contributors to freely use and extend this software. If you need some help, please write an [issue](https://github.com/sbogutzky/RollerShutterFirmware/issues).
