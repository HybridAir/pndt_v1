//used to handle settings related things (setting current page, auto shutdown, time setting, all that fun stuff)

#include "settings.h"

settings::settings() {
    //put default settings in here   
    currentPage = 0;
    
    
    byte reg = 0;
    byte value;
    value = EEPROM.read(reg);	//read the first register only
    if(value > 9) {
        mode = 0;
    }
    else {
      mode = value;
    }
    byte out = mode + 1;
    EEPROM.write(reg, out);
}

byte settings::getrom() {
    return mode;
}

byte settings::getPage() {
    return currentPage;
}

void settings::nextPage() {
    if(currentPage == NUMPAGES) {           //rollover the page number to 0 if we're already at the last page
        currentPage = 0;
    }
    else {
        currentPage++;
    }
}

void settings::prevPage() {     //need to make it so the page only gets set once per press
    if(currentPage == 0) {           //rollover to the last page if we're already at the first page
        currentPage = NUMPAGES;
    }
    else {
        currentPage--;
    }
}

void settings::setPage(byte in) {
    
}