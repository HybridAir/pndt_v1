
//handles the time/date setting menu
//todo: make the time selection a little cleaner with rectangles instead of highlight
//make the value setting into a struct or whatever

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
    currentBtn = btn;
       
    disp.drawBtnBar('-', "Next", '+');
}

String menuAbout::getTitle() {
    return title;
}