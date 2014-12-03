#ifndef OLED_H
#define	OLED_H

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>

#include "pages.h"
#include "settings.h"
#include "menus.h"


#define OLED_DC     4
#define OLED_CS     6
#define OLED_RESET  12
#define OLED_WIDTH  128


class oled {
    public:
        oled();
        void begin();
        void dispMon();
private:
    //void drawBtnBar();
              
};

#endif

