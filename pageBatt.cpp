//handles the battery page

#include "pageBatt.h"

extern settings set;
extern oled disp;
extern io inout;
extern Adafruit_SSD1306 display;

pageBatt::pageBatt() {
    justReset = true;
    resetVars();
}

void pageBatt::showPage(bool newPage) {
    if(newPage) {
        resetVars();
    }
    charge();
}

void pageBatt::resetVars() {
    previousMillis = 0;                                                         //stores the last time the battery average was taken
    barActive = false;
}

String pageBatt::getTitle() {
    return "Battery";
}

void pageBatt::powerCheck() {                                                   //used to check for any power warnings, and display associated warning messages
    if(justReset) {                                                             //if the device was just reset
        justReset = false;                                                      //set this false so we don't end up here again
            
        if(inout.monitorBatt() == 1) {                                          //if the battery is low
            battWarn(false);                                                    //show the low warning
            display.display();
            delay(WARNDELAY);                                                   //wait for WARNDELAY
        }
        else if(inout.monitorBatt() == 2) {                                     //if the battery is dead
            battWarn(true);                                                     //display the warning            
            delay(WARNDELAY);                                                   //wait for WARNDELAY
            inout.turnOff();                                                    //force the device off since it needs to charge
        }
        else if(inout.getCharge() == 1) {                                         //if the device was just turned on because it was plugged into usb
            set.setPage(2);
        }
    }   
    else if(inout.monitorBatt() == 2) {                                         //if the device was operating normally and the battery died
        display.clearDisplay();
        battWarn(true);                                                         //display the warning            
        delay(WARNDELAY);                                                       //wait for WARNDELAY
        inout.turnOff();                                                        //force the device off since it needs to charge
    }
}
    
void pageBatt::battWarn(bool dead) {
    drawBattery(0, 6, dead);
    byte height = 12;
    display.setCursor(37, height);
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.println("My battery is");
    //display.setCursor(37, height + 8);
    if(dead) {
        display.print("depleted.");
    }
    else {
        display.print("low.");
    }
    display.setCursor(37, height + 24);
    display.println("Please connect");
    //display.setCursor(37, height + 32);
    if(dead) {
        display.print("me to USB.");
    }
    else {
        display.print("me to USB soon.");
    }
    display.display();
}

void pageBatt::charge() {
    disp.drawBtnBar('<', "Menu", '>');
    display.setCursor(37, 0);
    display.setTextColor(WHITE);
    display.setTextSize(2);
    display.println("Battery");
    display.setTextSize(1);
    display.setCursor(37, 16);
    
    switch(inout.getCharge()) {
        case 0:
            display.println("Unplugged");
            break;
        case 1:
            display.println("Charging");
            break;
        case 2:
            display.println("Charged");
            break;
    }
    
    display.drawFastHLine(37, 25, 90, WHITE);
    
    display.setTextSize(3);
    display.setCursor(37, 28);
    display.print(inout.getBattPercent());
    display.print("%");
    drawBattery(0, 0, false);
    

}

void pageBatt::drawBattery(byte x, byte y, bool dead) {                                       //draws the battery outline, needs a position
    
    //primary outline
    display.drawFastVLine(x, y + 5, 35, WHITE);                                 //left line                         
    display.drawLine(x, y + 40, x + 8, y + 48, WHITE);                          //angle
    display.drawFastHLine(x + 8, y + 48, 24, WHITE);                            //bottom line
    display.drawFastVLine(x + 32, y + 5, 44, WHITE);                            //right line
    display.drawFastHLine(x, y + 5, 32, WHITE);                                 //top line
    
    //inner outline to make it thicker overall
    display.drawFastVLine(x + 1, y + 6, 34, WHITE);                             //left line     
    display.drawLine(x + 1, y + 40, x + 8, y + 47, WHITE);                      //angle 1
    display.drawLine(x + 1, y + 39, x + 9, y + 47, WHITE);                      //angle 2
    display.drawFastHLine(x + 8, y + 47, 24, WHITE);                            //bottom line
    display.drawFastVLine(x + 31, y + 5, 44, WHITE);                            //right line
    display.drawFastHLine(x, y + 6, 32, WHITE);                                 //top line
    
    display.fillRect(x + 8, y, 17, 6, WHITE);                                   //top cap thing   
    
    if(dead) {                                                                  //if the battery is dead
        //show a sad face in the battery instead of any bars
        display.setCursor(4, 26);
        display.setTextColor(WHITE);
        display.setTextSize(2);
        display.println(":(");
    }
    else {
        if(inout.getCharge() == 1) {                                                //check if the device is charging    
            //the following is used to blink the highest possible battery bar while charging
            bool blink = false;                                                     //get the timer variable going
            unsigned long currentMillis = millis();                                 //get the current time
            if(currentMillis - previousMillis > BARDELAY) {                         //check if we have waited long enough
                previousMillis = currentMillis;                                     //save the current time as the previous amount
                blink = true;                                                       //let a battery bar blink
            }

            if(blink) {                                                             //if it's time to blink a battery bar
                if(barActive) {                                                     //if the bar is lit
                    barActive = false;                                              //turn the bar off
                }
                else {                                                              //if the bar is not lit
                    barActive = true;                                               //turn the bar on
                }
            }

            if(inout.getBattBar() == 0) {                                      //if the battery is at least 2/3 full      
                if(barActive) {                                                     //display the top battery bar if it's time to
                    drawBattBar(x, y, 1);                                           
                }
                //draw the bottom two battery bars normally
                drawBattBar(x, y, 2);
                drawBattBar(x, y, 3);
            }
            else if (inout.getBattBar() == 1) {                                //if the battery is at least 1/3 full   
                if(barActive) {                                                     //display the middle battery bar if it's time to
                    drawBattBar(x, y, 2);     //blink it
                }
                drawBattBar(x, y, 3);                                               //draw the last battery bar normally
            }
            else {                                                                  //if the battery is not dead yet
                if(barActive) {                                                     //display the last battery bar if it's time to
                    drawBattBar(x, y, 3);
                }
            }
        }   
        else {                                                                      //if the device is not charging
            if(inout.getBattBar() == 0) {                                      //if the battery is at least 2/3 full
                //show all bars normally
                drawBattBar(x, y, 1);
                drawBattBar(x, y, 2);
                drawBattBar(x, y, 3);
            }
            else if (inout.getBattBar() == 1) {                                //if the battery is at least 2/3 full
                drawBattBar(x, y, 2);
                drawBattBar(x, y, 3);
            }
            else {                                                                  //if the battery is not dead yet
                drawBattBar(x, y, 3);
            }
        }   
    }
}

void pageBatt::drawBattBar(byte x, byte y, byte bar) {                             //used to draw a bar in the battery, needs the outline's position
    switch(bar) {
        case 1:
            display.fillRect(x + 4, y + 9, 25, 11, WHITE);
            break;
        case 2:
            display.fillRect(x + 4, y + 22, 25, 11, WHITE);
            break;
        case 3:
            display.fillRect(x + 10, y + 35, 19, 10, WHITE); 
            display.fillRect(x + 4, y + 35, 7, 4, WHITE);
            display.fillTriangle(x + 4, y + 37, x + 9, y + 43, x + 9, y + 37, WHITE);
            break;
    } 
}