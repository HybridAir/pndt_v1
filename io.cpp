//handles IO devices (buttons, temp, time, and power) Does not handle the display
//functions with a *** in the comment must be ran often for best results

#include "io.h"

io::io() {                                                                      //default constructor
    analogReference(EXTERNAL);                                                  //an external 2.5v analog reference is being used
    
    //initialize button inputs
    pinMode(BTNL, INPUT);                                                       
    pinMode(BTNC, INPUT);
    pinMode(BTNR, INPUT);
    //initialize power switch outputs
    pinMode(OFF, OUTPUT);                                                       
    pinMode(ON, OUTPUT);
    
    //set up button debouncing
    btnL = Bounce();   
    btnC = Bounce();   
    btnR = Bounce();   
    btnL.attach(BTNL);
    btnC.attach(BTNC);
    btnR.attach(BTNR);
}

byte io::btnMon() {                                                             //***checks buttons, and returns which ones have been pressed
    byte out = 0;                                                               //the variable that holds the triplet output value
    
    if(btnL.update() && !btnL.read()) {                                         //if BTNL's state just changed to low (pressed)
        out += 1;                                                               //add 1 to the triplet output
    }
    if(btnC.update() && !btnC.read()) {                                         //if BTNC's state just changed to low (pressed)
        out += 2;                                                               //add 2 to the triplet output
    }
    if(btnR.update() && !btnR.read()) {                                         //if BTNR's state just changed to low (pressed)
        out += 4;                                                               //add 4 to the triplet output
    }
    
    return out;                                                                 //returns the button status (0 = none, 1 = btnl, 3 = btnl AND btnc, etc)   
}

void io::powerSwitch(bool level) {                                                  //pulses the power switch on or off (does not hold output)
    //check powerStatOn or powerStatOff to check if it's already being held on/off                                                  
}

void io::powerHold(bool level, bool enable) {                                       //holds the power switch on/off until disabled
    //check to make sure nothing is being held
}

byte io::getCharge() {                                                              //returns the charge status
    
}

void io::monitorCharge() {                                                          //***used to monitor the charge status, and to automatically switch the power on or off
    
}

float io::getBatt() {                                                               //returns corrected battery level as a float
    
}