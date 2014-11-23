//handles all of the pages that can be displayed (does not include potential games maybe)

#include "pages.h"

extern io inout;
extern rtcTime time;
extern Adafruit_SSD1306 display;

pages::pages() {
    x = 130;
}

void pages::doPage() {                                                          //***checks with page needs to be displayed, and prepares to display it
    switch(pageNum) {
        case 0:
            scrollText();
            break;
        case 1:
            debug();
            break;
    }
}

void pages::scrollText() {
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