//push f9 to compile, right click makefile and make to upload
//reset the device on the 3rd f6 compile status line
//TODO: let the user know the device cannot be turned off when plugged in
#include <Arduino.h>
#include <Wire.h>
#include <EEPROM.h>

#include "io.h"
#include "rtcTime.h"
#include "oled.h"



void setup();
void loop();
void drawBtnBar();
int checkCharge();
String rtcStatus;
byte mode;

    
    io inout; 
    rtcTime time;
                                                                      //new instance of IO
    Adafruit_SSD1306 display(OLED_DC, OLED_RESET, OLED_CS);
    oled disp;


void setup() {
    disp.begin();
    time.begin();
 
  
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

int last = 0;



void loop() {
    
    //drawBtnBar();
    inout.ioMon();
    disp.dispMon();

    
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