//handles IO devices (buttons, temp, time, and power) Does not handle the display
//functions with a *** in the comment must be ran often for best results

#include "io.h"

io::io() {                                                                      //default constructor
    analogReference(INTERNAL);                                                  //using the internal analog reference
    
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
    battStable = false;                                                         //assume the battery voltage average is unstable
    lastCharge = false;                                                         //assume the device is currently unplugged
    active = false;                                             //temporary
    lastBatt = -1;                                                              //set to something outside the battery average threshold
    
    previousMillis = 0;                                                         //stores the last time the battery average was taken

}

void io::ioMon() {                                                              //***monitors IO, keeps background values up to date
    processTmp();
    processBatt();
    monitorCharge();
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
    digitalWrite(ON, LOW);                                                      //stop the power switch from being held ON (can't turn off if it is)
    digitalWrite(OFF, HIGH);                                                    //make the power switch turn OFF (device operation will end here)
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
            lastCharge = false;                                                 //if we get here, the device was already unplugged or is active, so keep it that way
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
    battStable = false;                                                         //assume battery average is unstable
    bool checkAvg = false;                                                      //don't allow getting a new average yet
    
    //the battery voltage average is kept clean by using a delay
    if(batt <= (lastBatt + 0.01) && batt >= (lastBatt - 0.01)) {                //check if the average battery voltage is +- .01 of the previous value
        battStable = true;                                                      //battery voltage can be considered stable
    }
    else {                                                                      //the average voltage deviates more than +- .01
        battStable = false;                                                     //battery voltage is considered unstable
    }

    if(battStable) {                                                            //if the battery voltage is stable
        //use a timer to allow a new average every DELAY
        unsigned long currentMillis = millis();                                 //get the current time (maybe get this from the time chip?????)
        if(currentMillis - previousMillis > DELAY) {                            //check if 100 ms have elapsed
            previousMillis = currentMillis;                                     //save the current time as the previous amount
            checkAvg = true;                                                    //let the program get the new average
        }
    }
    else {                                                                      //if the battery voltage is unstable (just turned on, etc)
        checkAvg = true;                                                        //skip the delay, and allow getting the new average asap
    }

    if(checkAvg) {                                                              //get a new average only if it's time to
        lastBatt = batt;                                                        //get the old average for comparing
        batTotal= batTotal - batReadings[batIndex];                             //subtract the last reading from the total                                    
        analogRead(BATT);                                                       //dummy analogread to keep the ADC happy                                            
        delayMicroseconds(10);                                                  //wait an insignificant amount of time
        batReadings[batIndex] = analogRead(BATT);                               //put a potentially gross tmp reading into the current index
        batTotal= batTotal + batReadings[batIndex];                             //add that reading to the total     
        batIndex = batIndex + 1;                                                //advance to the next position of the array              
        if (batIndex >= AVG) {                                                  //check if we are at the end of the array        
            batIndex = 0;                                                       //reset the position     
        }
        batt = batTotal / AVG;                                                  //get the smooth and creamy average out  
    }
}

float io::getBatt() {                                                           //returns corrected battery voltage as a float
    return 2.0*(batt * (AREF / 1024.0));                                        //convert the value to a true voltage and return it
}

byte io::monitorBatt() {                                                        //***used to trigger low battery warnings, returns error level
    //check when the device is first turned on (fullscreen dead batt)
    //check while normal operation for low
    //check when the batt is dead during normal operation (like first)
    
    if(battStable && getCharge() == 0) {                                        //only return a low warning if the battery voltage average is stable and not charging
        if(getBatt() >= LOWBATT) {                                              //if the battery is not low or dead
            return 0;
        }
        else if(getBatt() < LOWBATT && getBatt() >= DEADBATT) {                 //if the battery is low but not dead
            return 1;                                                           //return the low warning value
        }
        else {                                                                  //the battery is probably dead
            return 2;                                                           //return the dead warning value
        }
    }
    return 0;
}