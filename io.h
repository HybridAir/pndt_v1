#ifndef IO_H
#define	IO_H

#include <Arduino.h>
#include <Bounce.h> 

#define BTNL        10                                                          //left button
#define BTNC        9                                                           //center button
#define BTNR        8                                                           //right button
#define TMP         A5                                                          //temperature pin

#define OFF         1                                                           //off pin
#define ON          0                                                           //on pin
#define CHRG        5                                                           //charge status pin
#define BATT        A0                                                          //battery voltage pin

class io {
    public:
        io();
        byte btnMon();
        void powerSwitch(bool level);
        void powerHold(bool level, bool enable);
        byte getCharge();
        void monitorCharge();
        float getBatt();
    private:
        Bounce btnL;
        Bounce btnC;
        Bounce btnR;
              
};

#endif
