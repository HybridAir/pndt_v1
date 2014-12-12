//handles the home info page
//TODO: fuigure out how to initilize struct vars?
//get am pm going

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
        
        void updateVars(byte currentTime) {                                     //updates the time to the latest
            currentD0 = disp.extractD0(currentTime);
            currentD1 = disp.extractD1(currentTime);
        }
        
        void scrollTime(byte x, byte y, byte size, byte input) {                //scrolls one section of the time, accepts 1 to 2 digits
            offset = FONTHEIGHT*size;            
            byte digit0 = disp.extractD0(input);                                     //the digit in the tens place
            byte digit1 = disp.extractD1(input);                                     //the digit in the ones place 
            
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
    updateVars();
}

void pageHome::updateVars() {
    x = 0;
    y = 5;
    currentData = 0;
    setNewData = false;
    newData = 1;
    dataPos = 0;
    scrollingOut = true;
    dataDelay = 0;
    prevSec = second();
    clockHour.updateVars(hourFormat12());
    clockMin.updateVars(minute());
    clockSec.updateVars(second());
}

void pageHome::showPage(bool newPage) {
    
    if(newPage) {                                                               //if the page is new
        updateVars();                                                           //go update all the variables
    }  
    
    display.setTextColor(WHITE);
    display.setTextSize(4);       
    display.setCursor(41, 0);
    display.print(":");
    
    clockHour.scrollTime(0, 0, 4, hourFormat12());           //hour
    clockMin.scrollTime(58, 0, 4, minute());          //min
    clockSec.scrollTime(106, 14, 2, second());          //sec
    
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
    
    if(second() != prevSec) {                                                   //if it has been a second
        prevSec = second();                                                     //set this variable so the following code doesn't get spammed
        dataDelay++;                                                            //increase the delay counter
        if(dataDelay >= DATADELAY) {                                            //if it has been at least 5 seconds
            //change the currently displayed data to the next one
            dataDelay = 0;      
            newData++;
            if(newData > MAXDATA) {
                newData = 0;
            }
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
    byte emptyBars;
    char dateSeparator = '.';
    
    switch(data) {
        case 0:             //date
            display.print(month());
            display.print(dateSeparator);
            display.print(day());
            display.print(dateSeparator);
            display.print(year());
            break;
        case 1:             //temp/batt
            display.print((int)inout.getTmp());
            display.print((char)247);
            display.print('C');
            
            //draw the battery outline
            display.drawRect(x + 96, y, 29, 14, WHITE);
            display.drawRect(x + 97, y + 1, 27, 12, WHITE);
            display.fillRect(x + 125, y + 3, 3, 8, WHITE);
                       
            emptyBars = (inout.getBattBar() * 1);
            for(byte i = 0; i < (3 - emptyBars); i++) {
                drawBattBar(99, y + 3, i);
            }
            break;
        case 2:             //motd?
            display.println("Hello Worl");
            break;                
    }
}
    
    void pageHome::drawBattBar(byte x, byte y, byte bar) {
        byte xmult = bar * BARWIDTH;
        display.fillRect(x + xmult, y, 7, BARWIDTH, WHITE);
    }