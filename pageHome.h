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
              
};

#endif
