//handles all of the pages that can be displayed (does not include potential games maybe)

#include "pages.h"

extern settings set;
extern io inout;
extern rtcTime time;
extern Adafruit_SSD1306 display;

pages::pages() {
    x = 130;
}

void pages::doPage() {                                                          //***checks with page needs to be displayed, and prepares to display it
    switch(set.getPage()) {
        case 0:
            scrollText();
            break;
        case 1:
            debug();
            break;
    }
}

void pages::scrollText() {
    drawBtnBar();
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
    //display.print(" ");
    //display.println(inout.btnMon());
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

void pages::drawBtnBar() {
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