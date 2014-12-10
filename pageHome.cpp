//handles the home info page
//TODO: fuigure out how to initilize struct vars

#include "pageHome.h"

extern settings set;
extern oled disp;
extern io inout;
extern Adafruit_SSD1306 display;

struct Scroller               //used for animating the time
{
    public:
        byte currentD0;
        byte currentD1;
        byte position;
        byte offset;
        
        void scrollTime(byte x, byte y, byte size, byte input) {      //scrolls one section of the time, accepts 1 to 2 digits
            offset = FONTHEIGHT*size;
            byte digit0 = input / 10;                                           //the digit in the tens place
            byte digit1 = input - (digit0 * 10);                                //the digit in the ones place    
            
            if(currentD0 != digit0 || currentD1 != digit1 ) {                   //if there are any new digits
               //figure out which one needs scrolling and which one doesn't
                if(currentD0 != digit0) {                                       //if it's digit0
                     scrollDigit(x, y, size, currentD0, digit0);                //go and scroll it
                }
                else {                                                          //if it's the same, just print it in the default position
                    display.setTextSize(size);
                    display.setCursor(x, y);
                    display.print(currentD0);
                }

                if(currentD1 != digit1) {                                       //if it's digit1                           
                     scrollDigit(x + (FONTWIDTH * size), y, size, currentD1, digit1);
                }
                else {
                    display.setTextSize(size);
                    display.setCursor(x + (FONTWIDTH * size), y);
                    display.print(currentD1);
                }

                position++;                                                     //increase the scroll position, since there is going to be one scrolling in here

                if(position > offset + y) {                                     //if we are done moving/scrolling the digit
                    position = 0 + y;                                           //reset the y position
                    //reset the current digit
                    if(currentD0 != digit0) {
                        currentD0 = digit0;
                    }
                    if(currentD1 != digit1) {
                        currentD1 = digit1;
                    }
                }
            }    
            else {                                                              //if the digits are the same
                //just keep them in their places
                display.setTextSize(size);
                display.setTextColor(WHITE);
                display.setCursor(x, y);
                display.print(currentD0);  
                display.setCursor(x + (FONTWIDTH * size), y);
                display.print(currentD1);                                       
            }       
        }
   
        void scrollDigit(byte x, byte y, byte size, byte currentD, byte newD) { //scrolls a single digit
            //get formatting out of the way
            display.setTextColor(WHITE);
            display.setTextSize(size);

            display.setCursor(x, position);                                     //set the cursor for the previous/old/current number
            display.print(currentD);                                            //print the current number in its new position (down a bit for the animation)
            display.fillRect(x, y + offset, FONTWIDTH*size, offset, BLACK);     //draw a black rectangle mask ON TOP of it to hide the old number as it moves down
            display.setCursor(x, position - (offset + 1));                      //prepare the cursor to bring the new number down
            display.print(newD);                                                //draw the new number above the previous one, it will be moved down in due time
            display.fillRect(x, y - (offset + 1), FONTWIDTH*size, offset, BLACK);     //draw a black rectangle mask ON TOP of it to hide the new number as it moves down
        }
       
} clockHour, clockMin, clockSec;


pageHome::pageHome() {
    previousMillis = 0;
    x = 0;
    y = 5;
    currentData = 0;
    setNewData = false;
    newData = 1;
    dataPos = 0;
    scrollingOut = true;
}

void pageHome::showPage(bool newPage) {
    
    //if(new)
    //get time and update numbers before showing anything
    
    //temporary timer for emulating system time
    bool increment = false;
    unsigned long currentMillis = millis();                                 //get the current time
            if(currentMillis - previousMillis > 5000) {                         //check if we have waited long enough
                previousMillis = currentMillis;                                     //save the current time as the previous amount
                increment = true;                                                       //let a battery bar blink
                setNewData = true;
            }
    
    if(increment) {
        x++;
        if(x > 59) {
            x = 0;
        }
        increment = false;
    }
    
    
    display.setTextColor(WHITE);
    display.setTextSize(4);       
    display.setCursor(41, 0);
    display.print(":");
    
    clockHour.scrollTime(0, 0, 4, x);           //hour
    clockMin.scrollTime(58, 0, 4, x);          //min
    clockSec.scrollTime(106, 14, 2, x);          //sec
    
    display.setCursor(106, 2);
    display.setTextSize(1);
    display.print("PM");

    scrollData(0, 37);

    display.drawFastHLine(0, 33, 128, WHITE); 
    disp.drawBtnBar('<', "Menu", '>');   
}

String pageHome::getTitle() {
    return "Home";
}

void pageHome::scrollData(byte x, byte y) {
display.setTextWrap(false);
    
    if(setNewData) {
        setNewData = false;
        newData++;
       if(newData > MAXDATA) {
            newData = 0;
        }
    }
    
    if(currentData != newData) {
        //scroll the new data in
        dataPos++; 
        
        if(scrollingOut) {
            drawData(x, y, currentData);
            display.fillRect(x, y, 128, dataPos, BLACK);     //draw a black rectangle mask ON TOP of it to hide the old number as it moves down
        }
        else {
            drawData(x, y, newData);
            display.fillRect(x, y + dataPos, 128, DATAHEIGHT - dataPos, BLACK);     //draw a black rectangle mask ON TOP of it to hide the old number as it moves down
        }

        if(dataPos > DATAHEIGHT) {                                     //if we are done moving/scrolling the digit
            dataPos = 0;                                           //reset the y position
            //currentData = newData;
            if(scrollingOut) {
                scrollingOut = false;
            }
            else {
                scrollingOut = true;
                currentData = newData;
            }
        }
    }
    else {
        //keep it where it is but let it do stuff
        drawData(x, y, currentData);
    }
}

void pageHome::drawData(byte x, byte y, byte data) {            //used to draw a specific data
    display.setTextColor(WHITE);
    display.setTextSize(2);
    display.setCursor(x, y);
    String date;
    
    
    switch(data) {
        case 0:             //date
            date.concat(month());
            date.concat('.');
            date.concat(day());
            date.concat('.');
            date.concat(year());
            display.print(date);
            break;
        case 1:             //temp/batt
            display.print((int)inout.getTmp());
            display.print((char)247);
            display.print("C");
            display.drawRect(x + 96, y, 29, 14, WHITE);
            display.drawRect(x + 97, y + 1, 27, 12, WHITE);
            display.fillRect(x + 125, y + 3, 3, 8, WHITE);
            
            //display.fillRect(x + 99, y + 3, 4, 10, WHITE);
            //display.fillRect(x + 99 + 11, y + 3, 4, 10, WHITE);
            //display.fillRect(x + 99 + 22, y + 3, 4, 10, WHITE);

            break;
        case 2:             //motd?
            display.println("Hello World");
            break;                
    }
}