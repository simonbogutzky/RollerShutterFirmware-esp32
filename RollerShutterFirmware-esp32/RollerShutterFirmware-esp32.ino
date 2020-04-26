/*
  Roller Shutter Firmware
  The MIT License (MIT)
  Copyright (c) 2020 Simon Bogutzky
  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
  THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
  
  v1.1.1
*/

#include <ESP32WebServer.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

const char* ssid     = "";
const char* password = "";

ESP32WebServer server(80);

const char* vers = "1.1.1";
const int dwn = 12;
const int stp = 13;
const int up = 14;

const int oledReset = 4;
const int screenWidth = 128;
const int screenHeight = 64;

Adafruit_SSD1306 display(screenWidth, screenHeight, &Wire, oledReset);

void handleRoot() {
  server.send(200, "text/plain; charset=utf-8", "Roller shutter webserver");
}

void handleNotFound(){
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void setup()
{
  pinMode(dwn, OUTPUT);
  pinMode(stp, OUTPUT);
  pinMode(up, OUTPUT);

  digitalWrite(dwn, HIGH);
  digitalWrite(stp, HIGH);
  digitalWrite(up, HIGH);
  
  Serial.begin(9600);
  delay(1000);
 
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3D)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  display.clearDisplay(); // clear the display buffer.

  drawHeader();
  drawConnecting();
  drawSsid();

  WiFi.begin(ssid, password);

  // Wait for connection
  String waitIndicator = ".";
  drawText(0, 36, stringToChar(waitIndicator), 1);
  display.display();
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
     waitIndicator.concat(".");
     drawText(0, 36, stringToChar(waitIndicator), 1);
     display.display();
  }

  display.clearDisplay();

  Serial.println("Draw connect info.");
  drawConnectInfo();

  server.on("/", handleRoot);

  server.on("/down", [](){
    resetCmd();
    digitalWrite(dwn, LOW);
    server.send(200, "text/plain", "down");
    display.clearDisplay();
    drawConnectInfo();
    drawCommand("down");
  });

  server.on("/stop", [](){
    resetCmd();
    digitalWrite(stp, LOW);
    server.send(200, "text/plain", "stop");
    display.clearDisplay();
    drawConnectInfo();
    drawCommand("stop");
  });

  server.on("/up", [](){
    resetCmd();
    digitalWrite(up, LOW);
    server.send(200, "text/plain", "up");

    display.clearDisplay();
    drawConnectInfo();
    drawCommand("up");
  });

  server.on("/version", [](){
    server.send(200, "text/plain", vers);
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void resetCmd() {
  digitalWrite(dwn, HIGH);
  digitalWrite(stp, HIGH);
  digitalWrite(up, HIGH);
  delay(2000);
}

void drawHeader() {
  String header = "Roller shutter v";
  header.concat(vers);
  drawText(0, 0, stringToChar(header), 1);
  display.display();
}

void drawConnecting() {
  String connecting = "Connecting to ";
  drawText(0, 12, stringToChar(connecting), 1);
  display.display();
}

void drawConnect() {
  String connecting = "Connect to ";
  drawText(0, 12, stringToChar(connecting), 1);
  display.display();
}

void drawSsid() {
  drawText(0, 24, stringToChar(ssid), 1);
  display.display();
}

void drawIp() {
  String ip = "IP: ";
  ip.concat(WiFi.localIP().toString());
  drawText(0, 36, stringToChar(ip), 1);
  display.display();
}

void drawConnectInfo() {
  drawHeader();
  drawConnect();
  drawSsid();
  drawIp();
}

void drawCommand(String command) {
  String commandString = "Command: ";
  commandString.concat(command);
  drawText(0, 48, stringToChar(commandString), 1);
  display.display();
}

void drawText(byte xPos, byte yPos, char *text, byte textSize)
{
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(xPos, yPos);
  display.setTextSize(textSize);
  display.print(text);
}

char* stringToChar(String command){
    if(command.length()!=0){
        char *p = const_cast<char*>(command.c_str());
        return p;
    }
}

void loop(void){
  server.handleClient();
}
