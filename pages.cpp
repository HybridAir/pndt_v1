//handles all of the pages that can be displayed (does not include potential games maybe)

//TODO: add automatic charging page, temporarily flash the title text for each page

#include "pages.h"

extern settings set;
extern oled disp;
extern io inout;
extern rtcTime time;
extern Adafruit_SSD1306 display;

pages::pages() {
    x = 130;
    previousMillis = 0;                                                         //stores the last time the battery average was taken
    barActive = false;
}

void pages::doPage() {                                                          //***checks with page needs to be displayed, and prepares to display it
    switch(set.getPage()) {
        case 0:
            scrollText();
            break;
        case 1:
            debug();
            break;
        case 2:
            charge();
            break;
    }
}

void pages::scrollText() {
    disp.drawBtnBar('<', "Menu", '>');
    display.setTextColor(WHITE);
    display.setTextSize(7);
      x = x - 1;
  display.setCursor(x, 0);
  display.println("Hello, world.");
  display.display();
  if(x < -650) {
    x = 130;
  }
  
}

void pages::debug() {
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
    display.println(set.getrom());
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
}

void pages::charge() {
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
    //drawBattery(20, 23);
    drawBattery(0, 0);
    

}

void pages::drawBattery(byte x, byte y) {                                       //draws the battery outline, needs a position
    
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
        
        if(inout.getBattPercent() >= 66) {                                      //if the battery is at least 2/3 full      
            if(barActive) {                                                     //display the top battery bar if it's time to
                drawBattBar(x, y, 1);                                           
            }
            //draw the bottom two battery bars normally
            drawBattBar(x, y, 2);
            drawBattBar(x, y, 3);
        }
        else if (inout.getBattPercent() >= 33) {                                //if the battery is at least 1/3 full   
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
        if(inout.getBattPercent() >= 66) {                                      //if the battery is at least 2/3 full
            //show all bars normally
            drawBattBar(x, y, 1);
            drawBattBar(x, y, 2);
            drawBattBar(x, y, 3);
        }
        else if (inout.getBattPercent() >= 33) {                                //if the battery is at least 2/3 full
            drawBattBar(x, y, 2);
            drawBattBar(x, y, 3);
        }
        else {                                                                  //if the battery is not dead yet
            drawBattBar(x, y, 3);
        }
    }   
}

void pages::drawBattBar(byte x, byte y, byte bar) {                             //used to draw a bar in the battery, needs the outline's position
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

void pages::doBtn(byte btn) {
    if(btn == 1) {       //left button was pressed
        set.prevPage();
    }
    else if(btn == 2) {     //center button
        set.setMenu();
    }
    else if(btn == 4) {      //right button   
        set.nextPage();
    }
}