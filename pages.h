#ifndef PAGES_H
#define	PAGES_H

#include <Arduino.h>
#include "oled.h"

#include "io.h"
#include "rtcTime.h"
#include "settings.h"

class pages {
    public:
        pages();
        void doPage();
        void doBtn(byte btn);
    private:
        void debug();
        void scrollText();        
        void charge();
        void drawBattery(byte x, byte y);
        void drawBattBar(byte x, byte y, byte bar);
        int x;
              
};

#endif

