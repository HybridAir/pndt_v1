//handles the time/date setting menu
//todo: make the time selection a little cleaner with rectangles instead of highlight
//make the value setting into a struct or whatever

#include "menuTime.h"

extern Adafruit_SSD1306 display;
extern oled disp;
extern settings set;
extern menus menu;
extern io inout;

struct dataValue {
    public:
        byte currentVal;
        byte maxVal;
        byte minVal;
        
        void showVal(byte x, byte y, bool active) {
            if(active) {
                display.setTextColor(BLACK, WHITE);     //make this prettier later with a drawrect outline instead
            }
            else {
                display.setTextColor(WHITE);
            }
            display.setCursor(x, y);
            display.setTextSize(3);
    
            display.print(disp.extractD0(currentVal));
            display.print(disp.extractD1(currentVal));
        }
        
        void addVal(bool active) {
            if(active) {
                currentVal++;
                if(currentVal > maxVal) {
                    currentVal = minVal;
                }
            }
        }
        
        void subVal(bool active) {
            if(active) {
                currentVal--;
                if(currentVal < minVal || currentVal == 255) {
                    currentVal = maxVal;
                }
            }
        }
        
        int getVal() {
            return (int)currentVal;
        }
        
        void setVal(byte current, byte max, byte min) {
            currentVal = current;
            maxVal = max;
            minVal = min;
        }
        
} setHour, setMin, setMonth, setDay, setYear;


menuTime::menuTime() {
    title = "Set Time/Date";
    currentPage = 0;
    
    setHour.setVal(12, 23, 0);
    setMin.setVal(0, 59, 0);
    setMonth.setVal(1, 12, 1);
    setDay.setVal(1, 31, 1);
    setYear.setVal(0, 99, 0);
    currentBtn = 0;
    activeUnit = 0;

}

void menuTime::doMenu(byte btn) {
    currentBtn = btn;
    doBtn();
    
    switch(currentPage) {
        case 0:
            doTime();
            break;
        case 1:
            doDate();
            break;
        case 2:
            currentPage = 0;
            activeUnit = 0;
            finishTime();
            menu.setMenu();
            break;
    }
       
    disp.drawBtnBar('-', "Next", '+');
}

void menuTime::doBtn() {
    if(currentBtn == 1) {
        setHour.subVal(activeUnit == 0);
        setMin.subVal(activeUnit == 1);
        setMonth.subVal(activeUnit == 2);
        setDay.subVal(activeUnit == 3);
        setYear.subVal(activeUnit == 4);
    }
    else if(currentBtn == 2) {
        activeUnit++;
        if(activeUnit > MAXUNITS || activeUnit == 2) {
            currentPage++;
        }
    }
    else if(currentBtn == 4) {
        setHour.addVal(activeUnit == 0);
        setMin.addVal(activeUnit == 1);
        setMonth.addVal(activeUnit == 2);
        setDay.addVal(activeUnit == 3);
        setYear.addVal(activeUnit == 4);
    }
    currentBtn = 0;
}

String menuTime::getTitle() {
    return title;
}

void menuTime::doTime() {
    disp.showTitle("Set Time");
    
    
    setHour.showVal(22, 20, activeUnit == 0);
    setMin.showVal(69, 20, activeUnit == 1);        //83
    
    display.setTextColor(WHITE);
    display.setTextSize(3);
    display.setCursor(54, 20);
    display.print(':');
       
}

void menuTime::doDate() {
    disp.showTitle("Set Date");
    
    setMonth.showVal(0, 14, activeUnit == 2);
    setDay.showVal(47, 14, activeUnit == 3);
    setYear.showVal(94, 14, activeUnit == 4);
    
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.setCursor(2, 40);
    display.print("Month");
    display.setCursor(55, 40);
    display.print("Day");
    display.setCursor(99, 40);
    display.print("Year");
    display.setTextSize(3);
    display.setCursor(31, 14);
    display.print('.');
    display.setCursor(77, 14);
    display.print('.');
}

void menuTime::finishTime() {
    setTime(setHour.getVal(), setMin.getVal(), 0, setDay.getVal(), setMonth.getVal(), setYear.getVal() + 2000);
    RTC.set(now());
    setSyncProvider(RTC.get);
}