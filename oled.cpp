//handles the main hardware display related operations

#include "oled.h"

extern settings set;
extern menus menu;
extern io inout;
//extern rtcTime time;

extern Adafruit_SSD1306 display;
extern pages page;
extern pageBatt batt;

//function that is called all the time in main to update the display
//"page" functionality
//page/mode selection menu
//scrolling text announcement page, home screen clock data thing, battery/charge, settings, games, power warnings
//Adafruit_SSD1306 display(OLED_DC, OLED_RESET, OLED_CS);


oled::oled() {   
  display.setTextSize(4);
  display.setTextColor(WHITE);
  display.setTextWrap(false); 
}

void oled::begin() {
    display.begin(SSD1306_SWITCHCAPVCC);                                        //turn the display on
    display.display();                                                          //show the splash screen

    //prepare the startup timer
    unsigned long previousMillis = millis();                                    
    unsigned long currentMillis = 0;
    while(currentMillis < previousMillis + 2000) {                              //while we're waiting for 1 second to elapse
        currentMillis = millis();                                               //keep checking the time
        //stabilize the temperature and battery levels so they're correct
        inout.processTmp(true);
        inout.processBatt(true);
        //also give the user some time to view the splash screen
    }

    display.clearDisplay();
    batt.powerCheck();                                                          //need to check the battery and display any priority warnings
}

void oled::dispMon() {
    //there's going to be stuff in here okay
    //first line is always going to clear the display
    //next lines are going to go through each page to see what needs displaying
    //each page will add it's stuff if necessary
    //after that there will be some other "functions" that might need displaying
    //includes the button bar and probably battery warnings
    //last line is a display.display
    display.clearDisplay();
    batt.powerCheck();
    if(set.getMenu()) {         //check if the menu needs to be displayed
        menu.doMain();      //do the main menu page
    }
    else {
        page.doPage();          //the menu isn't open, resume normal operation
    }
    
    display.display();
}

void oled::drawBtnBar(char left, String center, char right) {
    display.drawFastHLine(0, 55, display.width(), WHITE);                       //draw the separator line
    display.setTextSize(1);
    
    //left
    display.fillTriangle(8, 56, 16, 56, 8, 64, WHITE);                          //left side designy stuff
    display.fillRect(0, 56, 8, 8, WHITE);
    
    display.setTextColor(BLACK);                                                //print out the left indicator character, and "bold" it
    display.setCursor(2, 56);
    display.print(left);
    display.setCursor(3, 56);
    display.print(left);
    
    //center
    display.setTextColor(WHITE);  
//    int centerLength = center.length();                                         //get the length of the center text
//    if((centerLength % 2) == 0) {                                               //if the length of the text is even
//        int spaces = USABLECHARS - centerLength;                                //subtract the text length from the number of usable characters to get the number of empty spaces
//        int halfSpaces = spaces / 2;                                            //divide the above by two to get the number of empty spaces on each side of the text
//        int halfChars = halfSpaces * 6;                                         //multiply the above by 6 to get the number of empty pixels on each side
//        display.setCursor(CHARMARGIN + halfChars + 3, 56);                      //set the cursor position, add 3 pixels to make the text center
//    }
//    else {                                                                      //if the length of the text is odd
//        int halfText = centerLength / 2;                                        //get the number of characters on each side of the middle character (since it's odd)
//        int spaces = USABLECHARS - (halfText * 2);                              //continue like it was even
//        int halfSpaces = spaces / 2;
//        int halfChars = halfSpaces * 6;
//        display.setCursor(CHARMARGIN + halfChars, 56);                          //don't need to add the 3 pixels since it should already be center
//    }
    centerText(56, 1, center);
    //display.print(center);                                                      //print the center text
    
    //right
    display.fillTriangle(112, 56, 120, 56, 120, 64, WHITE);                     //right side designy stuff
    display.fillRect(120, 56, 8, 8, WHITE);
    
    display.setTextColor(BLACK);                                                //print out the right indicator character, and "bold" it
    display.setCursor(121, 56);
    display.print(right);
    display.setCursor(120, 56);
    display.print(right);
}

void oled::centerText(byte y, byte size, String in) {
    byte textSize = (in.length() * 6) * size;       //24px 4 chars
    //byte out = (SSD1306_LCDWIDTH - textSize)/2;
    display.setTextSize(size);
    display.setCursor((SSD1306_LCDWIDTH - textSize)/2, y);
    display.print(in);
}
