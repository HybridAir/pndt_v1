#ifndef MENUABOUT_H
#define	MENUABOUT_H

#include <Arduino.h>
#include "oled.h"
#include "settings.h"
#include "menus.h"

class menuAbout {
    
    public:
        menuAbout();
        void doMenu(byte btn);
        String getTitle();
        
    private:
        String title;

};

#endif

