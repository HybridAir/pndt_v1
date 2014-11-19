#ifndef IO_H
#define	IO_H

#include <Arduino.h>
#include <Bounce.h> 

#define AREF        2.495

#define BTNL        10                                                          //left button
#define BTNC        9                                                           //center button
#define BTNR        8                                                           //right button
#define TMP         A5                                                          //temperature pin

#define OFF         1                                                           //off pin
#define ON          0                                                           //on pin
#define CHRG        5                                                           //charge status pin
#define BATT        A0                                                          //battery voltage pin

#define AVG         20                                                          //amount of entries to keep for running averages

class io {
    public:
        io();
        void ioMon();
        byte btnMon();
        void processTmp();
        float getTmp();
        void turnOff();
        byte getCharge();
        void monitorCharge();
        void processBatt();
        float getBatt();
    private:
        Bounce btnL;
        Bounce btnC;
        Bounce btnR;
        int tmp;
        int batt;
        bool lastCharge;
        
        int tmpReadings[AVG];
        int tmpIndex;
        int tmpTotal;
        int batReadings[AVG];
        int batIndex;
        int batTotal;
        
        bool active;    //temporary
              
};

#endif
