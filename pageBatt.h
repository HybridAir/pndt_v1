#ifndef PAGEBATT_H
#define	PAGEBATT_H

#include <Arduino.h>
#include "oled.h"

class pageBatt {
    public:
        pageBatt();
        void showPage(bool newPage);
        String getTitle();
        void doPage();
        void doBtn(byte btn);
    private:       
        void resetVars();
        void charge();
        void drawBattery(byte x, byte y);
        void drawBattBar(byte x, byte y, byte bar);
        
        unsigned long previousMillis; 
        bool barActive;
              
};

#endif

