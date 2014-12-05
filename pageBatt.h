#ifndef PAGEBATT_H
#define	PAGEBATT_H

#include <Arduino.h>
#include "oled.h"

#define WARNDELAY 10000

class pageBatt {
    public:
        pageBatt();
        void showPage(bool newPage);
        String getTitle();
        void powerCheck();
        void doBtn(byte btn);
    private:       
        void resetVars();
        void charge();
        void drawBattery(byte x, byte y, bool dead);
        void drawBattBar(byte x, byte y, byte bar);
        void battWarn(bool dead);       
        unsigned long previousMillis; 
        bool barActive;
        bool justReset;
              
};

#endif

