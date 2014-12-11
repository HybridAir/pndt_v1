#ifndef OLED_H
#define	OLED_H

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>

#include "pages.h"
#include "settings.h"
#include "menus.h"
#include "io.h"
#include "pageBatt.h"

#define OLED_DC     4
#define OLED_CS     6
#define OLED_RESET  12
#define USABLECHARS 16
#define CHARMARGIN  16


class oled {
    public:
        oled();
        void begin();
        void dispMon();
        void drawBtnBar(char left, String center, char right);
        void centerText(byte y, byte size, String in);
        void showTitle(String in);
private:
    
              
};

#endif

