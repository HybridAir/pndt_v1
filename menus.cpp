//handles all onscreen menu related stuff

#include "menus.h"

extern Adafruit_SSD1306 display;
extern oled disp;
extern settings set;

menuTime mTime;

menus::menus() {
    //defaults 
    activeItem = 0;

}

void menus::doMain() {
    disp.drawBtnBar('+', "Select", '-');
    drawList();
    display.display();
}

void menus::drawList() {            //used to draw the list of items, and highlight the currently selected one
    display.setCursor(0, 0);
    drawListItem("Set Time", 0);
    drawListItem("item 2", 1);
    drawListItem("item 3", 2);
    drawListItem("Return", 3);
}

void menus::drawListItem(String text, byte index) {            //draws a list item, needs it's position/index
    if(activeItem == index) {
        display.setTextColor(BLACK, WHITE);
    }
    else {
        display.setTextColor(WHITE);
    }
    display.setTextSize(1);
    display.println(text);
}

void menus::doItem() {
    switch(activeItem) {
        case 0:
            //menuTime.doMenu();
            break;
        case 1:
            break;
        case 2:
            break;
        case 3:
            set.setMenu();
            break;
    }
}

void menus::prevItem() {
    if(activeItem == 0) {           //rollover to the last item if we're already at the first item
        activeItem = NUMITEMS;
    }
    else {
        activeItem--;
    }
}

void menus::nextItem() {
    if(activeItem == NUMITEMS) {           //rollover the page number to 0 if we're already at the last page
        activeItem = 0;
    }
    else {
        activeItem++;
    }
}

void menus::doBtn(byte btn) {
            if(btn == 1) {       //left button was pressed
                prevItem();
            }
            else if(btn == 2) {     //center button
                //set.setMenu();
                doItem();
            }
            else if(btn == 4) {      //right button
                nextItem();
            }
}