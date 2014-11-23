//used to handle settings related things (setting current page, auto shutdown, time setting, all that fun stuff)

#include "settings.h"

settings::settings() {
    //put default settings in here   
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
    
}

void settings::nextPage() {
    
}

void settings prevPage() {
    
}

void settings::setPage(byte in) {
    
}