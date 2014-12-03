//handles all onscreen menu related stuff

#include "menus.h"

extern Adafruit_SSD1306 display;

menus::menus() {
    //defaults 

}

void menus::doMain() {
    display.setTextColor(WHITE);
    display.setTextSize(2);
  display.setCursor(0, 0);
  display.println("Main Menu");
  display.display();
}