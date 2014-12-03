//push f9 to compile, right click makefile and make to upload
//reset the device on the 3rd f6 compile status line
//TODO: let the user know the device cannot be turned off when plugged in
#include <Arduino.h>
#include <Wire.h>           //might need to move you to rtctime.h

#include "io.h"
#include "rtcTime.h"
#include "oled.h"



void setup();
void loop();
int checkCharge();
void doBtn();
byte mode;

settings set;    
io inout; 
rtcTime time;                                                                     //new instance of IO
Adafruit_SSD1306 display(OLED_DC, OLED_RESET, OLED_CS);
oled disp;


void setup() {
    disp.begin();
    time.begin();   
}

void loop() {
    inout.ioMon();
    disp.dispMon(); 
    doBtn();
}

void doBtn() {
    byte btnMode = 0;       //which mode the buttons will be used for, 0 is for pages, 1 for settings, idk etc
    byte btn = inout.btnMon();
    switch(btnMode) {
        case 0:
            if(btn == 1) {       //left button was pressed
                set.prevPage();
            }
            else if(btn == 4) {      //right button
                set.nextPage();
            }
            break;
    }
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