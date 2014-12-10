#ifndef PAGEHOME_H
#define	PAGEHOME_H

#include <Arduino.h>
#include "oled.h"

#define FONTHEIGHT 8
#define FONTWIDTH 6
#define MAXDATA 2
#define DATAHEIGHT 18


class pageHome {
    public:
        pageHome();
        void showPage(bool newPage);
        String getTitle();
    private:    
        byte x;
        byte y;
        byte currentData;
        bool setNewData;
        byte newData;
        int dataPos;
        bool scrollingOut;
        unsigned long previousMillis;
        void scrollData(byte x, byte y);
        void drawData(byte x, byte y, byte data);
              
};

#endif

