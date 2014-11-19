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
    
    //initialize running average variables to 0
    for (byte index = 0; index < AVG; index++) {
        tmpReadings[index] = 0;
        batReadings[index] = 0;
    }
    tmpTotal = 0;
    tmpIndex = 0;
    batTotal = 0;
    batIndex = 0;
    
    lastCharge = false;                                                             //assume the device is currently unplugged
    active = false;                                             //temporary

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

void io::turnOff() {                                                            //a simple way to turn the power switch off
    digitalWrite(ON, LOW);                                                       //stop the power switch from being held ON (can't turn off if it is)
    digitalWrite(OFF, HIGH);                                                     //make the power switch turn OFF (device operation will end here)
}

byte io::getCharge() {                                                          //checks the tri-state status output from the charger IC
    digitalWrite(CHRG, LOW);                                                    //disable pull-up, weak pull-down is currently active
    if(digitalRead(CHRG)) {                                                     //a high reading only has one result, so end here
        return 2;                                                               //charging has been completed (or the battery is not connected)
    }
    digitalWrite(CHRG, HIGH);                                                   //enable the pull-up, the weak pull-down is too weak to matter
    if(digitalRead(CHRG)) {                                                     //it turns out that the input was only kept low by the pull-down before this
        digitalWrite(CHRG, LOW);                                                //disable the pull-up to save power
        return 0;                                                               //the charger is offline/unpowered
    }
    
    digitalWrite(CHRG, LOW);                                                    //disable the pull-up to save power (if we managed to get here)
    return 1;                                                                   //the pin is basically pulled to ground, charger is charging
}

void io::monitorCharge() {                                                      //***used to monitor the charge status and automatically switch the power on or off
    //the device turns on automatically when charging, by design
    //lets the user know that the device is charging and how close it is
    //turning off while charging is disabled, by design
    
    switch(getCharge()) {
        case 0:                                                                 //used to turn the device OFF after being unplugged
            if(!active) {                                                       //make sure that the device was not being used
                if(lastCharge) {                                                //check if the device is charged or was just charging (it will be normally unplugged you know)
                    turnOff();
                }
            }
            lastCharge = false;                                                //if we get here, the device was already unplugged or is active, so keep it that way
            break;
        case 1:                                                                 //used to hold the device ON while charging or charged
        case 2:
            //this is done to make sure the user knows when the device is being charged
            //it shouldn't be turned off to help prevent the user from forgetting it's plugged in
            //not that it should hurt anything, but I didn't design auto USB power cutoff for reasons
            //we don't want to give the lipo any reason to be pissed off (very unlikely)
            lastCharge = true;
            digitalWrite(ON, HIGH);
            break;                     
    }  
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