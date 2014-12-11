//handles the time/date setting menu

#include "menuTime.h"

menuTime::menuTime() {
    //defaults 

}

void menuTime::doMenu() {
    disp.drawBtnBar('+', "OK", '-');
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.setTextSize(1);
    display.println("Set Time");
    display.display();
}