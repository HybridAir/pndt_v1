//handles the main hardware display related operations

#include "oled.h"


//extern io inout;
//extern rtcTime time;

extern Adafruit_SSD1306 display;

//function that is called all the time in main to update the display
//"page" functionality
//page/mode selection menu
//scrolling text announcement page, home screen clock data thing, battery/charge, settings, games, power warnings
//Adafruit_SSD1306 display(OLED_DC, OLED_RESET, OLED_CS);

pages page;


oled::oled() {   
  display.setTextSize(4);
  display.setTextColor(WHITE);
  display.setTextWrap(false);
  
  
}

void oled::begin() {
          display.begin(SSD1306_SWITCHCAPVCC);
  display.display();
  
  delay(1000);
  display.clearDisplay();   // clears the screen and buffer
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
    //page.debug();
    //drawBtnBar();
    page.doPage();
    
    display.display();
}

void oled::drawBtnBar() {
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
    //display.display();
}