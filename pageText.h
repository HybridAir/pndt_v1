#ifndef PAGETEXT_H
#define	PAGETEXT_H

#include <Arduino.h>
#include "oled.h"

class pageText {
    public:
        pageText();
        void showPage();
        void scrollText();
    private:       
        int position;
              
};

#endif

