/*
  MIT License

  Copyright (c) 2020 Lee Kyung-ha <i_am@nulleekh.com>

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/

#include <SPI.h>
#include <SD.h>
#include <Wire.h>

#define BUTTON_PIN 5
#define SD_CS_PIN 4
#define SENSOR_PIN A0

#define STANDBY_LED 2
#define RECORDING_LED 3

#define LOGGING_INTERVAL 1000

File log_file;

int current_status = 0;

double sensor_value;
double current_voltage;

int CheckCardType(void);
int CheckCardSize(void);
int OperateLogging(void);

void setup() {
  Serial.begin(9600);

  if (!SD.begin(4)) {
    Serial.println("ERROR : Initialization failed.");
    while (1);
  }
    Serial.println("INFO : Initialization succeed.");

  pinMode (BUTTON_PIN, INPUT);
  pinMode (STANDBY_LED, OUTPUT);
  pinMode (RECORDING_LED, OUTPUT);

  digitalWrite(STANDBY_LED, HIGH);
  digitalWrite(RECORDING_LED, LOW);

  log_file = SD.open("log_file.txt", FILE_WRITE);
}

void loop() {
  if (current_status == 0) {
    if (digitalRead(BUTTON_PIN) == HIGH) {
      current_status = 1;

      delay(3000);
    }
  }
  else if (current_status == 1) {
    digitalWrite(STANDBY_LED, LOW);
    digitalWrite(RECORDING_LED, HIGH);
    OperateLogging();
  }
  else if (current_status = 2) {
    digitalWrite(STANDBY_LED, HIGH);
    digitalWrite(RECORDING_LED, HIGH);
    delay(600);
    digitalWrite(STANDBY_LED, LOW);
    digitalWrite(RECORDING_LED, LOW);
    delay(300);
    Serial.print("INFO : ");
    Serial.println("Please turn off the device and change SD card for next experiment.");
  }
  else if (current_status = 3) {
    digitalWrite(STANDBY_LED, HIGH);
    digitalWrite(RECORDING_LED, HIGH);
    delay(300);
    digitalWrite(STANDBY_LED, LOW);
    digitalWrite(RECORDING_LED, LOW);
    delay(600);
    Serial.print("ERROR : ");
    Serial.println("Please turn off the device and check your SD sd_card.");
  }
}

int OperateLogging(void) {

  // if the file opened okay, write to it:
  if (log_file) {
    Serial.println("INFO : Log file is successfully opened.");
    Serial.println("INFO : Logging started.");
    
    while (digitalRead(BUTTON_PIN) == LOW) {
      sensor_value = analogRead (SENSOR_PIN); 
      current_voltage = sensor_value*5.0/1024.0;
  
      Serial.print ("Volatage(V) : ");
      Serial.println (current_voltage);

      log_file.println(current_voltage);
      delay(LOGGING_INTERVAL);
    }
    Serial.println("INFO : Logging ended.");
    current_status = 2;
    log_file.close();
    Serial.println("INFO : Log file is successfully saved.");
    
    Serial.println("done.");
  } 
  else {
   Serial.println("error opening test.txt");
  }
        

    return 0;
}
