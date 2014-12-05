//handles overall page control

//TODO: add automatic charging page, reset page stuff when they get switched to (scrolling position, etc)

#include "pages.h"

extern settings set;
extern oled disp;
extern io inout;
extern rtcTime time;
extern Adafruit_SSD1306 display;

pageBatt batt;
pageText text;

pages::pages() {
   // x = 130;
    previousMillis2 = 0;                                                         //stores the last time the battery average was taken
    prevPage = set.getPage();                                                //set the first current page, it will be the default in settings
    showPage = true;                                                            //don't need to show the title page upon power up
}

void pages::doPage() {                                                          //***checks with page needs to be displayed, and prepares to display it
    byte currentPage = set.getPage();                                           //get the current page   
    if(currentPage != prevPage) {                                               //if the page has changed since it was last checked      
        prevPage = currentPage;                                                 //save the new page       
        previousMillis2 = millis();                                             //reset the title timer
        showPage = false;                                                       //need to show the title now
    }
    if(!showPage) {                                                             //if the title needs to be shown
        doTitle(currentPage);                                                   //show the title for the current page   
        
        //check the title delay timer
        unsigned long currentMillis = millis();                                 //get the current time
        if(currentMillis - previousMillis2 > TITLEDELAY) {                      //if we have looked at the title long enough
            showPage = true;                                                    //the page can be shown now
        }
    }
    else {                                                                      //the page can be shown
        switch(currentPage) {                                                   //show the specific page
            case 0:
                //scrollText();
                text.showPage();
                break;
            case 1:
                debug();
                break;
            case 2:
                //charge();
                batt.showPage();
                break;
        }
    }
}

void pages::doTitle(byte page) {            //used to display a page's title before displaying the page
    disp.drawBtnBar('<', "Menu", '>');
    display.setTextColor(WHITE);
    display.setTextSize(2);
    display.setCursor(0, 0);
    
    //the following switch case will be replaced with the page giving it its title or whatever
    switch(page) {               //show the page
        case 0:
            display.println("0 scrolltext");
            break;
        case 1:
            display.println("1 debug");
            break;
        case 2:
            display.println("2 charge");
            break;
    }
}

//void pages::scrollText() {
//    disp.drawBtnBar('<', "Menu", '>');
//    display.setTextColor(WHITE);
//    display.setTextSize(7);
//      x = x - 1;
//  display.setCursor(x, 0);
//  display.println("Hello, world.");
//  display.display();
//  if(x < -650) {
//    x = 130;
//  }
//  
//}

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