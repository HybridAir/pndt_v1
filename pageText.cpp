//handles the text scrolling page

#include "pageText.h"

extern settings set;
extern oled disp;
extern Adafruit_SSD1306 display;

pageText::pageText() {
    resetVars();
}

void pageText::resetVars() {
    position = 130;
    if(month() == 12 && day() == 25) {
        message = 1;
        out = "Merry Christmas Kim!";
    }
    else if(month() == 8 && day() == 25) {
        message = 2;
        out = "Happy Birthday Kim!";
    }
    else if(month() == 6 && day() == 1) {
        message = 3;
        out = "Happy Anniversary Kim!";
    }
    else {
        message = 0;
        out = "Blake loves Kim";
    }
}

void pageText::showPage(bool newPage) {
    if(newPage) {
        resetVars();
    }
    scrollText();
}

String pageText::getTitle() {
    return "Message";
}

void pageText::scrollText() {
    disp.drawBtnBar('<', "Menu", '>');
    display.setTextColor(WHITE);
    //display.setCursor(0, 20);
    //display.setTextSize(1);
  //display.print(((42 * out.length()) + SSD1306_LCDWIDTH) * -1);
    
    
    display.setTextSize(7);
      position = position - 1;
  display.setCursor(position, 0);
  display.print(out);
  //if(month() == 12 && day() == 25) {
  //    display.println("Merry Christmas Kim!");
  //}
  display.display();
  int offset = ((42 * out.length()) + SSD1306_LCDWIDTH) * -1;
  if(position < offset) {
    position = SSD1306_LCDWIDTH;
  }
  
//    if(position < (42 * out.length()) + SSD1306_LCDWIDTH) {
//    position = SSD1306_LCDWIDTH;
//  }
  
}