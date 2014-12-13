//handles overall page control

//TODO: auto holiday message

#include "pages.h"

extern settings set;
extern oled disp;
extern io inout;
extern rtcTime time;
extern Adafruit_SSD1306 display;

pageBatt batt;
pageText text;
pageHome home;

pages::pages() {
    prevPage = set.getPage();                                                   //gets the default first page from settings
    showPage = true;                                                            //don't need to show the title page upon power up
}

void pages::doPage() {                                                          //***manages title and page display
    byte currentPage = set.getPage();                                           //get the current page   
    if(currentPage != prevPage) {                                               //if the page has changed since it was last checked      
        prevPage = currentPage;                                                 //save the new page       
        prevTitleTime = millis();                                               //reset the title timer
        showPage = false;                                                       //need to show the title now
        newPage = true;
    }
    if(!showPage) {                                                             //if the title needs to be shown
        doTitle(currentPage);                                                   //show the current page's title  
        
        //check the title delay timer
        unsigned long currentTitleTime = millis();                              //save the current time
        if(currentTitleTime - prevTitleTime > TITLEDELAY) {                     //if we have looked at the title long enough
            showPage = true;                                                    //the page can be shown now
        }
    }
    else {                                                                      //the page can be shown
        switch(currentPage) {                                                   //show the specific page
            case 0:
                home.showPage(newPage);
                break;
            case 1:
                text.showPage(newPage);
                break;
//            case 2:
//                debug();
//                break;
            case 2:
                batt.showPage(newPage);
                break;
        }
        newPage = false;
    }
}

void pages::doTitle(byte page) {                                                //used to display a page's title before displaying the page
    disp.drawBtnBar('<', "Menu", '>');
    
    //get the text formatting set up
    display.setTextColor(WHITE);                                                
    //display.setTextSize(2);
    display.setCursor(0, 0);
    
    switch(page) {                                                              //show a page title
        case 0:
            disp.centerText(16, 2, home.getTitle());
            break;
        case 1:
            disp.centerText(16, 2, text.getTitle());
            break;
//        case 2:
//            display.println("debug");
//            break;
        case 2:
            disp.centerText(16, 2, batt.getTitle());
            break;
    }
}

//void pages::debug() {
//        display.setCursor(0, 0);
//    display.setTextSize(1);
//    display.println(inout.getBatt());
//    //display.println(inout.getTmp());
//    //display.println(inout.btnMon());   
//    display.println(RTC.get());
//    display.println(now());
//    //display.println(time.getDate());
//    //display.println(year());
//    //display.println(time.getTime());
//    display.println(set.getrom());
//    //if(inout.monitorBatt() == 1) {
//    //    display.setTextSize(1);
//    //    display.println("low batt");
//    //}
//    //else if(inout.monitorBatt() == 2) {
//    //    display.setTextSize(1);
//    //    display.println("batt depleted");
//    //    display.display();
//    //    delay(2000);
//    //    inout.turnOff();
//    //}
//}

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
