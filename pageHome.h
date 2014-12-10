#ifndef PAGEHOME_H
#define	PAGEHOME_H

#include <Arduino.h>
#include "oled.h"

#define FONTHEIGHT 8
#define FONTWIDTH 6
#define MAXDATA 2
#define DATAHEIGHT 18
#define DATADELAY 5
#define BARWIDTH 8


class pageHome {
    public:
        pageHome();
        void showPage(bool newPage);
        String getTitle();
    private:    
        void updateVars();
        byte x;
        byte y;
        byte currentData;
        bool setNewData;
        byte newData;
        byte dataPos;
        byte prevSec;
        byte dataDelay;
        bool scrollingOut;
        //unsigned long previousMillis;
        void scrollData(byte x, byte y);
        void drawData(byte x, byte y, byte data);
        void drawBattBar(byte x, byte y, byte bar);
              
};

#endif

