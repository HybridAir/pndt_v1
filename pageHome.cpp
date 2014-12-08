//handles the home info page

#include "pageHome.h"

extern settings set;
extern oled disp;
extern io inout;
extern Adafruit_SSD1306 display;

struct Scroller
{
    public:
        byte prevDigit;
        byte position;
        bool timeToScroll;
        
        void pageHome::scrollTime(byte x, byte y, byte size, byte input) {        //scrolls one part of the time, accepts 1 to 2 digits
            byte digit0 = input / 10;
            byte digit1 = input - (digit0 * 10);
            
            scrollHour.scrollDigit(x, y, size, digit0);
            scrollMin.scrollDigit(x + (FONTWIDTH * size), y, size, digit1);    
            //this function will keep track of each previous digit, not the one xdirectly below
            

            
            
        }
        
        
        
        void scrollDigit(byte x, byte y, byte size, byte input) {       //scrolls a single digit
            //get formatting out of the way
            display.setTextColor(WHITE);
            display.setTextSize(size);
            byte offset = FONTHEIGHT*size;                                      //get the actual fontsize

            if(prevDigit != input) {                                             //if the input time is different than the last one, then assume that it's time to scroll
                position++;                                                     //increase the y position, it will move the numbers down, creating the animation
                display.setCursor(x, position);                                 //set the cursor for the previous/old/current number
                display.print(prevDigit);                                        //print the previous number in its new position (down a bit for the animation)
                display.fillRect(x, y + offset, FONTWIDTH*size, offset, BLACK); //draw a black rectangle mask ON TOP of it to hide the old number as it moves down
                display.setCursor(x, position - (offset + 1));                  //prepare the cursor to bring the new number down
                display.print(input);                                           //draw the new number above the previous one, it will be moved down in due time
                display.fillRect(x, y - (offset + 1), FONTWIDTH*size, offset, BLACK);     //draw a black rectangle mask ON TOP of it to hide the new number as it moves down

                if(position > offset + y) {                                     //if we are done moving/scrolling the number
                    position = 0 + y;                                           //reset the y position
                    prevDigit = input;                                           //set the previous time to the new number we were given
                    timeToScroll = false;                                       //don't need to scroll anything again until the next "pulse"
                }
            }    
            else {                                                              //if we dont need to scroll anything
                //just keep the old input number in the default place
                display.setTextColor(WHITE);
                display.setCursor(x, y);
                display.print(prevDigit);                                        //only show the previous time since we haven't scrolled to the new one yet
            }
        }
        
} scrollHour, scrollMin;


pageHome::pageHome() {
    scrollHour.position = 0;
    scrollHour.prevDigit = 0;
    previousMillis = 0;
    x = 0;
    y = 5;
    scrollHour.timeToScroll = true;
}

void pageHome::showPage(bool newPage) {
    
    //temporary timer for emulating system time
    bool increment = false;
    unsigned long currentMillis = millis();                                 //get the current time
            if(currentMillis - previousMillis > 1000) {                         //check if we have waited long enough
                previousMillis = currentMillis;                                     //save the current time as the previous amount
                increment = true;                                                       //let a battery bar blink
                scrollHour.timeToScroll = true;
            }
    
    //setHour(x);
    //scrollTime(0, 0, 4, x);
    scrollTime(0, 0, 4, x);
    //scrollMin.doit(40, 0, 4, y);
    
    if(increment) {
        x++;
        if(x > 59) {
            x = 0;
        }
        increment = false;
    }
    
    
    
    
    disp.drawBtnBar('<', "Menu", '>');
    //display.setCursor(0, 0);
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

void pageHome::doTime() {
    //keep spamming the number scrolling deal
        //scroll hour
        //scroll minute
        //scroll second
    //get the current time
    //give each part to the scroller
}



        void pageHome::scrollTime(byte x, byte y, byte size, byte input) {        //scrolls one part of the time, accepts 1 to 2 digits
            byte digit0 = input / 10;
            byte digit1 = input - (digit0 * 10);
            
            scrollHour.scrollDigit(x, y, size, digit0);
            scrollMin.scrollDigit(x + (FONTWIDTH * size), y, size, digit1);                 
        }


//void pageHome::scrollTime(byte x, byte y, byte size, byte input) {
//    //get formatting out of the way
//    display.setTextColor(WHITE);
//    display.setTextSize(size);
//    //position = position + y;
//    byte offset = FONTHEIGHT*size;
//    
//    if(prevHour != input) {                                                     //if the input hour is different than the last one, then assume that it's time to scroll
//        position++;                                                                //increase the y position, it will move the numbers down, creating the animation
//        display.setCursor(x, position);                                     //set the cursor for the previous/old/current number
//        display.print(prevHour);                                            //print the previous hour in its new position (down a bit since it's moving)
//        display.fillRect(x, y + offset, FONTWIDTH*size, offset, BLACK);     //draw a black rectangle mask ON TOP of it to hide the old number as it moves down
//        display.setCursor(x, position - (offset + 1));           //prepare the cursor to bring the new number down
//        display.print(input);                                               //draw the new hour above the previous one, it will be moved down in due time
//        display.fillRect(x, y - (offset + 1), FONTWIDTH*size, offset, BLACK);     //draw a black rectangle mask ON TOP of it to hide the old number as it moves down
//
//        if(position > offset + y) {                                           //if we are done moving/scrolling the number
//            position = 0 + y;                                                   //reset the y position
//            prevHour = input;
//            timeToScroll = false;       //don't need to scroll anything again until the next "pulse"
//        }
//    }    
//    else { //if we dont need to scroll anything
//        //just keep the old input in the default place
//        display.setTextColor(WHITE);
//        display.setCursor(x, y);
//        display.print(prevHour);        //only show the prevhour becuase we haven't scrolled to the new input yet
//    }
//    
//    
//    //it wants a position, text size, and a number
//    //it will first give it a leading 0 if necessary
//    //so it will handle two numbers at a time
//    //it needs to be able to know when to scroll only one number (greater than 10 idk 10-11-12, etc)
//    
//    //so everytime it gets a "pulse" from the system timer each second (it will ask the rtc for this)
//    //it will be allowed to advance/scroll the number if it has a new one and hasnt't scrolled it yet
//    //I guess that's it
//}






