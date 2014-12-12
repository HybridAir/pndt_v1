#ifndef MENUTIME_H
#define	MENUTIME_H

#include <Arduino.h>
#include "oled.h"
#include "settings.h"
#include "menus.h"

#define MAXUNITS 4
#define MAXHOUR 12
#define MAXMIN 59


class menuTime {
    
    public:
        menuTime();
        void doMenu(byte btn);
        String getTitle();
        
    private:
        void doBtn();
        void doTime();
        void doDate();
        void finishTime();
        String title;
        byte currentPage;
        byte currentBtn;
        byte currentTime;
        byte activeUnit;

};

#endif

