//handles the text scrolling page

#include "pageText.h"

extern settings set;
extern oled disp;
extern Adafruit_SSD1306 display;

pageText::pageText() {
    position = 130;
}

void pageText::showPage() {
    scrollText();
}

void pageText::scrollText() {
    disp.drawBtnBar('<', "Menu", '>');
    display.setTextColor(WHITE);
    display.setTextSize(7);
      position = position - 1;
  display.setCursor(position, 0);
  display.println("Hello, world.");
  display.display();
  if(position < -650) {
    position = 130;
  }
  
}