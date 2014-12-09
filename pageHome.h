#ifndef PAGEHOME_H
#define	PAGEHOME_H

#include <Arduino.h>
#include "oled.h"

#define FONTHEIGHT 8
#define FONTWIDTH 6


class pageHome {
    public:
        pageHome();
        void showPage(bool newPage);
        String getTitle();
    private:    
        byte x;
        byte y;
        unsigned long previousMillis;
              
};

#endif

