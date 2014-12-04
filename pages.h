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
        void debug();
        void scrollText();
        void doBtn(byte btn);
private:
    int x;
              
};

#endif

