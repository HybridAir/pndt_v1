#ifndef SETTINGS_H
#define	SETTINGS_H

#include <Arduino.h>
#include <EEPROM.h>

class settings {
    public:
        settings();
        byte getrom();
        byte getPage();
        void setPage(byte in);
        void nextPage();
        void prevPage();
private:
    byte mode;
};

#endif

