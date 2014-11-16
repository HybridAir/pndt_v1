//push f9 to compile, then f6 to compile everything and upload
//reset the device on the 4th f6 compile status line
//right click makefile and do make to upload
#include <Arduino.h>

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Time.h>
#include <DS1307RTC.h>
#include <EEPROM.h>

#define OLED_DC     4
#define OLED_CS     6
#define OLED_RESET  12
Adafruit_SSD1306 display(OLED_DC, OLED_RESET, OLED_CS);

const int numReadings = 20;
int readings[numReadings];      // the readings from the analog input
int index = 0;                  // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average

int BL = 10;
int BC = 9;
int BR = 8;

int OFF = 1;
int ON = 0;
int CHRG = 5;

#define TMP A5


void setup();
void loop();
int checkCharge();
String rtcStatus;
byte mode;


void setup() {
    analogReference(EXTERNAL);
    pinMode(BL, INPUT);
    pinMode(BC, INPUT);
    pinMode(BR, INPUT);
    pinMode(OFF, OUTPUT); //off
    pinMode(ON, OUTPUT); //on
  display.begin(SSD1306_SWITCHCAPVCC);
  display.display();
  delay(1000);
  display.clearDisplay();   // clears the screen and buffer

  display.setTextSize(4);
  display.setTextColor(WHITE);
  display.setTextWrap(false);
  
  if(RTC.detect() ==  0) {
      rtcStatus = "on";
  }
  else {
      rtcStatus = "off";
  }
  setSyncProvider(RTC.get);   // the function to get the time from the RTC
      //  RTC.set(1416113806);
      //setTime(1416113806);
  
  for (int thisReading = 0; thisReading < numReadings; thisReading++)
    readings[thisReading] = 0;   
  
  byte reg = 0;
byte value;

value = EEPROM.read(reg);	//read the first register only
if(value > 9) {
	mode = 0;
}
else {
  mode = value;
}
byte out = mode + 1;
EEPROM.write(reg, out);
  
}

int x = 130;
int last = 0;



void loop() {
//  x = x - 1;
//  display.clearDisplay();
//  display.setCursor(x, 0);
//  display.println("Hello, world.");
//  display.display();
//  if(x < -650) {
//    x = 130;
//  }
  
  
//    display.clearDisplay();
//    display.setCursor(0, 0);
//    int sensorValue = analogRead(A0);   //read the A1 pin value
//    float voltage = sensorValue * (3.31 / 1024.0);   //convert the value to a true voltage.
//    display.println(voltage);
//    display.display();
//    delay(100);
    
    
      total= total - readings[index];         
  // read from the sensor: 
      analogRead(A0);
      delay(10);
  readings[index] = analogRead(A0); 
  // add the reading to the total:
  total= total + readings[index];       
  // advance to the next position in the array:  
  index = index + 1;                    

  // if we're at the end of the array...
  if (index >= numReadings)              
    // ...wrap around to the beginning: 
    index = 0;                           

  // calculate the average:
  average = total / numReadings;         
  
      display.clearDisplay();
    display.setCursor(0, 0);
    float voltage = average * (2.495 / 1024.0);   //convert the value to a true voltage.
    display.setTextSize(4);
    display.println((voltage * 2));
    
    if (checkCharge() == 0) {
        display.setTextSize(2);
        display.println("Unplugged");
        if (last == 1) {
            digitalWrite(ON, LOW);
            delay(10);
            digitalWrite(OFF, HIGH);
        delay(10);
        digitalWrite(OFF, LOW);
        }
    }
    else if(checkCharge() == 1) {
        display.setTextSize(2);
        display.println("Charging");
        last = 1;
        digitalWrite(ON, HIGH);
    }
    else {
        display.setTextSize(2);
        display.println("Charged");
        last = 1;
        digitalWrite(ON, HIGH);
    }
    
    analogRead(TMP);
    delay(10);
    long temp = analogRead(TMP);
    display.setTextSize(1);
    display.print(((temp * (2495.0 / 1024.0)) - 500) / 10.0);
    
    display.print(" ");
    display.print(hourFormat12());
    display.print(minute());
    display.print(second());
    display.print(" ");
    display.print(rtcStatus);
    display.print(" ");
    display.println(mode);
    
    if(digitalRead(BL)) {
        display.print("1");
        digitalWrite(OFF, HIGH);
        delay(10);
        digitalWrite(OFF, LOW);
    }
    if(digitalRead(BC)) {
        display.print("2");
    }
    if(digitalRead(BR)) {
        display.print("3");
        digitalWrite(ON, HIGH);
        delay(10);
        digitalWrite(ON, LOW);
    }
    display.display();
    delay(100);
}

int checkCharge() {
    digitalWrite(CHRG, LOW);
    if(digitalRead(CHRG)) {
        return 2;
    }
    digitalWrite(CHRG, HIGH);
    if(digitalRead(CHRG)) {
        digitalWrite(CHRG, LOW);
        return 0;
    }
    
    digitalWrite(CHRG, LOW);
    return 1;
}