//push f9 to compile, right click makefile and make to upload
//reset the device on the 3rd f6 compile status line
//TODO: let the user know the device cannot be turned off when plugged in
#include <Arduino.h>

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <EEPROM.h>

#include "io.h"
#include "rtcTime.h"

#define OLED_DC     4
#define OLED_CS     6
#define OLED_RESET  12

#define OLED_WIDTH  128
Adafruit_SSD1306 display(OLED_DC, OLED_RESET, OLED_CS);

//const int numReadings = 20;
//int readings[numReadings];      // the readings from the analog input
//int index = 0;                  // the index of the current reading
//int total = 0;                  // the running total
//int average = 0;                // the average

//int BL = 10;
//int BC = 9;
//int BR = 8;
//
//int OFF = 1;
//int ON = 0;
//int CHRG = 5;

//#define TMP A5


void setup();
void loop();
void drawBtnBar();
int checkCharge();
String rtcStatus;
byte mode;

io inout;                                                                       //new instance of IO
rtcTime time;


void setup() {
  display.begin(SSD1306_SWITCHCAPVCC);
  display.display();
  delay(1000);
  display.clearDisplay();   // clears the screen and buffer

  display.setTextSize(4);
  display.setTextColor(WHITE);
  display.setTextWrap(true);
  
  setSyncProvider(RTC.get);
  
  //setSyncProvider(RTC.get);   // the function to get the time from the RTC
      //  RTC.set(1416113806);
      //setTime(1416113806);
  
//  for (int thisReading = 0; thisReading < numReadings; thisReading++)
//    readings[thisReading] = 0;   
  
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
    
    
//      total= total - readings[index];         
//  // read from the sensor: 
//      analogRead(A0);
//      delay(10);
//  readings[index] = analogRead(A0); 
//  // add the reading to the total:
//  total= total + readings[index];       
//  // advance to the next position in the array:  
//  index = index + 1;                    
//
//  // if we're at the end of the array...
//  if (index >= numReadings)              
//    // ...wrap around to the beginning: 
//    index = 0;                           
//
//  // calculate the average:
//  average = total / numReadings;         
//  
//      display.clearDisplay();
//    display.setCursor(0, 0);
//    float voltage = average * (2.495 / 1024.0);   //convert the value to a true voltage.
//    display.setTextSize(4);
//    display.println((voltage * 2));
//    
//    if (checkCharge() == 0) {
//        display.setTextSize(2);
//        display.println("Unplugged");
//        if (last == 1) {
//            digitalWrite(ON, LOW);
//            delay(10);
//            digitalWrite(OFF, HIGH);
//        delay(10);
//        digitalWrite(OFF, LOW);
//        }
//    }
//    else if(checkCharge() == 1) {
//        display.setTextSize(2);
//        display.println("Charging");
//        last = 1;
//        digitalWrite(ON, HIGH);
//    }
//    else {
//        display.setTextSize(2);
//        display.println("Charged");
//        last = 1;
//        digitalWrite(ON, HIGH);
//    }
//    
//    analogRead(TMP);
//    delay(10);
//    long temp = analogRead(TMP);
//    display.setTextSize(1);
//    display.print(((temp * (2495.0 / 1024.0)) - 500) / 10.0);
//    
//    display.print(" ");
//    display.print(hourFormat12());
//    display.print(minute());
//    display.print(second());
//    display.print(" ");
//    display.print(rtcStatus);
//    display.print(" ");
//    display.println(mode);
//    
//    if(digitalRead(BL)) {
//        display.print("1");
//        digitalWrite(OFF, HIGH);
//        delay(10);
//        digitalWrite(OFF, LOW);
//    }
//    if(digitalRead(BC)) {
//        display.print("2");
//    }
//    if(digitalRead(BR)) {
//        display.print("3");
//        digitalWrite(ON, HIGH);
//        delay(10);
//        digitalWrite(ON, LOW);
//    }
//    display.display();
//    delay(100);
    
    //drawBtnBar();
    inout.ioMon();
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(1);
    display.println(inout.getBatt());
    display.println(inout.getTmp());
    display.println(inout.btnMon());
    
    
    display.println(RTC.get());
    display.println(now());
    display.println(time.getDate());
    //display.println(year());
    display.println(time.getTime());
    if(inout.monitorBatt() == 1) {
        display.setTextSize(1);
        display.println("low batt");
    }
    else if(inout.monitorBatt() == 2) {
        display.setTextSize(1);
        display.println("batt depleted");
        display.display();
        delay(2000);
        inout.turnOff();
    }
    display.display();

    
}

void drawBtnBar() {
    display.drawFastHLine(0, 55, display.width(), WHITE);
    display.setTextSize(1);
    
    //left
    display.fillTriangle(8, 56, 16, 56, 8, 64, WHITE);
    display.fillRect(0, 56, 8, 8, WHITE);
    
    display.setTextColor(BLACK);
    display.setCursor(2, 56);
    display.print("<");
    display.setCursor(3, 56);
    display.print("<");
    
    //center
    display.setTextColor(WHITE);
    display.setCursor(52, 56);
    display.print("Menu");
    
    //right
    display.fillTriangle(112, 56, 120, 56, 120, 64, WHITE);
    display.fillRect(120, 56, 8, 8, WHITE);
    
    display.setTextColor(BLACK);
    display.setCursor(121, 56);
    display.print(">");
    display.setCursor(120, 56);
    display.print(">");
    display.display();
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