#ifndef MENUS_H
#define	MENUS_H

#include <Arduino.h>
#include "oled.h"
#include "settings.h"

#define NUMITEMS 2

class menus {
    public:
        menus();
        void doMain();
        void prevItem();
        void nextItem();
        void doBtn(byte btn);
    private:
        void drawList();
        void drawListItem(String text, byte index);
        byte activeItem;

};

#endif

