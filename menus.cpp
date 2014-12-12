//handles main menu related stuff

#include "menus.h"

extern Adafruit_SSD1306 display;
extern oled disp;
extern settings set;

menuTime mTime;

menus::menus() {
    activeItem = 0;
    showMenu = false;

}

void menus::doMain() {
    if(showMenu) {
        doItem();
    }
    else {
        disp.drawBtnBar('^', "Select", 'v');
        disp.showTitle("Main Menu");
        drawList();
    }
    display.display();
}

void menus::drawList() {            //used to draw the list of items, and highlight the currently selected one
    display.setCursor(0, 10);
    drawListItem(mTime.getTitle(), 0);
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
            mTime.doMenu(currentBtn);
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
    currentBtn = btn;
    if(!showMenu) {
        if(btn == 1) {       //left button was pressed
            prevItem();
        }
        else if(btn == 2) {     //center button
            showMenu = true;
        }
        else if(btn == 4) {      //right button
            nextItem();
        }
        currentBtn = 0;
    }
}

void menus::setMenu() {             //used to toggle whether the menu is displayed
    if(showMenu == true) {
        showMenu = false;
    }
    else {
        showMenu = true;
    }
}