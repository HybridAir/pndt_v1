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
    
    //initialize running average arrays to 0 
    for (byte index = 0; index < AVG; index++)
        tmpReadings[index] = 0;
        batReadings[index] = 0;
    }

void io::ioMon() {                                                              //***monitors IO, keeps running averages, etc

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

void io::processTmp() {                                                         //***keeps a running average of the temperature input  
    tmpTotal= tmpTotal - tmpReadings[tmpIndex];                                 //subtract the last reading from the total                                    
    analogRead(TMP);                                                            //dummy analogread to keep the ADC happy                                            
    delayMicroseconds(10);                                                      //wait an insignificant amount of time
    tmpReadings[tmpIndex] = analogRead(TMP);                                    //put a potentially gross tmp reading into the current index
    tmpTotal= tmpTotal + tmpReadings[tmpIndex];                                 //add that reading to the total     
    tmpIndex = tmpIndex + 1;                                                    //advance to the next position of the array              
    if (tmpIndex >= AVG) {                                                      //check if we are at the end of the array        
        tmpIndex = 0;                                                           //reset the position     
    }
    tmp = tmpTotal / AVG;                                                       //get the smooth and creamy average out       
}

float io::getTmp() {                                                            //returns the converted temperature as a float
    return (((tmp * ((AVG*1000.0) / 1024.0)) - 500) / 10.0);                    //do some witchcraft and return the converted float
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

void io::processBatt() {                                                        //***keeps a running average of the battery voltage     
    batTotal= batTotal - batReadings[batIndex];                                 //subtract the last reading from the total                                    
    analogRead(BATT);                                                            //dummy analogread to keep the ADC happy                                            
    delayMicroseconds(10);                                                      //wait an insignificant amount of time
    batReadings[batIndex] = analogRead(BATT);                                    //put a potentially gross tmp reading into the current index
    batTotal= batTotal + batReadings[batIndex];                                 //add that reading to the total     
    batIndex = batIndex + 1;                                                    //advance to the next position of the array              
    if (batIndex >= AVG) {                                                      //check if we are at the end of the array        
        batIndex = 0;                                                           //reset the position     
    }
    batt = batTotal / AVG;                                                       //get the smooth and creamy average out  
}

float io::getBatt() {                                                           //returns corrected battery voltage as a float
    return (batt * (AREF / 1024.0));                                            //convert the value to a true voltage and return it
}