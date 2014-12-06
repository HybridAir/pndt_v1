#ifndef PAGEHOME_H
#define	PAGEHOME_H

#include <Arduino.h>
#include "oled.h"


class pageHome {
    public:
        pageHome();
        void showPage(bool newPage);
        String getTitle();
    private:       
        void setHour(byte in);
        byte position;
        byte prevHour;
        byte x;
        unsigned long previousMillis;
              
};

#endif

