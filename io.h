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

#define AVG         20                                                          //amount of entries to keep for running averages
#define DELAY       30000                                                        //time to delay running average checks
#define AREF        2.42                                                        //internal calibrated analog reference voltage
#define LOWBATT     3.5                                                         //low battery voltage
#define DEADBATT    3.45                                                        //dead battery voltage

class io {
    public:
        io();
        void ioMon();
        byte btnMon();
        void processTmp(bool stabilizing);
        float getTmp();
        void turnOff();
        byte getCharge();
        void monitorCharge();
        void processBatt(bool stabilizing);
        float getBatt();
        byte getBattPercent();
        byte monitorBatt();
    private:
        Bounce btnL;
        Bounce btnC;
        Bounce btnR;
        int tmp;
        int batt;
        int lastBatt;
        int lastTmp;
        bool lastCharge;
        int tmpReadings[AVG];
        byte tmpIndex;
        int tmpTotal;
        int batReadings[AVG];
        byte batIndex;
        int batTotal;
        bool battStable;
        
        bool tmpStable;
        
        bool active;    //temporary
        
        unsigned long previousMillis;   
        unsigned long previousMillis2; 
              
};

#endif
