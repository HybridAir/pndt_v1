#ifndef SETTINGS_H
#define	SETTINGS_H

#include <Arduino.h>
#include <EEPROM.h>

#define NUMPAGES 1

class settings {
    public:
        settings();
        byte getrom();
        byte getPage();
        void setPage(byte in);
        void nextPage();
        void prevPage();
        void setMenu();
        bool getMenu();
        byte getActiveMode();
private:
    byte mode;
    byte currentPage;
    bool menuActive;
};

#endif

