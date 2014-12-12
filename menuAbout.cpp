//handles the about screen

#include "menuAbout.h"

extern Adafruit_SSD1306 display;
extern oled disp;
extern settings set;
extern menus menu;
extern io inout;


menuAbout::menuAbout() {
    title = "About";

}

void menuAbout::doMenu(byte btn) {
    display.setTextColor(WHITE);
    display.setTextSize(2);
    display.setCursor(0, 0);
    display.print("pndt_v1");
    display.setTextWrap(true);
    display.setTextSize(1);
    display.setCursor(0, 21);
    display.print("Designed and built by");
    display.println("Blake Stratton for");
    display.println("Kimberly Vandahl.");
    display.println("hw_v1 fw_v1.0");
    display.setTextWrap(false);
       
    disp.drawBtnBar('-', "Return", '-');
    
    if(btn == 2) {
        menu.setMenu();
    }
}

String menuAbout::getTitle() {
    return title;
}