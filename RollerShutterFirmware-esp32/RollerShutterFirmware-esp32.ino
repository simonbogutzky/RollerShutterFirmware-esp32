/*
  Roller Shutter Firmware
  The MIT License (MIT)
  Copyright (c) 2020 Simon Bogutzky
  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
  THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
  
  v2.0.0
*/

#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include "Config.h"

const char* vers = "2.0.0";
const int ledPin = 5;
const int dwnPin = 21;
const int stpPin = 22;
const int upPin = 23;

const int dwnHour = 11;
const int upHour = 7;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 7200, 60000);


void setup()
{
  Serial.begin(9600);
  Serial.println();

  bool connected = connectToWiFi();

  if (connected) {
    timeClient.begin();
    timeClient.update();
    Serial.println(timeClient.getFormattedTime());

    if(timeClient.getHours() == upHour) {
      up();
    }

    if(timeClient.getHours() == dwnHour) {
      down();
    }
  }

  esp_sleep_enable_timer_wakeup(sleepTimeInMicroSeconds);
  esp_deep_sleep_start();
}

void loop()
{
}

bool connectToWiFi()
{
  int ledState = 0;
  Serial.println("Connecting to WiFi network: " + String(wiFiSsid));
  WiFi.begin(wiFiSsid, wiFiPassword);

  unsigned long timeout = millis();
  while (WiFi.status() != WL_CONNECTED)
  {
    if (millis() - timeout > 10000)
    {
      Serial.println();
      Serial.println("WiFi connection timeout");
      return false;
    }

    digitalWrite(ledPin, ledState);
    ledState = (ledState + 1) % 2;
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  return true;
}

void down() {
  Serial.println("Down");
  resetCmd();
  digitalWrite(dwnPin, LOW);
}

void up() {
  Serial.println("Up");
  resetCmd();
  digitalWrite(upPin, LOW);
}

void resetCmd() {
  digitalWrite(dwnPin, HIGH);
  digitalWrite(stpPin, HIGH);
  digitalWrite(upPin, HIGH);
  delay(2000);
}