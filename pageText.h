#ifndef PAGETEXT_H
#define	PAGETEXT_H

#include <Arduino.h>
#include "oled.h"

class pageText {
    public:
        pageText();
        void showPage(bool newPage);
        String getTitle();
        void scrollText();
    private:       
        void resetVars();
        int position;
        byte message;
        String out;
              
};

#endif

