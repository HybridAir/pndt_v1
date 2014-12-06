//handles the home info page

#include "pageHome.h"

extern settings set;
extern oled disp;
extern io inout;
extern Adafruit_SSD1306 display;

pageHome::pageHome() {
position = 0;
prevHour = 0;
previousMillis = 0;
x = 0;
}

void pageHome::showPage(bool newPage) {
    
    bool increment = false;
    unsigned long currentMillis = millis();                                 //get the current time
            if(currentMillis - previousMillis > 1000) {                         //check if we have waited long enough
                previousMillis = currentMillis;                                     //save the current time as the previous amount
                increment = true;                                                       //let a battery bar blink
            }
    setHour(x);
    if(increment) {
        x++;
        if(x > 9) {
            x = 0;
        }
        increment = false;
    }
    
    
    
    
    disp.drawBtnBar('<', "Menu", '>');
    display.setCursor(0, 0);
    display.setTextColor(WHITE);
    display.setTextSize(4);
    //display.print("88");
    display.setCursor(41, 0);
    display.print(":");
    display.setCursor(58, 0);
    display.print("88");
    display.setCursor(106, 14);
    display.setTextSize(2);
    display.print("88");
    display.setCursor(106, 2);
    display.setTextSize(1);
    display.print("PM");
    
    display.drawFastHLine(0, 33, 128, WHITE); 
    
    display.setCursor(0, 37);
    display.setTextSize(2);
    display.print("01.01.15");
    
}

String pageHome::getTitle() {
    return "Home";
}

void pageHome::setHour(byte in) {           //used to set the hour on the display, has this cool animated thing
    //get text formatting set up
    display.setTextColor(WHITE);
    display.setTextSize(4);
    
    if(prevHour != in) {                    //if the input hour is different than the last one
        position++;                         //increase the position, it will animate the number down
        display.setCursor(0, position);
        display.print(prevHour);                    //print the previous hour
        display.fillRect(0, 32, 24, 32, BLACK);     //draw a black cutoff rectangle mask thing

        display.setCursor(0, position - 33);
        display.print(in);                          //draw the new hour above the previous one, it will be moved down
  //display.display();
  if(position > 32) {
    position = 0;
    prevHour = in;
  }
  delay(5);
    }
    else {
        display.setTextColor(WHITE);
    display.setTextSize(4);
        display.setCursor(0, 0);
  display.print(in);
    }
}

