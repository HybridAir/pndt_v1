#ifndef PAGES_H
#define	PAGES_H

#include <Arduino.h>
#include "oled.h"
#include "io.h"
#include "rtcTime.h"
#include "settings.h"

#include "pageBatt.h"
#include "pageText.h"
#include "pageHome.h"

#define BARDELAY 1000
#define TITLEDELAY 1000

class pages {
    public:
        pages();
        void doPage();
        void doBtn(byte btn);
    private:
        void debug();
        void scrollText();        
        void doTitle(byte page);
        //int x;
        byte prevPage;
        bool showPage;
        unsigned long prevTitleTime;   
        byte newPage;
        byte justReset;
};

#endif

